#ifndef _BISMARK_PASSIVE_FLAG_PARSER_
#define _BISMARK_PASSIVE_FLAG_PARSER_

#include <stdint.h>

#include "flow_flag.h"


int add_flag(flag_table_t* flag_table,
			 uint16_t flow_id,
			 struct th_flags* th_flags)
/* Parse a packet and add relevent entries to the provided flow
 * table. */

int process_flag_packet(struct th_flags* flag_bytes,
                       flag_table_t* const flag_table,
                       uint16_t flow_id);

#endif
