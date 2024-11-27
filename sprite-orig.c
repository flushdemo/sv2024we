#include "common.h"
#include "sprite.h"

void update_sprite(unsigned short* video_ptr,
                   unsigned short* background_ptr,
                   unsigned short clk) {
  unsigned short* tata = background_ptr;
  video_ptr += LINE_WIDTH*90 + 2*BIT_PLANES;
  for (unsigned short i=0; i<20; i++) {
    *video_ptr = *tata | clk;
    video_ptr+= LINE_WIDTH;
  }
}
