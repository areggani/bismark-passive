#ifndef _BISMARK_PASSIVE_SEQACK_PARSER_
#define _BISMARK_PASSIVE_SEQACK_PARSER_

#include <stdint.h>

#include "packet_seqack.h"

/* Parse a packet and add seq and ack No to table*/
void add_seqack_packet(seqack_table_t* seqack_table,
						uint16_t packet_id,
						uint32_t th_seq,
						uint32_t th_ack)

int process_seqack_packet(uint32_t* const seq_bytes,
						  uint32_t* const ack_bytes,
						seqack_table_t* const seqack_table,
                       uint16_t packet_id);

{ add_seqack_packet(seqack_table, packet_id, th_seq, th_ack);
}

#endif
