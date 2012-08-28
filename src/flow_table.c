#include "flow_table.h"

#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "anonymization.h"
#include "constants.h"
#include "hashing.h"

#ifdef TESTING
static uint32_t (*alternate_hash_function)(const char* data, int len) = NULL;
#endif

static int flow_entry_compare(const flow_table_entry_t* const first,
                              const flow_table_entry_t* const second) {
  return first->ip_source == second->ip_source
      && first->ip_destination == second->ip_destination
      && first->transport_protocol == second->transport_protocol
      && first->port_source == second->port_source
      && first->port_destination == second->port_destination
		#ifdef ENABLE_FLOW_FLAG
		&& first->th_flags == second->th_flags;
		#endif
}

void flow_table_init(flow_table_t* const table) {
  memset(table->entries, '\0', sizeof(*table));
}

void flow_table_entry_init(flow_table_entry_t* const entry) {
  memset(entry, '\0', sizeof(*entry));
}

int flow_table_process_flow(flow_table_t* const table,
                            flow_table_entry_t* const new_entry,
                            time_t timestamp_seconds) {
  const int hash_size = sizeof(new_entry->ip_source)
                      + sizeof(new_entry->ip_destination)
                      + sizeof(new_entry->port_source)
                      + sizeof(new_entry->port_destination)
                      + sizeof(new_entry->transport_protocol)
						#ifdef ENABLE_FLOW_FLAG
						+ sizeof(new_entry->th_flags);
						#endif
  uint32_t hash = fnv_hash_32((char *)new_entry, hash_size);
#ifdef TESTING
  if (alternate_hash_function) {
    hash = alternate_hash_function((char *)new_entry, sizeof(*new_entry));
  }
#endif

  /* Don't let the last_update of a flow exceed its datatype bounds. */
  if (table->num_elements > 0
      && (timestamp_seconds - table->base_timestamp_seconds
            > FLOW_TABLE_MAX_UPDATE_OFFSET
        || timestamp_seconds - table->base_timestamp_seconds
            < FLOW_TABLE_MIN_UPDATE_OFFSET)) {
    ++table->num_dropped_flows;
    return FLOW_ID_ERROR;
  }

  int first_available = -1;
  int probe;
  for (probe = 0; probe < HT_NUM_PROBES; ++probe) {
    uint32_t table_idx
      = (uint32_t)(hash + HT_C1*probe + HT_C2*probe*probe) % FLOW_TABLE_ENTRIES;
    flow_table_entry_t* entry = &table->entries[table_idx];
    if (entry->occupied == ENTRY_OCCUPIED
        && table->base_timestamp_seconds
            + entry->last_update_time_seconds
            + FLOW_TABLE_EXPIRATION_SECONDS < timestamp_seconds) {
      entry->occupied = ENTRY_DELETED;
      --table->num_elements;
      ++table->num_expired_flows;
    }
    if ((entry->occupied == ENTRY_OCCUPIED
          || entry->occupied == ENTRY_OCCUPIED_BUT_UNSENT)
        && flow_entry_compare(new_entry, entry)) {
      entry->last_update_time_seconds
          = timestamp_seconds - table->base_timestamp_seconds;
#ifndef DISABLE_FLOW_THRESHOLDING
      if (entry->occupied == ENTRY_OCCUPIED_BUT_UNSENT
          && entry->num_packets < 63) {  /* 63 = 2^6 - 1, the maximum value
                                            for entry->num_packets */
        ++entry->num_packets;
      }
#endif
      return table_idx + FLOW_ID_FIRST_UNRESERVED;
    }
    if (entry->occupied != ENTRY_OCCUPIED
        && entry->occupied != ENTRY_OCCUPIED_BUT_UNSENT) {
      if (first_available < 0) {
        first_available = table_idx;
      }
      if (entry->occupied == ENTRY_EMPTY) {
        break;
      }
    }
  }

  if (first_available < 0) {
    ++table->num_dropped_flows;
    return FLOW_ID_ERROR;
  }

  if (table->num_elements == 0) {
    table->base_timestamp_seconds = timestamp_seconds;
  }
  new_entry->occupied = ENTRY_OCCUPIED_BUT_UNSENT;
#ifndef DISABLE_FLOW_THRESHOLDING
  new_entry->num_packets = 1;
#endif
  new_entry->last_update_time_seconds
      = timestamp_seconds - table->base_timestamp_seconds;
  table->entries[first_available] = *new_entry;
  ++table->num_elements;
  return first_available + FLOW_ID_FIRST_UNRESERVED;
}

void flow_table_advance_base_timestamp(flow_table_t* const table,
                                       time_t new_timestamp) {
  const time_t offset = new_timestamp - table->base_timestamp_seconds;
  int idx;
  for (idx = 0; idx < FLOW_TABLE_ENTRIES; ++idx) {
    if (table->entries[idx].occupied == ENTRY_OCCUPIED_BUT_UNSENT ||
        table->entries[idx].occupied == ENTRY_OCCUPIED) {
      if ((time_t)table->entries[idx].last_update_time_seconds - offset
          < FLOW_TABLE_MIN_UPDATE_OFFSET) {
        table->entries[idx].occupied = ENTRY_DELETED;
        --table->num_elements;
      } else {
        table->entries[idx].last_update_time_seconds -= offset;
      }
    }
  }
  table->base_timestamp_seconds = new_timestamp;
}

int flow_table_write_update(flow_table_t* const table, gzFile handle) {
  if (!gzprintf(handle,
                "%ld %" PRIu32 " %d %d\n",
                table->base_timestamp_seconds,
                table->num_elements,
                table->num_expired_flows,
                table->num_dropped_flows)) {
    perror("Error sending update");
    return -1;
  }

  int idx;
  for (idx = 0; idx < FLOW_TABLE_ENTRIES; ++idx) {
    if (table->entries[idx].occupied == ENTRY_OCCUPIED_BUT_UNSENT) {
      uint64_t source_digest, destination_digest;
#ifndef DISABLE_ANONYMIZATION
      if (table->entries[idx].ip_source_unanonymized) {
#endif
        source_digest = table->entries[idx].ip_source;
#ifndef DISABLE_ANONYMIZATION
      } else {
        if (anonymize_ip(table->entries[idx].ip_source, &source_digest)) {
          fprintf(stderr, "Error anonymizing update\n");
          return -1;
        }
      }
      if (table->entries[idx].ip_destination_unanonymized) {
#endif
        destination_digest = table->entries[idx].ip_destination;
#ifndef DISABLE_ANONYMIZATION
      } else {
        if (anonymize_ip(table->entries[idx].ip_destination,
                         &destination_digest)) {
          fprintf(stderr, "Error anonymizing update\n");
          return -1;
        }
      }
#endif

      if (!gzprintf(handle,
            "%d %d %" PRIx64 " %d %" PRIx64 " %" PRIu8 " %" PRIu16 " %" PRIu16 "\n",
            idx + FLOW_ID_FIRST_UNRESERVED,
            !table->entries[idx].ip_source_unanonymized,
            source_digest,
            !table->entries[idx].ip_destination_unanonymized,
            destination_digest,
            table->entries[idx].transport_protocol,
            table->entries[idx].port_source,
            table->entries[idx].port_destination,
			#ifdef ENABLE_FLOW_FLAG
			table->entries[idx].th_flags
			#endif 
					)) {
        perror("Error sending update");
        return -1;
      }
      table->entries[idx].occupied = ENTRY_OCCUPIED;
    }
  }
  if (!gzprintf(handle, "\n")) {
    perror("Error sending update");
    return -1;
  }

  return 0;
}

#ifndef DISABLE_FLOW_THRESHOLDING
int flow_table_write_thresholded_ips(const flow_table_t* const table,
                                     const uint64_t session_id,
                                     const int sequence_number) {
  FILE* handle = fopen(FLOW_THRESHOLDING_LOG, "w");
  if (!handle) {
    perror("Error opening thresholded flows log");
    return -1;
  }
  if (fprintf(handle, "%" PRIu64 " %d\n\n", session_id, sequence_number) < 2) {
    perror("Error writing thesholded flows log");
    fclose(handle);
    return -1;
  }
  int idx;
  for (idx = 0; idx < FLOW_TABLE_ENTRIES; ++idx) {
    if (table->entries[idx].occupied == ENTRY_OCCUPIED_BUT_UNSENT
        && table->entries[idx].num_packets >= FLOW_THRESHOLD) {
      if (fprintf(handle,
                  "%d %" PRIx32 " %" PRIx32 " %" PRIu8 "\n",
                  idx + FLOW_ID_FIRST_UNRESERVED,
                  table->entries[idx].ip_source,
                  table->entries[idx].ip_destination,
                  table->entries[idx].num_packets) < 0) {
        perror("Error writing thresholded flows log");
        fclose(handle);
        return -1;
      }
    }
  }
  fclose(handle);
  return 0;
}
#endif

#ifdef TESTING
void testing_set_hash_function(uint32_t (*hasher)(const char* data, int len)) {
  alternate_hash_function = hasher;
}
#endif
