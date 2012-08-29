//
//  packet_seqack_table.h
//  packet_seqack_table
//
//  Created by Ahlem Reggani on 15/12/11.
//  Copyright 2011 LIP6. All rights reserved.
//


#ifndef _BISMARK_PASSIVE_PACKET_SEQACK_H_
#define _BISMARK_PASSIVE_PACKET_SEQACK_H_


#include <stdint.h>  
#include <stdio.h>
#include <sys/time.h>
#include <zlib.h>   
#include <netinet/tcp.h>

#include "constants.h" 

/* Information about a single packet. */
typedef struct {
				uint16_t packet_id;
				uint32_t th_seq;     /* tcp_seq sequence number */
				uint32_t th_ack;     /* acknowledgement number */
} seqack_entry_t;


typedef struct{
	
	uint16_t packet_id;
	seqack_entry_t entries[SEQACK_TABLE_ENTRIES];
	int length
	int num_dropped_entries;

}seqack_table_t

void seqack_table_init(seqack_table_t* const table);

/*look dns_table.h*/
void seqack_table_destroy(seqack_table_t* const table);

/*Add a new seqack_entry record to the table */
int seqack_table_add(seqack_table_t* const table, seqack_entry_t* const entry);


/* Serialize all table data to an open gzFile handle. */
int seqack_table_write_update(const seqack_table_t const table, gzFile handle); 

#endif
