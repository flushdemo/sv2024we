#include "common.h"
#include "text.h"

// Mask to be used to display font
unsigned short font_mask[FONT_SIZE];

// sin 64 values
// [round(3*math.sin(x/64 * 2*math.pi)) for x in range(64)]
static short text_shift[] =
  {
    0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, -1, -1, -1, -1, -2, -2, -2, -2, -2, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -2, -2, -2, -2, -2, -1, -1, -1, -1, 0
  };

// 2% time spent there
// Optimisation of the following code
// return (chr_i / FONT_CHARS_PER_LINE) * FONT_LINE
//      + (chr_i % FONT_CHARS_PER_LINE) * BIT_PLANES;
unsigned short font_position(unsigned short chr) {
  unsigned short chr_i = chr - ' ';
  // values in;
  //  0-19 : first line
  // 20-39 : second line
  // 40-59 : third line
  if        (chr_i >= 40) { // third line
    return 2*FONT_LINE + ((chr_i - 40) << 2);
  } else if (chr_i >= 20) { // second line
    return FONT_LINE + ((chr_i - 20) << 2);
  }
  return chr_i << 2;
}

// Function deprecated
// Use display_character_opt instead - speed x2-3
void display_character(unsigned short* video_ptr,
                              unsigned short* background_ptr,
                              unsigned short* font_base,
                              unsigned short chr) {
  unsigned short font_pos = font_position(chr);
  unsigned short* font_ptr = font_base + font_pos;
  unsigned short* mask_ptr = font_mask + font_pos;

  for (short j=FONT_HEIGHT-1; j >= 0; j--) {
    unsigned short mask = mask_ptr[0]; // same mask for the 4 bitplanes
    for (short i=BIT_PLANES-1; i >= 0; i--) {
      video_ptr[i] = (background_ptr[i] & (~mask)) | (font_ptr[i] & mask);
    }
    background_ptr += LINE_WIDTH;
    video_ptr += LINE_WIDTH;
    font_ptr += LINE_WIDTH;
    mask_ptr += LINE_WIDTH;
  }
}

void init_font_mask(unsigned short* font_base) {
  for (unsigned short i=0; i < SCREEN_SIZE / BIT_PLANES; i += BIT_PLANES) {
    unsigned short m = 0;
    for (unsigned short j=0; j < BIT_PLANES; j++) m |= font_base[i+j];
    // Storing inverted mask
    for (unsigned short j=0; j < BIT_PLANES; j++) font_mask[i+j] = ~m;
  }
}

// 6% time spent in this function
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
      if (text_buffer[i] != '#') { // Special character to speed up display
        unsigned short font_pos = font_position(text_buffer[i]);
        short delta = LINE_WIDTH * text_shift[(i*11+clk) & TEXT_SHIFT_MASK];
        display_character_opt(cur_vd_ptr + delta, cur_bg_ptr + delta,
                              font_base + font_pos, font_mask + font_pos);
      }
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

void display_font(unsigned short* video_ptr, unsigned short* font_data) {
  for (unsigned short i=0; i<FONT_SIZE; i++) {
    video_ptr[i] = font_data[i];
  }
}
