#pragma once

#include <stdint.h>

#ifndef MT_RTSP_STREAMER_H
#define MT_RTSP_STREAMER_H

#ifndef F_SETPIPE_SZ
#define F_SETPIPE_SZ	1031
#endif


struct stream_params {
    int   stream_sock;
    int   stream_pipe[2];
    int   stream_fs_fd;
    int   server_fd;
    pid_t task_id;
    char *stream_dump;
};

/*
struct stream_w {
    int   server_fd;
    pid_t task_id;
};
*/

class mt_streamer{
public:
    mt_streamer();
    ~mt_streamer();
public:
    struct stream_params* get_params();
    pid_t streamer_loop(int server_fd);
    int streamer_prepare(const char *name,
                         unsigned char *sps, int sps_len,
                         unsigned char *pps, int pps_len);
    int streamer_pipe_init(int pipefd[2]);
    int streamer_write(const void *buf, size_t count);
    int streamer_write_nal();
    int streamer_write_h264_header(unsigned char *sps_dec, size_t sps_len,
                                   unsigned char *pps_dec, size_t pps_len);
private:
    struct stream_params _stream_params;
};

#endif /*MT_RTSP_STREAMER_H*/
