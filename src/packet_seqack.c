#ifdef ENABLE_PACKET_SEQACK
#include "packet_seqack.h"

#include <inttypes.h>  
#include <stdio.h>
#include <string.h> +*/

void seqack_table_init(seqack_table_t* table) {
	memset(seqack, '\0', sizeof(*table));
}


int seqack_table_add_packet(seqack_table_t* const table,
							seqack_entry_t* const new_entry) {
	if (table->length >= SEQACK_TABLE_ENTRIES) {
		++table->num_dropped_entries;
		return -1;
	}
	table->entries[table->length] = *new_entry;
	++table->length;
	return 0;
}

/*look packet_series.c */
int seqack_table_write_update(//
							  //FABIAN: this does not look right having two const
							  //statements in their. The first one is likely wrong.
							  //AHLEM Why is this wrong? we write an entry in the table format
							  seqack_table_t* const table,
                               gzFile handle) {
	//FABIAN: You should maybe also print a trailing line with status,
	//like in dns_table.c, line 50-56
	//AHLEM here it is
	if (!gzprintf(handle,
				  "%d %d\n",
				  table->num_dropped_entries)) {
		perror("Error writing update");
		return -1;
	}
	int idx;
	//FABIAN: their is no object/variable named seqack, i guess this
	//should be "table" 
	//AHLEM: yes
	for (idx = 0; idx < table->length; ++idx) {
		int packet_id = table->seqack_table[idx].packet_id;
		if (packet_id >= PACKET_DATA_BUFFER_ENTRIES) {
			packet_id = -1;
		}
		if (!gzprintf(handle,
					  //FABIAN: I would suggest to
					  //first write out the packet_id
					  //AHELEM: done
					  " %d" "%" PRId32 " %" PRId32 "\n",
					  packet_id,
					  table->seqack_entries[idx].th_seq,
					  table->seqack_entries[idx].th_ack)) {
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

