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

void display_string(unsigned short* video_ptr,
                    unsigned short* background_ptr,
                    char* str) {
  while (*str != '\0') {
    display_character(video_ptr, background_ptr, *str);
    str++;
    video_ptr += BIT_PLANES;
    background_ptr += BIT_PLANES;
  }
}

void display_text(unsigned short* video_ptr,
                  unsigned short* background_ptr,
                  char** text) {
  while (**text != '\0') {
    display_string(video_ptr, background_ptr, *text);
    text++;
    video_ptr += LINE_WIDTH * LINE_HEIGHT;
    background_ptr += LINE_WIDTH * LINE_HEIGHT;
  }
}

void display_picture(unsigned short* video_ptr, unsigned short* picture) {
  for (unsigned short i=0; i<SCREEN_SIZE; i++) {
    video_ptr[i] = picture[i];
  }
}

int main() {
  unsigned short* video_ptr = Physbase();
  unsigned short* background_ptr = background.picture;

  char* text[4];
  text[0] = " HOWDY FOLKS";
  text[1] = "WE ARE BACK @";
  text[2] = "SILLY VENTURE";
  text[3] = "";

  load_degas(&background, "FOND.PI1");
  load_degas(&font, "FONTE.PI1");

  Setpalette((void*)&(font.palette));
  display_picture(video_ptr, background_ptr);

  video_ptr += TEXT_Y*LINE_WIDTH + TEXT_X;
  background_ptr += TEXT_Y*LINE_WIDTH + TEXT_X;
  display_text(video_ptr, background_ptr, text);
  
  Cnecin();
}
