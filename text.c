#include "common.h"
#include "text.h"

// Mask to be used to display font - actually background behind font
unsigned short font_mask[FONT_SIZE];

// Storing coordinates of each text box
unsigned short char_block[TEXT_BUFFER_SIZE]; // block count 0 - 19
unsigned short char_top_y[TEXT_BUFFER_SIZE];

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
void update_text(unsigned short* video_ptr,
                           unsigned short* background_ptr,
                           unsigned short* font_base,
                           const char* text_buffer,
                           unsigned short clk) {
  static short old_y_offset[TEXT_BUFFER_SIZE];
  static short old_text[TEXT_BUFFER_SIZE];

  unsigned short ln_count = 0;
  unsigned short blk_count = TEXT_STARTING_BLOCK;

  for (unsigned short i=0; text_buffer[i] != '\0'; i++) {
    if (text_buffer[i] == '\n') {
      ln_count++;
      blk_count = TEXT_STARTING_BLOCK; // To update bouding boxes
    } else {
      if (text_buffer[i] != '#') { // Special character to speed up display
        unsigned short y_offset = text_shift[(i*11+clk) & TEXT_SHIFT_MASK];
        if (old_y_offset[i] != y_offset || old_text[i] != text_buffer[i]) { // Did we change anything ?
          unsigned short y_coord  = TEXT_Y + ln_count * LINE_HEIGHT + y_offset;
          unsigned short y_disp   = y_coord * LINE_WIDTH;
          unsigned short x_disp   = blk_count * BIT_PLANES;
          unsigned short font_pos = font_position(text_buffer[i]);

          display_character_opt(video_ptr + y_disp + x_disp,
                                background_ptr + y_disp + x_disp,
                                font_base + font_pos, font_mask + font_pos);

          // Update previous state
          old_y_offset[i] = y_offset;
          old_text[i] = text_buffer[i];

#ifdef FLICKER_REDUCTION
          // Update characters bounding boxes
          char_block[i] = blk_count;
          char_top_y[i] = y_coord;
#endif
        }
      }
      blk_count++;
    }
  }
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
