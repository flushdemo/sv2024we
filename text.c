#include "common.h"
#include "text.h"

// Font constants
#define FONT_HEIGHT 16
#define FONT_CHARS_PER_LINE 20

// Text constants
#define LINE_HEIGHT 16
#define TEXT_SHIFT_MASK 0x3f

// Sinus table - 64 values
static short text_shift[] = {
  0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4,
  5, 4, 4, 4, 4, 4, 4, 3, 3, 3, 2, 2, 1, 1, 0, 0,
  0, 0, 0, -1, -1, -2, -2, -3, -3, -3, -4, -4, -4, -4, -4, -4,
  -5, -4, -4, -4, -4, -4, -4, -3, -3, -3, -2, -2, -1, -1, 0, 0,
};

unsigned short font_position(unsigned short chr) {
  unsigned short chr_i = chr - ' ';
  return (chr_i / FONT_CHARS_PER_LINE) * FONT_HEIGHT*LINE_WIDTH
    + (chr_i % FONT_CHARS_PER_LINE) * BIT_PLANES;
}

void display_character(unsigned short* video_ptr,
                              unsigned short* background_ptr,
                              unsigned short* font_base,
                              unsigned short chr) {
  unsigned short* font_ptr = font_base + font_position(chr);

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

// Returns the position of the end of string (character '\0')
unsigned short update_text(unsigned short* video_ptr,
                           unsigned short* background_ptr,
                           unsigned short* font_base,
                           const char* text_buffer,
                           unsigned short clk) {
  unsigned short ln_count = 0;
  unsigned short* cur_vd_ptr = video_ptr;
  unsigned short* cur_bg_ptr = background_ptr;
  unsigned short i;

  for (i=0; text_buffer[i] != '\0'; i++) {
    if (text_buffer[i] == '\n') {
      ln_count++;
      unsigned short d = ln_count * LINE_WIDTH * LINE_HEIGHT;
      cur_vd_ptr = video_ptr + d;
      cur_bg_ptr = background_ptr + d;
    } else {
      short delta = LINE_WIDTH * text_shift[(i*11+clk>>1) & TEXT_SHIFT_MASK];
      display_character(cur_vd_ptr + delta, cur_bg_ptr + delta,
                        font_base, text_buffer[i]);
      cur_vd_ptr += BIT_PLANES;
      cur_bg_ptr += BIT_PLANES;
    }
  }
  return i;
}

void display_picture(unsigned short* video_ptr, unsigned short* picture) {
  for (unsigned short i=0; i<SCREEN_SIZE; i++) {
    video_ptr[i] = picture[i];
  }
}
