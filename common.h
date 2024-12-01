#ifndef __SV2024_COMMON_H
#define __SV2024_COMMON_H 1

// Feature flags
//#define USE_DOUBLE_BUFFER 1
#define GNOME_SMART_DRAWING 1
#define SHOW_FPS 1

// Screen constants
#define SCREEN_LINES 200 // Number of lines on the screen
#define LINE_WIDTH 80 // 80 16bits blocks per line
#define SCREEN_SIZE (SCREEN_LINES * LINE_WIDTH) // in 16bits blocks
#define BIT_PLANES 4

// Gnome constants
#define GNOME_WIDTH 7 // in 16 bits blocks

// Text constants
#define TEXT_BUFFER_SIZE 256

#endif
