#ifndef __SV2024_SNOWFLAKE_H
#define __SV2024_SNOWFLAKE_H 1

#include "common.h"

#define SNOW_FLAKE_HEIGHT 8
#define MAX_SNOW_FLAKES 10
#define MAX_SNOW_Y (SCREEN_LINES - SNOW_FLAKE_HEIGHT)
#define MIN_SNOW_VELOCITY 4
#define MAX_SNOW_VELOCITY 12

void init_snow(void);
unsigned short update_snow(unsigned short* video_ptr,
                           unsigned short* background_ptr,
                           unsigned short clk);

#endif
