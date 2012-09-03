#ifdef ENABLE_PACKET_SEQACK
#include "constants.h"
#include "packet_seqack.h."
#include "seqack_parser.h"

#include <assert.h>
#include <resolv.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>


/*process packet by adding a record to the table*/

int process_seqack_packet(seqack_table_t* seqack_table,
														uint16_t packet_id,
														uint32_t th_seq,
														uint32_t th_ack) {
seqack_entry_t entry;
entry.packet_id = packet_id;
entry.th_seq = th_seq;
entry.th_ack = th_ack;
seqack_table_add(seqack_table, &entry);
							  }
