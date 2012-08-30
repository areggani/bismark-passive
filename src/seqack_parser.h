#ifndef _BISMARK_PASSIVE_SEQACK_PARSER_
#define _BISMARK_PASSIVE_SEQACK_PARSER_

#include <stdint.h>

#include "packet_seqack.h"

/* Parse a packet and add seq and ack No to table*/
int process_dns_packet(const uint8_t* const bytes,
                       int len,
                       seqack_table_t* const seqack_table,
                       uint16_t packet_id);

#endif
