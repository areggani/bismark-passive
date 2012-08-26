//
//  packet_seqack_table.h
//  packet_seqack_table
//
//  Created by Ahlem Reggani on 15/12/11.
//  Copyright 2011 LIP6. All rights reserved.
//


#ifndef _BISMARK_PASSIVE_PACKET_SEQACK_H_
#define _BISMARK_PASSIVE_PACKET_SEQACK_H_

/*typedef	u_int32_t tcp_seq;*/

#include <stdint.h>  
#include <stdio.h>
#include <sys/time.h>
#include <zlib.h>   

#include "constants.h" 

/* Information about a single packet. */
typedef struct {
				uint32_t th_seq;     /* tcp_seq sequence number */
				uint32_t th_ack;     /* acknowledgement number */
} mystruct;

/*parameters*/
mystruct->th_seq = tcp_header->seq;
mystruct->th_ack = tcp_header->ack;

typedef struct{
	
	int32_t timestamp;
	mystruct seqack[SEQACK_TABLE_ENTRIES];

}packet_seqack_t

void packet_seqack_init(packet_seqack_t* const seqack);

/* Add a packet to the end of the packet series. t*/
int packet_seqack_add_packet(
							 packet_seqack_t* const packet_seqack,
							 const struct timeval* const timestamp,
							 int flow_id,
							 uint32_t th_seq,
 							 uint32_t th_ack);

/* Serialize all time series entries to an open gzFile handle. */
int packet_seqack_write_update(const packet_seqack_t const seqack, gzFile handle); 

#endif
