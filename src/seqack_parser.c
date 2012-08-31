/* RFC 1305 will be very helpful for understanding this parser. */

#include "constants.h"
#include "packet_seqack.h."
#include "seqack_parser.h"

#include <assert.h>
#include <resolv.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>


/*enter a new record in the table*/
static void add_seqack_packet(seqack_table_t* seqack_table,
                         uint16_t packet_id,
							  uint32_t th_seq,
							  uint32_t th_ack) {
  seqack_entry_t entry;
  entry.packet_id = packet_id;
  entry.th_seq = th_seq;
  entry.th_ack = th_ack;
  seqack_table_add(dns_table, &entry);
}

/*process the seq and ack before adding the record*/
//AHLEM do I need to check for something before adding the seq and ack or do I simply need the add function?
int process_seqack_packet(const uint8_t* const bytes,
                       int len,
						  dseqack_table_t* const seqack_table,
						  uint16_t packet_id);
{
	
      add_seqack_record(seqack_table, packet_id, th_seq, th_ack);
  
  return 0;
}
