#ifndef _BISMARK_PASSIVE_SEQACK_PARSER_
#define _BISMARK_PASSIVE_SEQACK_PARSER_

#include <stdint.h>

#include "packet_seqack.h"

int process_seqack_packet(seqack_table_t* seqack_table,
                        uint32_t th_seq,                         
			uint32_t th_ack)  
#endif
