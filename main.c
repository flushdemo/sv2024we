#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tos.h>

#include "common.h"
#include "misc.h"
#include "printer.h"
#include "snowflake.h"
#include "sprite.h"
#include "text.h"
#include "vbl.h"

// Number of frames to average for FPS
#define FPS_FRAMES_AVERAGE 32

// Atari specifics
#define VBL_VECTOR 28 // 0x0070 >> 2 // VBL Vector
#define REG_FRCLOCK 0x466 // clock register
#define REG_SCREENPT 0x045e // Screen pointer
// https://freemint.github.io/tos.hyp/en/bios_sysvars.html

// Data sizes
#define DEGAS_FILE_SIZE 32034 // Degas picture format in bytes
// FONT_SIZE is in 16bit blocks, while FONT_FILE_SIZE is in bytes.
#define FONT_FILE_SIZE (34 + FONT_SIZE*2) // 34 for Degas header
#define MUSIC_BUFFER_SIZE 32768

// Misc
#define GNOME_SPEED 5

struct degas_pic {
  unsigned short resolution;
  unsigned short palette[16];
  unsigned short picture[SCREEN_SIZE];
};

struct degas_font {
  unsigned short resolution;
  unsigned short palette[16];
  unsigned short picture[FONT_SIZE];
};

long (*soundtrack_vbl) (); // Soundtrack VBL function

// Main data areas
static struct degas_pic background;
static struct degas_font font;
static char music_buffer[MUSIC_BUFFER_SIZE];

// Needed global for now
char text_buffer[TEXT_BUFFER_SIZE];

// Video ram buffer for double buffering
static unsigned short backsnow_buffer[SCREEN_SIZE];
#ifdef USE_DOUBLE_BUFFER
static unsigned short video_buffer[SCREEN_SIZE + 128]; // 256 bytes alignment
#endif

// Returns the frames count - Needs to be executed by supervisor
static long sup_get_clock(void) { return *((long *)REG_FRCLOCK); }
static long get_clock(void) { return Supexec(sup_get_clock); }

static size_t load_file(char* target, char* filename, size_t length) {
  FILE* fd = fopen(filename, "rb");
  size_t cnt = fread(target, 1, length, fd);
  fclose(fd);
  return cnt;
}

static void load_picture_or_quit(struct degas_pic *target, char* filename) {
  if ( load_file((char*)target, filename, DEGAS_FILE_SIZE) != DEGAS_FILE_SIZE ) {
    printf("Error while loading picture \"%s\"\n", filename);
    Cnecin();
    exit(1);
  }
}

static void load_font_or_quit(struct degas_font *target, char* filename) {
  if ( load_file((char*)target, filename, FONT_FILE_SIZE) != FONT_FILE_SIZE ) {
    printf("Error while loading font \"%s\"\n", filename);
    Cnecin();
    exit(1);
  }
}

static void update_sprite_proxy(unsigned short *video_ptr,
                                unsigned short *background_ptr,
                                unsigned clk) {
  static unsigned short old_clk = 0;
  clk = clk / GNOME_SPEED;
#ifdef GNOME_SMART_DRAWING
  if (clk != old_clk)
#endif
    update_sprite(video_ptr, background_ptr, clk);
  old_clk = clk;
}

static void main_loop(unsigned short *video_ptr,
                      unsigned short *vback_ptr, // double buffer
                      unsigned short *backsnow_ptr,
                      unsigned short *background_ptr,
                      unsigned short *font_ptr,
                      char* text_buffer) {
#ifdef SHOW_FPS
  unsigned short frames_cnt = 0;
  unsigned short old_clk = get_clock();
#endif

  for (unsigned short i;; i++) {
    unsigned short clk = get_clock();
    unsigned short *tmp_ptr;

#ifdef USE_DOUBLE_BUFFER
    // Display buffer pointed at by video_ptr
    Setscreen((unsigned short *)-1, video_ptr, -1);
    Vsync();

    // Swap video ram and back buffer
    tmp_ptr = vback_ptr;
    vback_ptr = video_ptr;
    video_ptr = tmp_ptr;
#endif

    update_snow(video_ptr, backsnow_ptr, background_ptr, clk);
    update_printer(text_buffer, clk);
    update_text(video_ptr, backsnow_ptr, font.picture, text_buffer, clk);
    update_sprite_proxy(video_ptr, backsnow_ptr, clk);

    #ifdef SHOW_FPS
    frames_cnt += clk - old_clk;
    if ( !(i & (FPS_FRAMES_AVERAGE-1)) ) {
      unsigned short fps_100 = 100*50*FPS_FRAMES_AVERAGE / frames_cnt; // FPS
      printf("\x1bH%d.%02d FPS \n", fps_100/100, fps_100%100);
      frames_cnt = 0;
    }
    old_clk = clk;
    #endif

  }
}

int main() {
  unsigned short* video_ptr = Physbase();
  unsigned short* vback_ptr;
#ifdef USE_DOUBLE_BUFFER
  vback_ptr = (unsigned short *)((long)(video_buffer + 128) & (long)0xffffff00);
#endif

  // Set soundtrack pointers
  long* sndh_ptr = (long*) music_buffer;
  long (*soundtrack_init) () = (long(*)()) &(sndh_ptr[0]);
  long (*soundtrack_deinit) () = (long(*)()) &(sndh_ptr[1]);
  soundtrack_vbl = (long(*)()) &(sndh_ptr[2]);

  load_picture_or_quit(&background, "FOND.PI1");
  load_font_or_quit(&font, "FONTE.PI1");
  load_file(music_buffer, "MUSIC.SND", MUSIC_BUFFER_SIZE);

  Supexec(soundtrack_init);
  Supexec(set_music_player_vbl);

  hide_mouse();
  Setpalette((void*)&(background.palette));
  display_picture(video_ptr, background.picture);
#ifdef USE_DOUBLE_BUFFER
  display_picture(vback_ptr, background.picture);
#endif
  display_picture(backsnow_buffer, background.picture);

  init_font_mask(font.picture);
  init_snow();
  init_sprite();
  //init_sprite(); // <= FIXME: bug probably odd address.
  main_loop(video_ptr, vback_ptr, backsnow_buffer,
            background.picture, font.picture, text_buffer);

  Supexec(restore_vbl);
  Supexec(soundtrack_deinit);
  show_mouse();
}
