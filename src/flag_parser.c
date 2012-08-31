#ifdef ENABLE_FLOW_FLAGS
#include "constants.h"
#include "flag_parser.h"
#include "flow_flag.h"

#include <assert.h>
#include <resolv.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include "anonymization.h"


int add_flag(flag_table_t* flag_table,
                         uint16_t flow_id,
                         uint8_t th_flags)
                         {
  flow_flag_entry_t entry;
  entry.flow_id = flow_id;
  entry.th_flags = th_flags;
  flag_table_add(flag_table, &entry);
return 0;
}

//FABIAN: You will also need some update functionality.
//Actually you will more often update than add.
//I would put this in a process_flags functions that creates a new entry if
//the packet belongs to a new flow, and updates the existing value if the
//packet belongs to a flow that has been added already
//AHLEM ok, if the flow id doent exist insert if then update

int process_flag_packet(const uint8_t* const bytes,
                       int len,
                       flag_table_t* const flag_table,
                       uint16_t flow_id)
{   int idx
	for (idx = 0; idx < flag_table->len; ++idx)
	{ 
		if (flag_table->entries[idx].flow_id <> flow_id ) {
			add_flag(flag_table, flow_id, th_flag);
		}else {
			//AHLEM how to exactly update ? do and & with the previous value?
			flag_table->entries[idx].th_flag =  ;
			}
	}
  
  return 0;
}
#endif
