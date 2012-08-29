#ifdef ENABLE_PACKET_SEQACK
#include "packet_seqack.h"

#include <inttypes.h>  
#include <stdio.h>
#include <string.h> +*/

void seqack_table_init(seqack_table_t* table) {
	memset(seqack, '\0', sizeof(*table));
}

void seqack_table_destroy(seqack_table_t* const table) {
	int idx;
	for (idx = 0; idx < table->length; ++idx) {
		free(table->entries[idx].th_seq);
		free(table->entries[idx].th_ack);
	}
}

int seqack_table_add_packet(seqack_table_t* const table,
							seqack_entry_t* const new_entry) {
	if (table->length >= SEQACK_TABLE_ENTRIES) {
		++table->num_dropped_entries;
		return -1;
	}
	table->entries[table->length] = *new_entry;
	++table->length;
	return 0;
}

/*look packet_series.c */
int seqack_table_write_update(const seqack_table_t* const table,
                               gzFile handle) {
	
	int idx;
	for (idx = 0; idx < seqack->length; ++idx) {
		int packet_id = table->seqack_table[idx].packet_id;
		if (packet_id >= PACKET_DATA_BUFFER_ENTRIES) {
			packet_id = -1;
		}
		if (!gzprintf(handle,
					  "%" PRId32 " %" PRIu16 " %d\n",
					  table->seqack_entries[idx].th_seq,
					  table->seqack_entries[idx].th_ack,
					  packet_id)) {
			perror("Error writing update");
			return -1;
		}
	}
	if (!gzprintf(handle, "\n")) {
		perror("Error writing update");
		return -1;
	}
	return 0;
}


#endif

