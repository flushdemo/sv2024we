#ifndef __SV2024_SNOWFLAKE_H
#define __SV2024_SNOWFLAKE_H 1

#include "common.h"

#define SNOW_FLAKE_VARIANTS 9
#define SNOW_FLAKE_HEIGHT 6
#define MAX_SNOW_Y (SCREEN_LINES - SNOW_FLAKE_HEIGHT)
#define MIN_SNOW_Y_VELOCITY 3
#define MAX_SNOW_Y_VELOCITY 6
#define MIN_SNOW_X_VELOCITY 12
#define MAX_SNOW_X_VELOCITY 36

#define SNOW_MIN_X_BLOCK 0 // GNOME_WIDTH // Included
#define SNOW_MAX_X_BLOCK 20 // Excluded

void init_snow(void);
unsigned short update_snow(unsigned short* video_ptr,
                           unsigned short* backsnow_ptr,
                           unsigned short* background_ptr,
                           unsigned short clk);

#endif
