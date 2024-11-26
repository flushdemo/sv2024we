#include <tos.h>
#include <stdio.h>

// Font constants
#define FONT_HEIGHT 16
#define FONT_CHARS_PER_LINE 20

// Screen constants
#define LINE_WIDTH 80 // 80 16bits blocks per line
#define BIT_PLANES 4
#define SCREEN_SIZE 16000 // in 16bit blocks

// Text constants
#define LINE_HEIGHT 32
#define TEXT_X 160 - (14*4) // 14 characters - right aligned
#define TEXT_Y (200 - (16*5)) / 2 // 5 lines centered

// Degas file constants
#define DEGAS_FILE_SIZE 32034/2 // in 16bits blocs

struct degas {
  unsigned short resolution;
  unsigned short palette[16];
  unsigned short picture[SCREEN_SIZE];
};
static struct degas font;
static struct degas background;

int load_degas(struct degas *target, char* filename) {
  FILE* fd = fopen(filename, "rb");
  size_t cnt = fread(target, 2, DEGAS_FILE_SIZE, fd);
  fclose(fd);
  return cnt != DEGAS_FILE_SIZE;
}

void display_character(unsigned short* video_ptr, char chr) {
  unsigned short chr_i = chr - ' ';
  unsigned short chr_pos =
    (chr_i / FONT_CHARS_PER_LINE) * FONT_HEIGHT*LINE_WIDTH
    + (chr_i % FONT_CHARS_PER_LINE) * BIT_PLANES;
  for (unsigned short j=0; j < FONT_HEIGHT; j++) {
    for (unsigned short i=0; i < BIT_PLANES; i++) {
      video_ptr[i + j*LINE_WIDTH] = font.picture[chr_pos + i + j*LINE_WIDTH];
    }
  }
}

void display_string(unsigned short* video_ptr, char* str) {
  while (*str != '\0') {
    display_character(video_ptr, *str);
    str++;
    video_ptr += BIT_PLANES;
  }
}

void display_text(unsigned short* video_ptr, char** text) {
  while (*text != '\0') {
    display_string(video_ptr, *text);
    text++;
    video_ptr += LINE_WIDTH * LINE_HEIGHT;
  }
}

void display_picture(unsigned short* video_ptr, unsigned short* picture) {
  for (unsigned short i=0; i<SCREEN_SIZE; i++) {
    video_ptr[i] = picture[i];
  }
}

int main() {
  char* text[4];
  text[0] = " HOWDY FOLKS";
  text[1] = "WE ARE BACK @";
  text[2] = "SILLY VENTURE";
  text[3] = "";

  load_degas(&background, "FOND.PI1");
  load_degas(&font, "FONTE.PI1");

  unsigned short* video_ptr = Physbase();

  Setpalette((void*)&(font.palette));
  display_picture(video_ptr, background.picture);
  display_text(video_ptr + TEXT_Y*LINE_WIDTH + TEXT_X, text);
  
  Cnecin();
}
