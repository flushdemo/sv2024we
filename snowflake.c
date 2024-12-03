#include <tos.h>
#include "common.h"
#include "snowflake.h"

#define FLAKE_SINE_MASK 0x1f
#define FLAKE_COUNT 4
#define FLAKE_ASSET_LINE_WIDTH (BIT_PLANES * FLAKE_COUNT)
#define FLAKE_ASSET_HEIGHT 8
#define SNOW_FLAKE_HEADER (SNOW_FLAKE_HEIGHT - FLAKE_ASSET_HEIGHT)

struct snow_flake {
  unsigned short x_block; // in 0 - 19
  unsigned short x_shift; // left shift in the 16bits block in [0 - 8]
  unsigned short x_vel; // x velocity
  unsigned short y_vel; // y velocity
  unsigned short x_pos; // x position on the sine
  unsigned short y_pos; // y position (top of picture)
  unsigned short old_x_pos; // currently displayed position
  unsigned short old_y_pos; // currently displayed position
  short x_velcnt;
  short y_velcnt; // counter to implement mouvement
};

// [round(4*math.sin(x/32 * 2*math.pi))+4 for x in range(32)]
static unsigned short flake_sine[] = {
4, 5, 6, 6, 7, 7, 8, 8, 8, 8, 8, 7, 7, 6, 6, 5, 4, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3
};

// base masks for snow flakes
static unsigned long base_mask[] = {
  0x01ff01ff,
  0x03ff03ff,
  0x07fe07fe,
  0x0ffc0ffc,
  0x1ff81ff8,
  0x3ff03ff0,
  0x7fe07fe0,
  0xffc0ffc0,
  0xff80ff80
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
};

static unsigned short snow_flake_mask[] = {
  0x0000,
  0x0000,
  0x001c,
  0x003e,
  0x003e,
  0x003e,
  0x001c,
};

static unsigned short background_mask[] = {
  0x003e,
  0x003e,
  0x0063,
  0x0041,
  0x0041,
  0x0041,
  0x0022,
};

static unsigned short foreground_mask[] = {
  0xffc1,
  0xffc1,
  0xff80,
  0xff80,
  0xff80,
  0xff80,
  0xffc1,
};

// Flakes variants storage
static unsigned short flake_pic_variants
[SNOW_FLAKE_VARIANTS][BIT_PLANES * SNOW_FLAKE_HEIGHT];
static unsigned long sf_mask_variants
[SNOW_FLAKE_VARIANTS][SNOW_FLAKE_HEIGHT];
static unsigned long bg_mask_variants
[SNOW_FLAKE_VARIANTS][SNOW_FLAKE_HEIGHT];
static unsigned long fg_mask_variants
[SNOW_FLAKE_VARIANTS][SNOW_FLAKE_HEIGHT];

struct snow_flake snow[MAX_SNOW_FLAKES];
static unsigned short snow_count;

unsigned short random_block() {
  unsigned short block;
#ifdef NO_SNOW_ON_GNOME
  // Avoid columns 1-4, center of gnome - too much clipping
  // flakes in columns 0, 5-19.
  block = ((Random() % 16) + 5) % SNOW_MAX_X_BLOCK;
#else
  // flakes everywhere
  block = Random() % SNOW_MAX_X_BLOCK;
#endif
  return block;
}

unsigned short unused_random_block() {
  unsigned short block;
  unsigned short used = 1;
  while (used) {
    block = random_block();
    used = 0;
    for (unsigned int i=0; i<snow_count; i++) {
      if (snow[i].x_block == block) {
        used = 1;
        break;
      }
    }
  }
  return block;
}

void reset_snow_flake(struct snow_flake *flake) {
  flake->x_pos = Random() % SNOW_FLAKE_VARIANTS;
  flake->x_block = unused_random_block();
  flake->x_shift = Random() % 8;
  flake->x_vel = (Random() % (MAX_SNOW_X_VELOCITY-MIN_SNOW_X_VELOCITY)) + MIN_SNOW_X_VELOCITY;
  flake->y_vel = (Random() % (MAX_SNOW_Y_VELOCITY-MIN_SNOW_Y_VELOCITY)) + MIN_SNOW_Y_VELOCITY;
  flake->y_pos = 0;
  flake->old_x_pos = 0;
  flake->old_y_pos = 0;
  flake->x_velcnt = 0;
  flake->y_velcnt = 0;
}

void init_snow_flake(struct snow_flake *flake) {
  reset_snow_flake(flake);
  flake->y_pos = Random() % MAX_SNOW_Y;
}

unsigned long build_sprite_mask(unsigned short* sprite) {
  unsigned short mask = 0;
  for (unsigned short i=0; i<BIT_PLANES; i++) {
    mask |= *(sprite+i);
  }
  return mask<<16 | mask;
}

void compute_flake_variants(void) {
  unsigned short step = 8 / (SNOW_FLAKE_VARIANTS - 1);
  for (unsigned short v=0; v<SNOW_FLAKE_VARIANTS; v++) {
    for (unsigned short h=0; h<SNOW_FLAKE_HEADER; h++) {
      for (unsigned short b=0; b<BIT_PLANES; b++) {
        flake_pic_variants[v][h*BIT_PLANES+b] = 0;
      }
      bg_mask_variants[v][h] = base_mask[v];
      fg_mask_variants[v][h] = ~base_mask[v];
    }
    for (unsigned short h=0; h<FLAKE_ASSET_HEIGHT; h++) {
      unsigned short sfh = SNOW_FLAKE_HEADER + h;
      for (unsigned short b=0; b<BIT_PLANES; b++) { // Build flake_pic_variants
        flake_pic_variants[v][sfh*BIT_PLANES+b] =
          asset_flocons[h*FLAKE_ASSET_LINE_WIDTH+b] <<
          (v * (8 / (SNOW_FLAKE_VARIANTS-1)));
      }
      unsigned long sprite_mask = build_sprite_mask( &flake_pic_variants[v][sfh*BIT_PLANES] );
      bg_mask_variants[v][sfh] = ~sprite_mask & base_mask[v];
      fg_mask_variants[v][sfh] = ~base_mask[v];
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

void update_snow_flake(struct snow_flake *flake, unsigned short delta) {
  flake->x_velcnt -= delta;
  flake->y_velcnt -= delta;
  while (flake->y_velcnt < 0) {
    flake->y_velcnt += flake->y_vel;
    flake->y_pos++;
  }
  while (flake->x_velcnt < 0) {
    flake->x_velcnt += flake->x_vel;
    flake->x_pos++;
  }
  if (flake->y_pos > MAX_SNOW_Y) {
    reset_snow_flake(flake);
  }
}

int flake_in_text(struct snow_flake *flake) {
  unsigned short fl_blk = flake->x_block;
  unsigned short fl_y = flake->y_pos;
  if ( (flake->x_block >= TEXT_STARTING_BLOCK) &&
       (flake->y_pos >= (TEXT_MIN_Y - SNOW_FLAKE_HEIGHT)) &&
       (flake->y_pos <= TEXT_MAX_Y) ){ // Are we in the text zone ?
    for (unsigned short i=0; text_buffer[i] != '\0'; i++) {
      char chr = text_buffer[i];
      if ((chr != '\n') && (chr != '#') && (chr != ' ')) {
        if ((fl_blk == char_block[i]) &&
            (fl_y > char_top_y[i]) &&
            (fl_y < (char_top_y[i] + CHAR_HEADER_HEIGHT + FONT_HEIGHT))) {
          return 1;
        }
      }
    }
  }
  return 0; // flake not in text
}

void clear_flake_sprite(unsigned short* video_ptr,
                        unsigned short* backsnow_ptr,
                        unsigned short* background_ptr) {
  for (unsigned short i=0; i < SNOW_FLAKE_HEIGHT; i++) {
    for (unsigned short j=0; j < BIT_PLANES; j++) {
      video_ptr[j] = background_ptr[j];
      backsnow_ptr[j] = background_ptr[j];
    }
    video_ptr += LINE_WIDTH;
    backsnow_ptr += LINE_WIDTH;
    background_ptr += LINE_WIDTH;
  }
}

void display_snow_flake(unsigned short* video_ptr,
                        unsigned short* backsnow_ptr,
                        unsigned short* background_ptr,
                        struct snow_flake *flake) {
  if ( flake->y_pos != flake->old_y_pos ||
       flake->x_pos != flake->old_x_pos ){ // Did the flake move ?

    // Position pointers at flake location
    unsigned short displacement = LINE_WIDTH*flake->y_pos + BIT_PLANES*flake->x_block;
    video_ptr += displacement;
    backsnow_ptr += displacement;
    background_ptr += displacement;

    if (flake->y_pos > (MAX_SNOW_Y - SNOW_CLEAR_ZONE)) { // Too low ?
      clear_flake_sprite(video_ptr, backsnow_ptr, background_ptr);
    }
    else {
      // We have to update backsnow (To fix later?)
      unsigned short var = flake_sine[flake->x_pos & FLAKE_SINE_MASK];
      unsigned short update_video_ram = 1; // By default we update the vide RAM

#ifdef FLICKER_REDUCTION
      if ( flake_in_text(flake) ) { // Are we in behind a character ?
        update_video_ram = 0; // Don't update video RAM
      }
#endif

      display_flake_sprite_opt(video_ptr,
                               backsnow_ptr,
                               background_ptr,
                               flake_pic_variants[var],
                               sf_mask_variants[var],
                               bg_mask_variants[var],
                               fg_mask_variants[var],
                               update_video_ram);

      flake->old_x_pos = flake->x_pos;
      flake->old_y_pos = flake->y_pos;
    }
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
