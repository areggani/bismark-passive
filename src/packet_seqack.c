#ifdef ENABLE_PACKET_SEQACK
#include "packet_seqack.h"

#include <inttypes.h>  
#include <stdio.h>
#include <string.h> +*/

void packet_seqack_init(packet_seqack_t* seqack) {
	memset(seqack, '\0', sizeof(*seqack));
}

int packet_seqack_add_packet(packet_seqack_t* const seqack,
							 const struct timeval* const timestamp,
							 uint32_t th_seq,     
							uint32_t th_ack){
	
	if (seqack->length >= SEQACK_TABLE_ENTRIES) {
		if (seqack->discarded_by_overflow + 1 > seqack->discarded_by_overflow) {
			++seqack->discarded_by_overflow;
		}
		return -1;
	}
	
	seqack->packet_seqack[seqack->length].th_seq = th_seq;
	seqack->packet_seqack[seqack->length].th_ack = th_ack;
	++seqack->length;
	
	return seqack->length - 1;
}

int packet_seqack_write_update(const packet_seqack_t* const seqack,
                               gzFile handle) {
	
	int idx;
	for (idx = 0; idx < seqack->length; ++idx) {
		int flow_id = seqack->packet_seqack[idx].flow;
		if (flow_id >= FLOW_TABLE_ENTRIES) {
			flow_id = -1;
		}
		if (!gzprintf(handle,
					  "%" PRId32 " %" PRIu16 " %d\n",
					  seqack->packet_seqack[idx].th_seq,
					  seqack->packet_seqack[idx].th_ack
					  )) {
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

