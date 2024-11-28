#ifndef __SV2024_SNOWFLAKE_H
#define __SV2024_SNOWFLAKE_H 1

#define SNOW_FLAKE_HEIGHT 8
#define MAX_SNOW_FLAKES 10
#define MAX_SNOW_Y (200 - SNOW_FLAKE_HEIGHT)
#define MAX_SNOW_VELOCITY 3

void init_snow(void);
unsigned short update_snow(unsigned short* video_ptr,
                           unsigned short* background_ptr,
                           unsigned short clk);

#endif
