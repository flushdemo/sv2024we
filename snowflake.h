#ifndef __SV2024_SNOWFLAKE_H
#define __SV2024_SNOWFLAKE_H 1

#include "common.h"

#define SNOW_FLAKE_VARIANTS 9
#define SNOW_FLAKE_HEIGHT 7
#define MAX_SNOW_Y (SCREEN_LINES - SNOW_FLAKE_HEIGHT)
#define MIN_SNOW_Y_VELOCITY 2
#define MAX_SNOW_Y_VELOCITY 5
#define MIN_SNOW_X_VELOCITY 12
#define MAX_SNOW_X_VELOCITY 36

#define SNOW_CLEAR_ZONE 4
#define SNOW_MIN_X_BLOCK 0 // GNOME_WIDTH // Included
#define SNOW_MAX_X_BLOCK 20 // Excluded

void init_snow(void);
unsigned short update_snow(unsigned short* video_ptr,
                           unsigned short* backsnow_ptr,
                           unsigned short* background_ptr,
                           unsigned short clk);

// Optimized function do draw the flake sprites
void display_flake_sprite_opt(unsigned short* video_ptr,
                              unsigned short* backsnow_ptr,
                              unsigned short* background_ptr,
                              unsigned short* flake_pic,
                              unsigned short* flake_mask,
                              unsigned short* background_mask,
                              unsigned short* foreground_mask,
                              unsigned short foreground_flake_flag);

#endif
