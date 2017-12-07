/*
 *      Copyright (C) 2017-2020 MediaTime
 *      http://media-tm.com (shareviews@sina.com)
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with MediaTime; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 *      Original Author: shareviews@sina.com (2017-12-XX)
 *   Inspiration Source: iperf
 */
 
#ifndef MTPERF_H_INCLUDED
#define MTPERF_H_INCLUDED

#ifdef WIN32
    #include <Winsock2.h>
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #define closesocket close
#endif
#include "MTTimer.h"

/* default settings */
#define P_TCP SOCK_STREAM
#define P_UDP SOCK_DGRAM
#define DEFAULT_UDP_BLKSIZE 1460 /* default is dynamically set, else this */
#define DEFAULT_TCP_BLKSIZE (128 * 1024)  /* default read/write block size */

/* default settings */
#define DEFAULT_PORT 8989  /* default port to listen on*/
#define DEFAULT_UDP_RATE (1024 * 1024) /* 1 Mbps */
#define DEFAULT_OMIT 0 /* seconds */
#define DEFAULT_DURATION 10 /* seconds */

#define uS_TO_NS 1000
#define SEC_TO_US 1000000LL
#define SEC_TO_NS 1000000000LL	/* too big for enum/const on some platforms */

#define MAX_RESULT_STRING 4096
#define MAX_UDP_BUFFER_EXTRA 1024
#define MAX_BUFFER_SIZE 1024
#define MAX_SLEEP_TIME  100 /*us*/

/* constants for command line arg sanity checks */
#define MB (1024 * 1024)
#define MAX_TCP_BUFFER (512 * MB)
#define MAX_BLOCKSIZE MB
/* Minimum size UDP send is the size of two 32-bit ints followed by a 64-bit int */
#define MIN_UDP_BLOCKSIZE (4 + 4 + 8)
/* Maximum size UDP send is (64K - 1) - IP and UDP header sizes */
#define MAX_UDP_BLOCKSIZE (65535 - 8 - 20)
#define MIN_INTERVAL 0.1
#define MAX_INTERVAL 60.0
#define MAX_TIME 86400
#define MAX_BURST 1000
#define MAX_MSS (9 * 1024)
#define MAX_STREAMS 128

struct task_stats
{
    int       domain;               /* AF_INET or AF_INET6 */
    int       socket_bufsize;       /* window size for TCP */
    int       blksize;              /* size of read/writes (-l) */
    uint64_t  rate;                 /* target data rate for application pacing*/
    uint64_t  fqrate;               /* target data rate for FQ pacing*/
    int	      pacing_timer;	        /* pacing timer in microseconds */
    int       burst;                /* packets per burst */
    int       mss;                  /* for TCP MSS */
    int       ttl;                  /* IP TTL option */
    int       tos;                  /* type of service bit */
    int       flowlabel;            /* IPv6 flow label */
    uint64_t  bytes;                 /* number of bytes to send */
    uint64_t  blocks;                /* number of blocks (packets) to send */
    char      unit_format;          /* -f */
    int	      connect_timeout;	    /* socket connection timeout, in ms */
};

struct task_settings
{
    char      role;                             /* 'c' lient or 's' erver */
    int       sender;                           /* client & !reverse or server & reverse */
    int       sender_has_retransmits;
    char     *server_hostname;                  /* -c option */
    char     *tmp_template;
    char     *bind_address;                     /* first -B option */
    int       bind_port;                        /* --cport option */
    int       server_port;
    int       omit;                             /* duration of omit period (-O flag) */
    int       duration;                         /* total duration of test (-t flag) */
    char     *diskfile_name;                    /* -F option */
    char     *title;                            /* -T option */
    char     *congestion;                       /* -C option */
    char     *congestion_used;                  /* what was actually used */
    char     *remote_congestion_used;           /* what the other side used */
    char     *logfile;                          /* --logfile option */
    FILE     *outfile;

    int       ctrl_sck;
    int       listener;
    int       prot_listener;

    /* bind */
    char *name;
    struct addrinfo *ai;

    /* boolean variables for Options */
    int       no_delay;                         /* -N option */
    int       reverse;                          /* -R option */
    int	      verbose;                          /* -V option - verbose mode */
    int	      zerocopy;                         /* -Z option - use sendfile */
    int       debug;				            /* -d option - enable debug */
    int	      get_server_output;		        /* --get-server-output */
    int	      udp_counters_64bit;		        /* --use-64-bit-udp-counters */
    int       forceflush;                       /* --forceflush - flushing output at every interval */
    int	      multisend;

    /* Select related parameters */
    int       max_fd;
    fd_set    read_set;                         /* set of read sockets */
    fd_set    write_set;                        /* set of write sockets */

    /* Interval related members */
    signed char state;
    int       done;
    int       omitting;
    double    stats_interval;
    double    reporter_interval;
    void      (*stats_callback) (struct task_setting *);
    void      (*reporter_callback) (struct task_setting *);
    Timer     *omit_timer;
    Timer     *timer;
    Timer     *stats_timer;
    Timer     *reporter_timer;

    uint64_t   bytes_sent;
    uint64_t   blocks_sent;
};

#endif // MTPERF_H_INCLUDED
