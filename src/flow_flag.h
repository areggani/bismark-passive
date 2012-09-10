#ifndef _BISMARK_PASSIVE_FLOW_FLAG_H_
#define _BISMARK_PASSIVE_FLOW_FLAG_H_

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <zlib.h>

#include "constants.h"
#include "flow_table.h"



struct th_flags {
	u_int8_t fin:1;
	u_int8_t syn:1;
	u_int8_t rst:1;
	u_int8_t psh:1;
	u_int8_t ack:1;
	u_int8_t urg:1;
	u_int8_t res2:2;  
}; 


typedef struct { 
	struct th_flags th_flags;     
	uint16_t flow_id;
	
} flow_flag_entry_t;

typedef struct {
	
	flow_flag_entry_t entries[FLOW_FLAG_ENTRIES];
	int length;
	int num_dropped_flag_entries;	
  } flag_table_t; 

void flag_table_init(flag_table_t* const flag_table);

/*Add a new flag to the table*/
int flag_table_add(flag_table_t* const flag_table, flow_flag_entry_t* const entry);


/* Serialize all table data to an open gzFile handle. */
int flag_table_write_update(flag_table_t* const flag_table, gzFile handle);

#endif
