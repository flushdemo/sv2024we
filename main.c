#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tos.h>

#include "common.h"
#include "misc.h"
#include "printer.h"
#include "sprite.h"
#include "text.h"
#include "vbl.h"

#define SHOW_FPS 1

// Atari specifics
#define VBL_VECTOR 28 // 0x0070 >> 2 // VBL Vector
#define REG_FRCLOCK 0x466 // clock register

// Data sizes
#define DEGAS_FILE_SIZE 32034 // Degas picture format in bytes
// FONT_SIZE is in 16bit blocks, while FONT_FILE_SIZE is in bytes.
#define FONT_FILE_SIZE (34 + FONT_SIZE*2) // 34 for Degas header
#define TEXT_BUFFER_SIZE 256
#define MUSIC_BUFFER_SIZE 32768

// Text zone constants
#define TEXT_X 160 - (13*4) // 13 characters - right aligned
#define TEXT_Y (200 - (16*5)) / 2 // 5 lines centered

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
char text_buffer[TEXT_BUFFER_SIZE]; // Temporarily public for debugging

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
  if (clk != old_clk) update_sprite(video_ptr, background_ptr, clk);
  old_clk = clk;
}

static void main_loop(unsigned short *video_ptr,
                      unsigned short *background_ptr,
                      unsigned short *font_ptr,
                      char* text_buffer) {
  unsigned short *text_vid_ptr = video_ptr + TEXT_Y*LINE_WIDTH + TEXT_X;
  unsigned short *text_bg_ptr = background_ptr + TEXT_Y*LINE_WIDTH + TEXT_X;
  #ifdef SHOW_FPS
  unsigned short frames_cnt = 0;
  unsigned short old_clk = get_clock();
  #endif

  for (unsigned short i;; i++) {
    unsigned short clk = get_clock();
    unsigned short txt_i;

    update_printer(text_buffer, clk);
    update_text(text_vid_ptr, text_bg_ptr, font.picture, text_buffer, clk);
    update_sprite_proxy(video_ptr, background_ptr, clk);

    #ifdef SHOW_FPS
    frames_cnt += clk - old_clk;
    if ( !(i & 0x3f) ) {
      unsigned short fps_100 = 100*50*64 / frames_cnt; // FPS
      printf("\x1bH%d.%02d FPS \n", fps_100/100, fps_100%100);
      frames_cnt = 0;
    }
    old_clk = clk;
    #endif
  }
}

int main() {
  unsigned short* video_ptr = Physbase();

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

  init_font_mask(font.picture);
  main_loop(video_ptr, background.picture, font.picture, text_buffer);

  Supexec(restore_vbl);
  Supexec(soundtrack_deinit);
  show_mouse();
}
