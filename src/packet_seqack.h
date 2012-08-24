//
//  packet_seqack_table.h
//  packet_seqack_table
//
//  Created by Ahlem Reggani on 15/12/11.
//  Copyright 2011 LIP6. All rights reserved.
//


#ifndef _BISMARK_PASSIVE_PACKET_SEQACK_H_
#define _BISMARK_PASSIVE_PACKET_SEQACK_H_

typedef	u_int32_t tcp_seq;

#include <stdint.h>  /*TO CHECK */
#include <stdio.h>
#include <sys/time.h>
#include <zlib.h>   

#include "constants.h" 

/* Information about a single packet. */
typedef struct {
				tcp_seq th_seq;     /* sequence number */
				tcp_seq th_ack;     /* acknowledgement number */
} packet_data_t;

/** A data structure for storing information about series of packets. For space
 * efficiency, we assume at most 2^31 microseconds (~36 minutes) between packets
 * in the series. This is fine since there's generally a lot of ambient traffic
 * on networks, and we send updates much more often than that anyway. */
/*+ typedef struct {
	/* The timestamp of the first packet in the series. */
/*+	int64_t start_time_microseconds;
	/* The timestamp of the last packet added to the series. */
/*+	int64_t last_time_microseconds;
	/* The number of packets received so far. */
/*+	int32_t length;
	
	packet_data_t packet_data[PACKET_DATA_BUFFER_ENTRIES];
	
	/* If length >= PACKET_DATA_BUFFER_ENTRIES, new packets are discarded because
	 * there is no space for them. */
/*+	uint32_t discarded_by_overflow;
} packet_series_t; +*/  

void packet_seqack_init(packet_seqack_t* const seqack);

/* Add a packet to the end of the packet series. timestamp should be an absolure
 * timestamp (e.g., as provided by libc or libpcap. Does not take ownership of
 * timestamp. flow must be a valid index into the flow table, or -1 if no such
 * index exists. */
int packet_seqack_add_packet(
							 packet_seqack_t* const packet_seqack,
							 tcp_seq th_seq,
 							 tcp_seq th_ack);

/* Serialize all time series entries to an open gzFile handle. */
int packet_seqack_write_update(const packet_seqack_t* const seqack, gzFile handle);*/

#endif