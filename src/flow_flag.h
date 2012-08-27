#ifndef _BISMARK_PASSIVE_FLOW_FLAG_H_
#define _BISMARK_PASSIVE_FLOW_FLAG_H_

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <zlib.h>

#include "constants.h"

typedef struct { 
	uint8_t th_flags;     
    uint16_t flow_id;
  
} flow_flag_entry_t;  

typedef struct {
	
  flow_flag_entry_t entries[FLOW_FLAG_ENTRIES];
   time_t base_timestamp_seconds;
	
  } flow_flag_t; 

void flow_flag_init(flow_flag_t* const flag);

void flow_flag_entry_init(flow_flag_entry_t* const fentry);


/* Advance the base timestamp to a new value. This will rewrite offsets of
 * existing flows to match the new base timestamp, which can cause flows to be
 * deleted if the new base makes the offsets larger than INT16_MAX. */
void flow_flag_advance_base_timestamp(flow_flag_t* const flag,
                                       time_t new_timestamp);

/* Write entries in the hash table that are marked ENTRY_OCCUPIED_BUT_UNSENT,
 * then update their state to ENTRY_OCCUPIED. This ensures each flow record is
 * only sent once. */
int flow_flag_write_update(flow_flag_t* const flag, gzFile handle);

#ifndef DISABLE_FLOW_THRESHOLDING
/* Each flow maintains a count of the number of packets in that flow, up to the
 * first 64 packets. This function inspects these counts and writes the IP
 * addresses of the flow out to disk if it exceeds FLOW_THRESHOLD. This is only
 * done before the first update is prepared, to prevent redundant flows.
 *
 * This feature was added to support running active measurements
 * against the set of hosts accessed by the home network. */
int flow_flag_write_thresholded_ips(const flow_flag_t* const flag,
                                     const uint64_t session_id,
                                     const int sequence_number);
#endif

#ifndef NDEBUG
void testing_set_hash_function(uint32_t (*hasher)(const char* data, int len));
#endif

#endif
