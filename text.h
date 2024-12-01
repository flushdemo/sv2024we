#ifndef __SV2024_TEXT_H
#define __SV2024_TEXT_H 1

// Font constants
#define FONT_HEIGHT 16
#define FONT_CHARS_PER_LINE 20
#define FONT_LINE (FONT_HEIGHT * LINE_WIDTH)
#define FONT_SIZE (3 * FONT_LINE) // 3 characters lines (60 chars) in a file

// Text constants
#define LINE_HEIGHT 16
#define TEXT_SHIFT_MASK 0x3f

void init_font_mask(unsigned short* font_base);

unsigned short update_text(unsigned short* video_ptr,
                           unsigned short* background_ptr,
                           unsigned short* font_base,
                           const char* text_buffer,
                           unsigned short clk);

void display_picture(unsigned short* video_ptr, unsigned short* picture);

// Display a font buffer for troubleshooting purpose
void display_font(unsigned short* video_ptr, unsigned short* font_data);

// Assembly version
void display_character_opt(unsigned short* video_ptr,
                           unsigned short* background_ptr,
                           unsigned short* font_ptr,
                           unsigned short* mask_ptr);

#endif
