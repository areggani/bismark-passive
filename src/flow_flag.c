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

void flag_table_destroy(flag_table_t* const flag_table) {
	int idx;
	for (idx = 0; idx < flag_table->length; ++idx) {
		free(flag_table->entries[idx].th_flags);
	}
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
		if (!gzprintf(handle,
					  "%" PRIu16 " 0 %s \n",
					  flag_table->entries[idx].flow_id,
					  flag_table->entries[idx].th_flags)){
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


