#include <tos.h>
#include "common.h"
#include "snowflake.h"

struct snow_flake {
  unsigned short x_block; // in 0 - 19
  unsigned short x_shift; // left shift in the 16bits block in [0 - 8]
  unsigned short y_pos; // y position (top of picture)
  unsigned short y_vel; // y velocity
  short y_velcnt; // counter to implement mouvement
};

// 8 lines
// 4 bit planes
// 1 word per bit plane
// 8 bits LSB used
static unsigned short snow_flake_pic[] = {
  0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x001c, 0x001c, 
  0x0000, 0x0000, 0x003e, 0x003e, 
  0x0000, 0x0000, 0x003e, 0x003e, 
  0x0000, 0x0000, 0x003e, 0x003e, 
  0x0000, 0x0000, 0x001c, 0x001c, 
  0x0000, 0x0000, 0x0000, 0x0000,
};

static unsigned short snow_flake_mask[] = {
  0x0000,
  0x0000,
  0x0000,
  0x001c,
  0x003e,
  0x003e,
  0x003e,
  0x001c,
};

struct snow_flake snow[MAX_SNOW_FLAKES];
unsigned short snow_count;

static void reset_snow_flake(struct snow_flake *flake) {
  flake->x_block = Random() % 20;
  flake->x_shift = Random() % 8;
  flake->y_vel = (Random() % (MAX_SNOW_VELOCITY-MIN_SNOW_VELOCITY)) + MIN_SNOW_VELOCITY;
  flake->y_velcnt = 0;
  flake->y_pos = 0;
}

static void init_snow_flake(struct snow_flake *flake) {
  reset_snow_flake(flake);
  flake->y_pos = Random() % MAX_SNOW_Y;
}

void init_snow(void) {
  snow_count = MAX_SNOW_FLAKES;
  for (unsigned short i=0; i < MAX_SNOW_FLAKES; i++) {
    init_snow_flake(&snow[i]);
  }
}

static void update_snow_flake(unsigned short* video_ptr,
                              unsigned short* background_ptr,
                              struct snow_flake *flake,
                              unsigned short delta) {
  flake->y_velcnt -= delta;
  while (flake->y_velcnt < 0) {
    flake->y_velcnt += flake->y_vel;
    flake->y_pos++;
  }
  if (flake->y_pos > MAX_SNOW_Y) {
    reset_snow_flake(flake);
  }
}

static void display_snow_flake(unsigned short* video_ptr,
                               unsigned short* background_ptr,
                               const struct snow_flake *flake) {
  unsigned short displacement = LINE_WIDTH*flake->y_pos + BIT_PLANES*flake->x_block;
  video_ptr += displacement;
  background_ptr += displacement;
  for (unsigned short i=0; i < SNOW_FLAKE_HEIGHT; i++) {
    for (unsigned short j=0; j < BIT_PLANES; j++) {
      if (flake->y_pos > (MAX_SNOW_Y - 2)) {
        // Clean snow flake
        video_ptr[j] = background_ptr[j];
      }
      else { // Draw snow flake
        unsigned short m = snow_flake_mask[i];
        video_ptr[j] =
          (snow_flake_pic[BIT_PLANES*i + j] & m)
          | (background_ptr[j] & ~m);
      }
    }
    video_ptr += LINE_WIDTH;
    background_ptr += LINE_WIDTH;
  }
}

unsigned short update_snow(unsigned short* video_ptr,
                           unsigned short* background_ptr,
                           unsigned short clk) {
  static unsigned short old_clk = 0;
  if (old_clk == 0) old_clk = clk; // initialization fix
  for (unsigned short i=0; i < snow_count; i++) {
    update_snow_flake(video_ptr, background_ptr, &snow[i], (clk - old_clk));
    display_snow_flake(video_ptr, background_ptr, &snow[i]);
  }
  old_clk = clk;
  return snow_count;
}
