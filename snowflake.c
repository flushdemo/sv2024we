#include <tos.h>
#include "common.h"
#include "snowflake.h"

struct snow_flake {
  unsigned short variant;
  unsigned short x_block; // in 0 - 19
  unsigned short x_shift; // left shift in the 16bits block in [0 - 8]
  unsigned short y_vel; // y velocity
  unsigned short y_pos; // y position (top of picture)
  unsigned short old_y_pos; // currently displayed position
  short y_velcnt; // counter to implement mouvement
};

// 8 lines
// 4 bit planes
// 1 word per bit plane
// 8 bits LSB used
static unsigned short snow_flake_pic[] = {
  0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x001c, 0x001c, 
  0x0000, 0x0000, 0x003e, 0x003e, 
  0x0000, 0x0000, 0x003e, 0x003e, 
  0x0000, 0x0000, 0x003e, 0x003e, 
  0x0000, 0x0000, 0x001c, 0x001c, 
};

static unsigned short snow_flake_mask[] = {
  0x0000,
  0x001c,
  0x003e,
  0x003e,
  0x003e,
  0x001c,
};

static unsigned short background_mask[] = {
  0x003e,
  0x0022,
  0x0000,
  0x0000,
  0x0000,
  0x0000,
};

static unsigned short foreground_mask[] = {
  0xffc1,
  0xffc1,
  0xffc1,
  0xffc1,
  0xffc1,
  0xffe3,
};

// Flakes variants storage
static unsigned short flake_pic_variants
[SNOW_FLAKE_VARIANTS][BIT_PLANES * SNOW_FLAKE_HEIGHT];
static unsigned short sf_mask_variants
[SNOW_FLAKE_VARIANTS][SNOW_FLAKE_HEIGHT];
static unsigned short bg_mask_variants
[SNOW_FLAKE_VARIANTS][SNOW_FLAKE_HEIGHT];
static unsigned short fg_mask_variants
[SNOW_FLAKE_VARIANTS][SNOW_FLAKE_HEIGHT];

static struct snow_flake snow[MAX_SNOW_FLAKES];
static unsigned short snow_count;

static void reset_snow_flake(struct snow_flake *flake) {
  flake->variant = Random() % SNOW_FLAKE_VARIANTS;
  flake->x_block = ((Random() % 18) + 4) % 20; // flakes in columns 0, 1, 4, 5 -- 19.
  // Avoid columns 2, 3, center of gnome - too much clipping
  flake->x_shift = Random() % 8;
  flake->y_vel = (Random() % (MAX_SNOW_VELOCITY-MIN_SNOW_VELOCITY)) + MIN_SNOW_VELOCITY;
  flake->y_pos = 0;
  flake->old_y_pos = 0;
  flake->y_velcnt = 0;
}

static void init_snow_flake(struct snow_flake *flake) {
  reset_snow_flake(flake);
  flake->y_pos = Random() % MAX_SNOW_Y;
}

void compute_flake_variants(void) {
  for (unsigned short v=0; v<SNOW_FLAKE_VARIANTS; v++) {
    for (unsigned short h=0; h<SNOW_FLAKE_HEIGHT; h++) {
      sf_mask_variants[v][h] = snow_flake_mask[h] << (v * (8 / (SNOW_FLAKE_VARIANTS-1)));
      bg_mask_variants[v][h] = background_mask[h] << (v * (8 / (SNOW_FLAKE_VARIANTS-1)));
      fg_mask_variants[v][h] = (foreground_mask[h] << (v * (8 / (SNOW_FLAKE_VARIANTS-1))))
        | ((1 << (v * (8 / (SNOW_FLAKE_VARIANTS-1)))) - 1); // fill the right with ones
      for (unsigned short b=0; b<BIT_PLANES; b++) {
        flake_pic_variants[v][h*BIT_PLANES+b] = snow_flake_pic[h*BIT_PLANES+b] << (v * (8 / (SNOW_FLAKE_VARIANTS-1)));
      }
    }
  }
}

void init_snow(void) {
  snow_count = MAX_SNOW_FLAKES;
  compute_flake_variants();
  for (unsigned short i=0; i < MAX_SNOW_FLAKES; i++) {
    init_snow_flake(&snow[i]);
  }
}

static void update_snow_flake(struct snow_flake *flake, unsigned short delta) {
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
                               unsigned short* backsnow_ptr,
                               unsigned short* background_ptr,
                               struct snow_flake *flake) {
  if (flake->y_pos != flake->old_y_pos) {
    unsigned short displacement = LINE_WIDTH*flake->y_pos + BIT_PLANES*flake->x_block;
    unsigned short var = flake->variant;
    video_ptr += displacement;
    backsnow_ptr += displacement;
    background_ptr += displacement;
    for (unsigned short i=0; i < SNOW_FLAKE_HEIGHT; i++) {
      unsigned short sm = sf_mask_variants[var][i];
      unsigned short bm = bg_mask_variants[var][i];
      unsigned short fm = fg_mask_variants[var][i];
      for (unsigned short j=0; j < BIT_PLANES; j++) {
        if (flake->y_pos > (MAX_SNOW_Y - 1)) {
          // Clean snow flake
          video_ptr[j] = background_ptr[j];
          backsnow_ptr[j] = background_ptr[j];
        }
        else { // Draw snow flake
          unsigned short sp = flake_pic_variants[var][BIT_PLANES*i + j];
          unsigned short bg = background_ptr[j];
          unsigned short fg = video_ptr[j];
          video_ptr[j] = (sp & sm) | (bg & bm) | (fg & fm);
          backsnow_ptr[j] = (sp & sm) | (bg & ~sm);
        }
      }
      video_ptr += LINE_WIDTH;
      backsnow_ptr += LINE_WIDTH;
      background_ptr += LINE_WIDTH;
    }
    flake->old_y_pos = flake->y_pos;
  }
}

unsigned short update_snow(unsigned short* video_ptr,
                           unsigned short* backsnow_ptr,
                           unsigned short* background_ptr,
                           unsigned short clk) {
  static unsigned short old_clk = 0;
  if (old_clk == 0) old_clk = clk; // initialization fix
  for (unsigned short i=0; i < snow_count; i++) {
    update_snow_flake(&snow[i], (clk - old_clk));
    display_snow_flake(video_ptr, backsnow_ptr, background_ptr, &snow[i]);
  }
  old_clk = clk;
  return snow_count;
}
