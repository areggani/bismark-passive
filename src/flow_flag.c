#ifdef ENABLE_FLOW_FLAG
#include "flow_flag.h"

#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "anonymization.h"
#include "constants.h"  
#include "util.h"


void flag_table_init(flag_table_t* const flag_table) {
  memset(flag_table, '\0', sizeof(*flag_table));
}


int flag_table_add(flag_table_t* const flag_table,
					   flow_flag_entry_t* const new_entry) {
	if (flag_table->length >= FLOW_FLAG_ENTRIES) {
		++flag_table->num_dropped_flag_entries;
		return -1;
	}
	flag_table->entries[flag_table->length] = *new_entry;
	++flag_table->length;
	return 0;
}

int flag_table_write_update(flag_table_t* const flag_table, gzFile handle) {
	if (!gzprintf(handle,
				  "%d \n",
				  flag_table->num_dropped_flag_entries)){
		perror("Error writing update");
		return -1;
	}
	int idx;
	for (idx = 0; idx < flag_table->length; ++idx) {
			
		uint8_t flag;
		
		if ((flag = flag_table->entries[idx].th_flags) & (TH_SYN|TH_FIN|TH_RST|TH_PUSH)) {
		
		if (flag & TH_ECE) {
			flag = E;
		}if (flag & TH_RST){
			flag = R;
		}if (flag & TH_CWR){
			flag = C;
		}if (flag & TH_URG){
			flag = U;
		}if (flag & TH_ACK){
			flag = A;
		}if (flag & TH_PUSH){
			flag = P;
		}if (flag & TH_SYN){
			flag = S;
		}if (flag & TH_FIN){
			flag = F;
		}
			
	} else
			flag = NONE;
		
		if (!gzprintf(handle,
					  "%" PRIu16 "%"  PRIu8 "\n",
					  flag_table->entries[idx].flow_id,
					  flag_table->entries[idx].th_flags = flag)){
			perror("Error writing update");
			return -1;
		}
	}
	if (!gzprintf(handle, "\n")) {
		perror("Error writing update");
		return -1;
	}
	
	return 0;
}
#endif


