#ifndef __SV2024TEXT_H
#define __SV2024TEXT_H 1

void init_font_mask(unsigned short* font_base);

unsigned short update_text(unsigned short* video_ptr,
                           unsigned short* background_ptr,
                           unsigned short* font_base,
                           const char* text_buffer,
                           unsigned short clk);

void display_picture(unsigned short* video_ptr, unsigned short* picture);

#endif
