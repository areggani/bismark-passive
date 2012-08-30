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
                         uint8_t th_flags,
                         int len)
                         {
  flow_flag_entry_t entry;
  entry.flow_id = flow_id;
  
	entry.th_flags=;
  flag_table_add(flag_table, &entry);
return 0;
}


/*dont think we need this but somehting to do the logical AND for all the flags here or in the main function NO?*/
int process_flag_packet(const uint8_t* const bytes,
                       int len,
                       flag_table_t* const flag_table,
                       uint16_t flow_id)
{
  if (len <=0) return -1;
  char * argv[3];
  int n;
  if((n=tokenize((char*)bytes,argv,3,' ')) ==3) {
   if(strcasecmp(argv[0],"GET")!=0) // a GET command
    return -1;
  }
  else return -1;
  int flagcut=0;
  int length=(int)strlen(argv[1]);
  if(length>MAX_URL)
    {argv[1][MAX_URL-1]='\0';
     flagcut=1;

  add_flag(flag_table, flow_id,argv[1],flagcut);
  return 0;
}
#endif
