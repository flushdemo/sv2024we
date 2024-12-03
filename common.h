#ifndef __SV2024_COMMON_H
#define __SV2024_COMMON_H 1

// Feature flags
#define FLICKER_REDUCTION 1
//#define USE_DOUBLE_BUFFER 1
//#define NO_SNOW_ON_GNOME 1
#define GNOME_SMART_DRAWING 1
//#define SHOW_FPS 1

// Snow flakes
#define MAX_SNOW_FLAKES 10
#define SNOW_FLAKE_HEIGHT 10

// Screen constants
#define SCREEN_LINES 200 // Number of lines on the screen
#define LINE_WIDTH 80 // 80 16bits blocks per line
#define SCREEN_SIZE (SCREEN_LINES * LINE_WIDTH) // in 16bits blocks
#define BIT_PLANES 4

// Gnome constants
#define GNOME_WIDTH 7 // in 16 bits blocks

// Text zone constants
#define FONT_HEIGHT 16
#define TEXT_HEIGHT (FONT_HEIGHT * 5)
#define CHAR_HEADER_HEIGHT 8
#define CHAR_FOOTER_HEIGHT 8

#define TEXT_BUFFER_SIZE 64
#define TEXT_X 160 - (13*4) // 13 characters - right aligned
#define TEXT_Y ((SCREEN_LINES - TEXT_HEIGHT) / 2 - CHAR_HEADER_HEIGHT)// 5 lines centered
#define TEXT_STARTING_BLOCK 7
#define TEXT_MIN_Y (TEXT_Y - 4) // Take into account sine offset
#define TEXT_MAX_Y (TEXT_Y + TEXT_HEIGHT + CHAR_HEADER_HEIGHT + CHAR_FOOTER_HEIGHT + 4)

// Gnome constants
#define GNOME_MAX_BLOCK 6 // Excluded
#define GNOME_MIN_Y 21
#define GNOME_MAX_Y (160 + GNOME_MIN_Y)

extern char text_buffer[];
extern unsigned short char_block[];
extern unsigned short char_top_y[];

// Assets
extern unsigned short asset_palette[];
extern unsigned short asset_fond[];
extern unsigned short asset_fonte[];
extern unsigned short asset_flocons[];
extern unsigned short asset_musique[];

#endif
