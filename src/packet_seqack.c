#ifdef ENABLE_PACKET_SEQACK
#include "packet_seqack.h"

#include <inttypes.h>  
#include <stdio.h>
#include <string.h> +*/

void packet_seqack_init(packet_seqack_t* seqack) {
	memset(seqack, '\0', sizeof(*seqack));
}

int packet_seqack_add_packet(
							 packet_seqack_t* const seqack,
						/*+	 const struct timeval* const timestamp,
							 uint32_t size,
							 int flow) +*/ 
							tcp_seq th_seq,     
							tcp_seq th_ack){
	
	if (seqack->length >= PACKET_DATA_BUFFER_ENTRIES) {
		if (seqack->discarded_by_overflow + 1 > seqack->discarded_by_overflow) {
			++seqack->discarded_by_overflow;
		}
		return -1;
	}
	
	int64_t current_timestamp_microseconds
	= timestamp->tv_sec * NUM_MICROS_PER_SECOND + timestamp->tv_usec;
	if (seqack->length == 0) {
		seqack->start_time_microseconds = current_timestamp_microseconds;
		seqack->packet_data[seqack->length].timestamp = 0;
	} else {
		seqack->packet_data[seqack->length].timestamp
		= current_timestamp_microseconds - series->last_time_microseconds;
	}
	seqack->packet_data[series->length].size = size;
	series->packet_data[series->length].flow = flow;
	series->last_time_microseconds = current_timestamp_microseconds;
	++series->length;
	
	return series->length - 1;
}

int packet_seqack_write_update(const packet_series_t* const seqack,
                               gzFile handle) {
	if (!gzprintf(handle,
				  "%" PRIu64 " %" PRIu32 "\n",
				  series->start_time_microseconds,
				  series->discarded_by_overflow)) {
		perror("Error writing update");
		return -1;
	}
	int idx;
	for (idx = 0; idx < series->length; ++idx) {
		int flow_id = series->packet_data[idx].flow;
		if (flow_id >= FLOW_TABLE_ENTRIES) {
			flow_id = -1;
		}
		if (!gzprintf(handle,
					  "%" PRId32 " %" PRIu16 " %d\n",
					  series->packet_data[idx].th_seq,
					  series->packet_data[idx].th_ack,
					  flow_id)) {
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
