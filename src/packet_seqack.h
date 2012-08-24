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

#include <stdint.h>  /*TO CHECK */
#include <stdio.h>
#include <sys/time.h>
#include <zlib.h>   

#include "constants.h" 

/* Information about a single packet. */
typedef struct {
				uint32_t th_seq;     /* tcp_seq sequence number */
				uint32_t th_ack;     /* acknowledgement number */
} packet_data_t;

void packet_seqack_init(packet_seqack_t* const seqack);

/* Add a packet to the end of the packet series. timestamp should be an absolure
 * timestamp (e.g., as provided by libc or libpcap. Does not take ownership of
 * timestamp. flow must be a valid index into the flow table, or -1 if no such
 * index exists. */
int packet_seqack_add_packet(
							 packet_seqack_t* const packet_seqack,
							 uint32_t th_seq,
 							 uint32_t th_ack);

/* Serialize all time series entries to an open gzFile handle. 
int packet_seqack_write_update(const packet_seqack_t const seqack, gzFile handle); */

#endif
