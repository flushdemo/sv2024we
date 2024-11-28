#ifndef __SV2024SPRITE_H
#define __SV2024SPRITE_H 1

void init_font_mask();

void update_sprite(unsigned short* video_ptr,
                   unsigned short* background_ptr,
                   unsigned short clk);

#endif
