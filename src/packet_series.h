#ifndef _BISMARK_PASSIVE_PACKET_SERIES_H_
#define _BISMARK_PASSIVE_PACKET_SERIES_H_

#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <zlib.h>

#include "constants.h"

/* Information about a single packet. */
typedef struct {
  /* The packet's timestamp, expressed as microseconds offset from the previous
   * packet in the series. This value can be negative if packets are processed
   * out of order. */
  int32_t timestamp;
  /* Number of bytes in the packet. */
  uint16_t size;
  /* Index into the flow table. -1 means no flow information is available. */
  uint16_t flow;
	#ifdef ENABLE_PACKET_SEQACK
	uint32_t th_seq;     /* tcp_seq sequence number */
	uint32_t th_ack;     /* acknowledgement number */
	#endif	
} packet_data_t;

/** A data structure for storing information about series of packets. For space
 * efficiency, we assume at most 2^31 microseconds (~36 minutes) between packets
 * in the series. This is fine since there's generally a lot of ambient traffic
 * on networks, and we send updates much more often than that anyway. */
typedef struct {
  /* The timestamp of the first packet in the series. */
  int64_t start_time_microseconds;
  /* The timestamp of the last packet added to the series. */
  int64_t last_time_microseconds;
  /* The number of packets received so far. */
  int32_t length;

  packet_data_t packet_data[PACKET_DATA_BUFFER_ENTRIES];

  /* If length >= PACKET_DATA_BUFFER_ENTRIES, new packets are discarded because
   * there is no space for them. */
  uint32_t discarded_by_overflow;
} packet_series_t;

void packet_series_init(packet_series_t* const series);

/* Add a packet to the end of the packet series. timestamp should be an absolure
 * timestamp (e.g., as provided by libc or libpcap. Does not take ownership of
 * timestamp. flow must be a valid index into the flow table, or -1 if no such
 * index exists. */
int packet_series_add_packet(
    packet_series_t* const packet_series,
    const struct timeval* const timestamp,
    uint32_t size,
    int flow);

/* Serialize all time series entries to an open gzFile handle. */
int packet_series_write_update(const packet_series_t* const series, gzFile handle);

#endif
