#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tos.h>

#include "common.h"
#include "vbl.h"
#include "text.h"
#include "sprite.h"

// Atari specifics
#define VBL_VECTOR 28 // 0x0070 >> 2 // VBL Vector
#define REG_FRCLOCK 0x466 // clock register

#define DEGAS_FILE_SIZE 32034 // Degas picture format in bytes
#define TEXT_BUFFER_SIZE 256
#define MUSIC_BUFFER_SIZE 32768

// Text zone constants
#define TEXT_X 160 - (14*4) // 14 characters - right aligned
#define TEXT_Y (200 - (16*5)) / 2 // 5 lines centered

struct degas {
  unsigned short resolution;
  unsigned short palette[16];
  unsigned short picture[SCREEN_SIZE];
};

// To be passed as parameter
long (*soundtrack_vbl) (); // Soundtrack VBL function

// Main data areas
static struct degas font;
static struct degas background;
static char music_buffer[MUSIC_BUFFER_SIZE];
static char text_buffer[TEXT_BUFFER_SIZE];

// Returns the frames count - Needs to be executed by supervisor
static long sup_get_clock(void) { return *((long *)REG_FRCLOCK); }
static long get_clock(void) { return Supexec(sup_get_clock); }

static size_t load_file(char* target, char* filename, size_t length) {
  FILE* fd = fopen(filename, "rb");
  size_t cnt = fread(target, 1, length, fd);
  fclose(fd);
  return cnt;
}

static void load_degas_or_quit(struct degas *target, char* filename) {
  if ( load_file((char*)target, filename, DEGAS_FILE_SIZE) != DEGAS_FILE_SIZE ) {
    printf("Error while loading \"%s\"\n", filename);
    Cnecin();
    exit(1);
  }
}

void main_loop(unsigned short *video_ptr,
               unsigned short *background_ptr,
               unsigned short *font_ptr,
               char* text_buffer,
               char* fps_buffer) {
  unsigned short *text_vid_ptr = video_ptr + TEXT_Y*LINE_WIDTH + TEXT_X;
  unsigned short *text_bg_ptr = background_ptr + TEXT_Y*LINE_WIDTH + TEXT_X;
  unsigned short frames_cnt = 0;
  unsigned short old_clk = get_clock();

  for (unsigned short i;; i++) {
    unsigned short clk = get_clock();
    unsigned short txt_i;

    frames_cnt += clk - old_clk;
    update_text(text_vid_ptr, text_bg_ptr, font.picture, text_buffer, clk);
    update_sprite(video_ptr, background_ptr, clk);

    if ( !(i & 0x0f) ) {
      unsigned short fps_100 = 100*50*16 / frames_cnt; // FPS
      sprintf(fps_buffer, "\n%d.%02d FPS \n", fps_100/100, fps_100%100);
      frames_cnt = 0;
    }
    old_clk = clk;
  }
}

int main() {
  unsigned short* video_ptr = Physbase();
  char* text =" HOWDY FOLKS\n\nWE ARE BACK @\n\nSILLY VENTURE\n";
  unsigned short i;

  // Set soundtrack pointers
  long* sndh_ptr = (long*) music_buffer;
  long (*soundtrack_init) () = (long(*)()) &(sndh_ptr[0]);
  long (*soundtrack_deinit) () = (long(*)()) &(sndh_ptr[1]);
  soundtrack_vbl = (long(*)()) &(sndh_ptr[2]);

  load_degas_or_quit(&font, "FONTE.PI1");
  load_degas_or_quit(&background, "FOND.PI1");
  load_file(music_buffer, "MUSIC.SND", MUSIC_BUFFER_SIZE);

  Supexec(soundtrack_init);
  Supexec(set_music_player_vbl);

  Setpalette((void*)&(background.palette));
  display_picture(video_ptr, background.picture);
  for (i=0; text[i] != '\0'; i++) text_buffer[i] = text[i];

  init_font_mask(font.picture);
  main_loop(video_ptr, background.picture, font.picture,
            text_buffer, text_buffer + i);

  Supexec(restore_vbl);
  Supexec(soundtrack_deinit);
}
