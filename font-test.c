#include <tos.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// clock register
#define REG_FRCLOCK 0x466

// Font constants
#define FONT_HEIGHT 16
#define FONT_CHARS_PER_LINE 20

// Screen constants
#define LINE_WIDTH 80 // 80 16bits blocks per line
#define BIT_PLANES 4
#define SCREEN_SIZE 16000 // in 16bit blocks

// Text constants
#define LINE_HEIGHT 16
#define TEXT_X 160 - (14*4) // 14 characters - right aligned
#define TEXT_Y (200 - (16*5)) / 2 // 5 lines centered
#define TEXT_BUFFER_SIZE 256
#define TEXT_SHIFT_MASK 0x3f

// Degas file constants
#define DEGAS_FILE_SIZE 32034/2 // in 16bits blocs

struct degas {
  unsigned short resolution;
  unsigned short palette[16];
  unsigned short picture[SCREEN_SIZE];
};
static struct degas font;
static struct degas background;

static char text_buffer[TEXT_BUFFER_SIZE];
static short text_shift[] = {
  0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4,
  5, 4, 4, 4, 4, 4, 4, 3, 3, 3, 2, 2, 1, 1, 0, 0,
  0, 0, 0, -1, -1, -2, -2, -3, -3, -3, -4, -4, -4, -4, -4, -4,
  -5, -4, -4, -4, -4, -4, -4, -3, -3, -3, -2, -2, -1, -1, 0, 0,
};

int load_degas(struct degas *target, char* filename) {
  FILE* fd = fopen(filename, "rb");
  size_t cnt = fread(target, 2, DEGAS_FILE_SIZE, fd);
  fclose(fd);
  return cnt != DEGAS_FILE_SIZE;
}

void display_character(unsigned short* video_ptr,
                       unsigned short* background_ptr,
                       char chr) {
  unsigned short chr_i = chr - ' ';
  unsigned short* font_ptr = font.picture +
    (chr_i / FONT_CHARS_PER_LINE) * FONT_HEIGHT*LINE_WIDTH
    + (chr_i % FONT_CHARS_PER_LINE) * BIT_PLANES;

  for (unsigned short j=0; j < FONT_HEIGHT; j++) {
    unsigned short mask = 0;
    for (unsigned short i=0; i < BIT_PLANES; i++)  mask |= font_ptr[i];
    for (unsigned short i=0; i < BIT_PLANES; i++) {
      video_ptr[i] = (background_ptr[i] & (~mask)) | (font_ptr[i] & mask);
    }
    background_ptr += LINE_WIDTH;
    video_ptr += LINE_WIDTH;
    font_ptr += LINE_WIDTH;
  }
}

// Returns the frames count
long get_clock(void) { return *((long *)REG_FRCLOCK); }

void update_text(unsigned short* video_ptr) {
  unsigned short* background_ptr = background.picture + TEXT_Y*LINE_WIDTH + TEXT_X;

  unsigned short ln_count = 0;
  unsigned short* cur_vd_ptr = video_ptr;
  unsigned short* cur_bg_ptr = background_ptr;
  unsigned short clk = Supexec(get_clock);

  for (unsigned short i=0; text_buffer[i] != '\0'; i++) {
    if (text_buffer[i] == '\n') {
      ln_count++;
      unsigned short d = ln_count * LINE_WIDTH * LINE_HEIGHT;
      cur_vd_ptr = video_ptr + d;
      cur_bg_ptr = background_ptr + d;
    } else {
      short delta = LINE_WIDTH * text_shift[(i*11+clk>>1) & TEXT_SHIFT_MASK];
      display_character(cur_vd_ptr + delta, cur_bg_ptr + delta, text_buffer[i]);
      cur_vd_ptr += BIT_PLANES;
      cur_bg_ptr += BIT_PLANES;
    }
  }
}

void display_picture(unsigned short* video_ptr, unsigned short* picture) {
  for (unsigned short i=0; i<SCREEN_SIZE; i++) {
    video_ptr[i] = picture[i];
  }
}

int main() {
  unsigned short* video_ptr = Physbase();

  char* text =" HOWDY FOLKS\n\nWE ARE BACK @\n\nSILLY VENTURE\n";

  if ( load_degas(&background, "FOND.PI1") != 0 ) {
    printf("Error while loading FOND.PI1\n");
    Cnecin();
    exit(1);
  }
  if ( load_degas(&font, "FONTE.PI1") != 0 ) {
    printf("Error while loading FONTE.PI1\n");
    Cnecin();
    exit(1);
  }

  Setpalette((void*)&(font.palette));
  display_picture(video_ptr, background.picture);

  video_ptr += TEXT_Y*LINE_WIDTH + TEXT_X;

  for (unsigned short i=0; text[i] != '\0'; i++) {
    text_buffer[i] = text[i];
  }
  while (1) {
    update_text(video_ptr);
    // update_sprite(get_clock());
  }

  Cnecin();
}
