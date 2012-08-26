#ifdef ENABLE_FLOW_FLAG
#include "flow_flag.h"

#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "anonymization.h"
#include "constants.h"  
#include "hashing.h"

#ifdef TESTING
static uint32_t (*alternate_hash_function)(const char* data, int len) = NULL;
#endif

static int flow_entry_compare(const flow_flag_entry_t* const first,
                              const flow_flag_entry_t* const second) {
  return first->th_flags == second->th_flags; 	}

void flow_flag_init(flow_flag_t* const flag) {
  memset(flag->entries, '\0', sizeof(*flag));
}

void flow_flag_entry_init(flow_flag_entry_t* const fentry) {
  memset(entry, '\0', sizeof(*fentry));
}


