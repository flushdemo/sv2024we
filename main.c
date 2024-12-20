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
#define MUSIC_BUFFER_SIZE 42500

#define PALETTE_ADDR ((short *)(0xFF8240))

long (*soundtrack_vbl) (); // Soundtrack VBL function

// Needed global for now
char text_buffer[TEXT_BUFFER_SIZE];

// Video buffers
static unsigned short backsnow_buffer[SCREEN_SIZE];
#ifdef USE_DOUBLE_BUFFER
static unsigned short video_buffer[SCREEN_SIZE + 128]; // 256 bytes alignment
#endif

// Clock reference
static unsigned short clk_0 = 0;

// Returns the frames count - Needs to be executed by supervisor
static long sup_get_clock(void) { return *((long *)REG_FRCLOCK); }
static unsigned short get_clock(void) { return (Supexec(sup_get_clock) & 0xffff) - clk_0; }
static void init_clock(void) { clk_0 = get_clock(); }

// Gnome drawing arguments made global, cause the sprite is drawn in
// the context of an interruption.
unsigned short *gnome_video_ptr;
unsigned short *gnome_background_ptr;

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

    update_printer(text_buffer, clk);
    update_text(video_ptr, backsnow_ptr, asset_fonte, text_buffer, clk);
    update_snow(video_ptr, backsnow_ptr, background_ptr, clk);
    // Gnome sprite is updated during Timer A interrupt

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

void init_gnome(unsigned short* video_ptr, unsigned short* background_ptr) {
  // Gnome context
  gnome_video_ptr = video_ptr;
  gnome_background_ptr = background_ptr;
  init_sprite();
}

int main() {
  unsigned short* video_ptr = Physbase();
  unsigned short* vback_ptr;
#ifdef USE_DOUBLE_BUFFER
  vback_ptr = (unsigned short *)((long)(video_buffer + 128) & (long)0xffffff00);
#endif

  // Set soundtrack pointers
  long* sndh_ptr = (long*) asset_musique;
  long (*soundtrack_init) () = (long(*)()) &(sndh_ptr[0]);
  long (*soundtrack_deinit) () = (long(*)()) &(sndh_ptr[1]);
  soundtrack_vbl = (long(*)()) &(sndh_ptr[2]);

  // Cleanup screen and disable key strokes
  hide_mouse();
  Supexec((void *)disable_conterm);

  // Display background
  Setpalette((void*)asset_palette);
  display_picture(video_ptr, asset_fond);

  // initialize stuff
  display_picture(backsnow_buffer, asset_fond);
#ifdef USE_DOUBLE_BUFFER
  display_picture(vback_ptr, asset_fond);
#endif

  init_font_mask(asset_fonte);
  init_snow();
  init_gnome(video_ptr, backsnow_buffer);

  // Start music and initialize clock
  Supexec(soundtrack_init);
  Supexec(set_sprite_timer);
  Supexec(set_custom_vbl);
  init_clock();

  // Then main loop
  main_loop(video_ptr, vback_ptr, backsnow_buffer,
            asset_fond, asset_fonte, text_buffer);

  Supexec(restore_vbl);
  Supexec(soundtrack_deinit);
  Supexec((void *)enable_conterm);
  show_mouse();
}
