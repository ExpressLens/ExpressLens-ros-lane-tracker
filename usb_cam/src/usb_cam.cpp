
/*********************************************************************
 *
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2014, Robert Bosch LLC.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the Robert Bosch nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************/
#define __STDC_CONSTANT_MACROS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <ros/ros.h>
#include <boost/lexical_cast.hpp>
#include <sensor_msgs/fill_image.h>

#include <usb_cam/usb_cam.h>

#define CLEAR(x) memset (&(x), 0, sizeof (x))

namespace usb_cam {

static void errno_exit(const char * s)
{
  ROS_ERROR("%s error %d, %s", s, errno, strerror(errno));
  exit(EXIT_FAILURE);
}

static int xioctl(int fd, int request, void * arg)
{
  int r;

  do
    r = ioctl(fd, request, arg);
  while (-1 == r && EINTR == errno);

  return r;
}

const unsigned char uchar_clipping_table[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, // -128 - -121
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, // -120 - -113
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, // -112 - -105
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, // -104 -  -97
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, //  -96 -  -89
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, //  -88 -  -81
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, //  -80 -  -73
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, //  -72 -  -65
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, //  -64 -  -57
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, //  -56 -  -49
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, //  -48 -  -41
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, //  -40 -  -33
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, //  -32 -  -25
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, //  -24 -  -17
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, //  -16 -   -9
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0, //   -8 -   -1
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88,
    89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
    114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
    137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182,
    183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205,
    206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228,
    229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251,
    252, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, // 256-263
    255, 255, 255, 255, 255, 255, 255, 255, // 264-271
    255, 255, 255, 255, 255, 255, 255, 255, // 272-279
    255, 255, 255, 255, 255, 255, 255, 255, // 280-287
    255, 255, 255, 255, 255, 255, 255, 255, // 288-295
    255, 255, 255, 255, 255, 255, 255, 255, // 296-303
    255, 255, 255, 255, 255, 255, 255, 255, // 304-311
    255, 255, 255, 255, 255, 255, 255, 255, // 312-319
    255, 255, 255, 255, 255, 255, 255, 255, // 320-327
    255, 255, 255, 255, 255, 255, 255, 255, // 328-335
    255, 255, 255, 255, 255, 255, 255, 255, // 336-343
    255, 255, 255, 255, 255, 255, 255, 255, // 344-351
    255, 255, 255, 255, 255, 255, 255, 255, // 352-359
    255, 255, 255, 255, 255, 255, 255, 255, // 360-367
    255, 255, 255, 255, 255, 255, 255, 255, // 368-375
    255, 255, 255, 255, 255, 255, 255, 255, // 376-383
    };
const int clipping_table_offset = 128;

/** Clip a value to the range 0<val<255. For speed this is done using an
 * array, so can only cope with numbers in the range -128<val<383.
 */
static unsigned char CLIPVALUE(int val)
{
  // Old method (if)
  /*   val = val < 0 ? 0 : val; */
  /*   return val > 255 ? 255 : val; */

  // New method (array)
  return uchar_clipping_table[val + clipping_table_offset];
}

/**
 * Conversion from YUV to RGB.
 * The normal conversion matrix is due to Julien (surname unknown):
 *
 * [ R ]   [  1.0   0.0     1.403 ] [ Y ]
 * [ G ] = [  1.0  -0.344  -0.714 ] [ U ]
 * [ B ]   [  1.0   1.770   0.0   ] [ V ]
 *
 * and the firewire one is similar:
 *
 * [ R ]   [  1.0   0.0     0.700 ] [ Y ]
 * [ G ] = [  1.0  -0.198  -0.291 ] [ U ]
 * [ B ]   [  1.0   1.015   0.0   ] [ V ]
 *
 * Corrected by BJT (coriander's transforms RGB->YUV and YUV->RGB
 *                   do not get you back to the same RGB!)
 * [ R ]   [  1.0   0.0     1.136 ] [ Y ]
 * [ G ] = [  1.0  -0.396  -0.578 ] [ U ]
 * [ B ]   [  1.0   2.041   0.002 ] [ V ]
 *
 */
static void YUV2RGB(const unsigned char y, const unsigned char u, const unsigned char v, unsigned char* r,
                    unsigned char* g, unsigned char* b)
{
  const int y2 = (int)y;
  const int u2 = (int)u - 128;
  const int v2 = (int)v - 128;
  //std::cerr << "YUV=("<<y2<<","<<u2<<","<<v2<<")"<<std::endl;

  // This is the normal YUV conversion, but
  // appears to be incorrect for the firewire cameras
  //   int r2 = y2 + ( (v2*91947) >> 16);
  //   int g2 = y2 - ( ((u2*22544) + (v2*46793)) >> 16 );
  //   int b2 = y2 + ( (u2*115999) >> 16);
  // This is an adjusted version (UV spread out a bit)
  int r2 = y2 + ((v2 * 37221) >> 15);
  int g2 = y2 - (((u2 * 12975) + (v2 * 18949)) >> 15);
  int b2 = y2 + ((u2 * 66883) >> 15);
  //std::cerr << "   RGB=("<<r2<<","<<g2<<","<<b2<<")"<<std::endl;

  // Cap the values.
  *r = CLIPVALUE(r2);
  *g = CLIPVALUE(g2);
  *b = CLIPVALUE(b2);
}

void uyvy2rgb(char *YUV, char *RGB, int NumPixels)
{
  int i, j;
  unsigned char y0, y1, u, v;
  unsigned char r, g, b;
  for (i = 0, j = 0; i < (NumPixels << 1); i += 4, j += 6)
  {
    u = (unsigned char)YUV[i + 0];
    y0 = (unsigned char)YUV[i + 1];
    v = (unsigned char)YUV[i + 2];
    y1 = (unsigned char)YUV[i + 3];
    YUV2RGB(y0, u, v, &r, &g, &b);
    RGB[j + 0] = r;
    RGB[j + 1] = g;
    RGB[j + 2] = b;
    YUV2RGB(y1, u, v, &r, &g, &b);
    RGB[j + 3] = r;
    RGB[j + 4] = g;
    RGB[j + 5] = b;
  }
}

static void mono102mono8(char *RAW, char *MONO, int NumPixels)
{
  int i, j;
  for (i = 0, j = 0; i < (NumPixels << 1); i += 2, j += 1)
  {
    //first byte is low byte, second byte is high byte; smash together and convert to 8-bit
    MONO[j] = (unsigned char)(((RAW[i + 0] >> 2) & 0x3F) | ((RAW[i + 1] << 6) & 0xC0));
  }
}

static void yuyv2rgb(char *YUV, char *RGB, int NumPixels)
{
  int i, j;
  unsigned char y0, y1, u, v;
  unsigned char r, g, b;

  for (i = 0, j = 0; i < (NumPixels << 1); i += 4, j += 6)
  {
    y0 = (unsigned char)YUV[i + 0];
    u = (unsigned char)YUV[i + 1];
    y1 = (unsigned char)YUV[i + 2];
    v = (unsigned char)YUV[i + 3];
    YUV2RGB(y0, u, v, &r, &g, &b);
    RGB[j + 0] = r;
    RGB[j + 1] = g;
    RGB[j + 2] = b;
    YUV2RGB(y1, u, v, &r, &g, &b);
    RGB[j + 3] = r;
    RGB[j + 4] = g;
    RGB[j + 5] = b;
  }
}

void rgb242rgb(char *YUV, char *RGB, int NumPixels)
{
  memcpy(RGB, YUV, NumPixels * 3);
}


UsbCam::UsbCam()
  : io_(IO_METHOD_MMAP), fd_(-1), buffers_(NULL), n_buffers_(0), avframe_camera_(NULL),
    avframe_rgb_(NULL), avcodec_(NULL), avoptions_(NULL), avcodec_context_(NULL),
    avframe_camera_size_(0), avframe_rgb_size_(0), video_sws_(NULL), image_(NULL), is_capturing_(false) {
}
UsbCam::~UsbCam()
{
  shutdown();
}

int UsbCam::init_mjpeg_decoder(int image_width, int image_height)
{
  avcodec_register_all();

  avcodec_ = avcodec_find_decoder(AV_CODEC_ID_MJPEG);
  if (!avcodec_)
  {
    ROS_ERROR("Could not find MJPEG decoder");
    return 0;
  }

  avcodec_context_ = avcodec_alloc_context3(avcodec_);
#if LIBAVCODEC_VERSION_MAJOR < 55
  avframe_camera_ = avcodec_alloc_frame();
  avframe_rgb_ = avcodec_alloc_frame();
#else
  avframe_camera_ = av_frame_alloc();
  avframe_rgb_ = av_frame_alloc();
#endif

  avpicture_alloc((AVPicture *)avframe_rgb_, AV_PIX_FMT_RGB24, image_width, image_height);

  avcodec_context_->codec_id = AV_CODEC_ID_MJPEG;
  avcodec_context_->width = image_width;
  avcodec_context_->height = image_height;

#if LIBAVCODEC_VERSION_MAJOR > 52
  avcodec_context_->pix_fmt = AV_PIX_FMT_YUV422P;
  avcodec_context_->codec_type = AVMEDIA_TYPE_VIDEO;
#endif

  avframe_camera_size_ = avpicture_get_size(AV_PIX_FMT_YUV422P, image_width, image_height);
  avframe_rgb_size_ = avpicture_get_size(AV_PIX_FMT_RGB24, image_width, image_height);

  /* open it */
  if (avcodec_open2(avcodec_context_, avcodec_, &avoptions_) < 0)
  {
    ROS_ERROR("Could not open MJPEG Decoder");
    return 0;
  }
  return 1;
}

void UsbCam::mjpeg2rgb(char *MJPEG, int len, char *RGB, int NumPixels)
{
  int got_picture;

  memset(RGB, 0, avframe_rgb_size_);

#if LIBAVCODEC_VERSION_MAJOR > 52
  int decoded_len;
  AVPacket avpkt;
  av_init_packet(&avpkt);

  avpkt.size = len;
  avpkt.data = (unsigned char*)MJPEG;
  decoded_len = avcodec_decode_video2(avcodec_context_, avframe_camera_, &got_picture, &avpkt);

  if (decoded_len < 0)
  {
    ROS_ERROR("Error while decoding frame.");
    return;
  }
#else
  avcodec_decode_video(avcodec_context_, avframe_camera_, &got_picture, (uint8_t *) MJPEG, len);
#endif

  if (!got_picture)
  {
    ROS_ERROR("Webcam: expected picture but didn't get it...");
    return;
  }

  int xsize = avcodec_context_->width;
  int ysize = avcodec_context_->height;
  int pic_size = avpicture_get_size(avcodec_context_->pix_fmt, xsize, ysize);
  if (pic_size != avframe_camera_size_)
  {
    ROS_ERROR("outbuf size mismatch.  pic_size: %d bufsize: %d", pic_size, avframe_camera_size_);
    return;
  }

  video_sws_ = sws_getContext(xsize, ysize, avcodec_context_->pix_fmt, xsize, ysize, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL,
			      NULL,  NULL);
  sws_scale(video_sws_, avframe_camera_->data, avframe_camera_->linesize, 0, ysize, avframe_rgb_->data,
            avframe_rgb_->linesize);
  sws_freeContext(video_sws_);

  int size = avpicture_layout((AVPicture *)avframe_rgb_, AV_PIX_FMT_RGB24, xsize, ysize, (uint8_t *)RGB, avframe_rgb_size_);
  if (size != avframe_rgb_size_)
  {
    ROS_ERROR("webcam: avpicture_layout error: %d", size);
    return;
  }
}

void UsbCam::process_image(const void * src, int len, camera_image_t *dest)
{
  if (pixelformat_ == V4L2_PIX_FMT_YUYV)
  {
    if (monochrome_)
    { //actually format V4L2_PIX_FMT_Y16, but xioctl gets unhappy if you don't use the advertised type (yuyv)
      mono102mono8((char*)src, dest->image, dest->width * dest->height);
    }
    else
    {
      yuyv2rgb((char*)src, dest->image, dest->width * dest->height);
    }
  }
  else if (pixelformat_ == V4L2_PIX_FMT_UYVY)
    uyvy2rgb((char*)src, dest->image, dest->width * dest->height);