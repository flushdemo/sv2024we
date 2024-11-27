#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tos.h>

#include "common.h"
#include "text.h"

#define REG_FRCLOCK 0x466 // clock register
#define DEGAS_FILE_SIZE 32034/2 // Degas picture format in 16bits blocs
#define TEXT_BUFFER_SIZE 256

// Text zone constants
#define TEXT_X 160 - (14*4) // 14 characters - right aligned
#define TEXT_Y (200 - (16*5)) / 2 // 5 lines centered

struct degas {
  unsigned short resolution;
  unsigned short palette[16];
  unsigned short picture[SCREEN_SIZE];
};

// Main data areas
static struct degas font;
static struct degas background;
static char text_buffer[TEXT_BUFFER_SIZE];

// Returns the frames count - Needs to be executed by supervisor
static long sup_get_clock(void) { return *((long *)REG_FRCLOCK); }
static long get_clock(void) { return Supexec(sup_get_clock); }

static int load_degas(struct degas *target, char* filename) {
  FILE* fd = fopen(filename, "rb");
  size_t cnt = fread(target, 2, DEGAS_FILE_SIZE, fd);
  fclose(fd);
  return cnt != DEGAS_FILE_SIZE;
}

static void load_degas_or_quit(struct degas *target, char* filename) {
  if ( load_degas(target, filename) != 0 ) {
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
  unsigned short frames_cnt = 0;
  unsigned short old_clk = get_clock();

  for (unsigned short i;; i++) {
    unsigned short clk = get_clock();
    unsigned short txt_i;

    frames_cnt += clk - old_clk;
    update_text(video_ptr, background_ptr, font.picture, text_buffer, clk);
    // update_sprite(video_ptr, clk);

    if ( !(i & 0x0f) ) {
      unsigned short fps_100 = 100*50*16 / frames_cnt; // FPS
      sprintf(fps_buffer, "%d.%02d FPS \n", fps_100/100, fps_100%100);
      frames_cnt = 0;
    }
    old_clk = clk;
  }
}

int main() {
  unsigned short* video_ptr = Physbase();
  char* text =" HOWDY FOLKS\n\nWE ARE BACK @\n\nSILLY VENTURE\n";
  unsigned short i;
  
  load_degas_or_quit(&font, "FONTE.PI1");
  load_degas_or_quit(&background, "FOND.PI1");

  Setpalette((void*)&(font.palette));
  display_picture(video_ptr, background.picture);
  for (i=0; text[i] != '\0'; i++) text_buffer[i] = text[i];

  main_loop(video_ptr + TEXT_Y*LINE_WIDTH + TEXT_X,
            background.picture + TEXT_Y*LINE_WIDTH + TEXT_X,
            font.picture, text_buffer, text_buffer + i);
}
