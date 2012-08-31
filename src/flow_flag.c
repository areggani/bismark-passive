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
		//Actually you should think how
		//to print out the flags, maybe
		//encode the different flags
		//with a letter:
		//e.g.
		//SYN=S,ACK=A,FIN=F,RST=R,Push=P,...
		//Then a flow with SYN,ACK and
		//FIN will be encoded as SAF
		//AHLEM but the flag is 8bit int and we want to print a char it is not a problem?
		//should this be in flag parser or here? I put the code below but dont how to write it then on the table
		u_char flag;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define FLAG (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
		
		if (flag_table->entries[idx].th_flags & TH_ECE){
			flag = E;
		}
		if (flag_table->entries[idx].th_flags & TH_RST){
			flag = R;
		}
		if (flag_table->entries[idx].th_flags & TH_CWR){
			flag = C;
		}if (flag_table->entries[idx].th_flags & TH_URG){
			flag = U;
		}if (flag_table->entries[idx].th_flags & TH_ACK){
			flag = A;
		}if (flag_table->entries[idx].th_flags & TH_PUSH){
			flag = P;
		}if (flag_table->entries[idx].th_flags & TH_SYN){
			flag = S;
		}if (flag_table->entries[idx].th_flags & TH_FIN){
			flag = F;
		}
		
		if (!gzprintf(handle,
					  //FABIAN: The flags are not a
					  //sting -> %s will not work
					  //AHLEM ok changed
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


