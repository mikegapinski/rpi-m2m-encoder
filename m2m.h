#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <assert.h>

#include <sys/mman.h>

#include <linux/videodev2.h>

#include "tools.h"
#include "logging.h"
#include "frame.h"
#include "xioctl.h"


typedef struct {
	uint8_t	*data;
	size_t	allocated;
} us_m2m_buffer_s;

typedef struct {
	int				fd;
	us_m2m_buffer_s	*input_bufs;
	unsigned		n_input_bufs;
	us_m2m_buffer_s	*output_bufs;
	unsigned		n_output_bufs;

	unsigned		width;
	unsigned		height;
	unsigned		input_format;
	unsigned		stride;
	bool			dma;
	bool			ready;

	int				last_online;
} us_m2m_encoder_runtime_s;

typedef struct {
	char			*name;
	char			*path;
	unsigned		output_format;
	unsigned		fps;
	unsigned		bitrate;
	unsigned		gop;
	unsigned		quality;
	bool			allow_dma;

	us_m2m_encoder_runtime_s *run;
} us_m2m_encoder_s;


us_m2m_encoder_s *us_m2m_h264_encoder_init(const char *name, const char *path, unsigned bitrate, unsigned gop);
us_m2m_encoder_s *us_m2m_mjpeg_encoder_init(const char *name, const char *path, unsigned quality);
us_m2m_encoder_s *us_m2m_jpeg_encoder_init(const char *name, const char *path, unsigned quality);
void us_m2m_encoder_destroy(us_m2m_encoder_s *enc);

int us_m2m_encoder_compress(us_m2m_encoder_s *enc, const us_frame_s *src, us_frame_s *dest, bool force_key);

#ifdef __cplusplus
}
#endif
