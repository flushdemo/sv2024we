#include "common.h"
#include "printer.h"

// Prints (and erase) characters in the text buffer

static void clear_text_buffer(char* buffer) {
  while (*buffer != '\0') {
    *buffer = '\0';
    buffer++;
  }
}

// FPS improvement of 13% ish
static void optimise_text_buffer(char* buffer) {
  while (*buffer != '\0') {
    if (*buffer == ' ') *buffer = '#';
    buffer++;
  }
}

unsigned short skip_spaces(char* buffer, char* cur_str, unsigned short c_cnt) {
  while (cur_str[c_cnt] == ' ') {
    buffer[c_cnt] = ' ';
    c_cnt++;
  }
  return c_cnt;
}

static void next_step(char* buffer) {
  static unsigned short s_cnt = 0; // String counter into the printer_talk table
  static unsigned short c_cnt = 0; // Character counter into current string
  static unsigned short state = 0; // initial state
  static short display_time = -1;
  static char* cur_str;

  if (display_time == -1) { // Initialization
    cur_str = printer_talk[s_cnt];
    display_time = printer_timing[s_cnt];
    c_cnt = skip_spaces(buffer, cur_str, 0);
  }
  switch (state) {
  case 0: {
    if (cur_str[c_cnt] == '\0') {
      buffer[c_cnt] = '\0';
      if (c_cnt > 0) buffer[c_cnt-1] = cur_str[c_cnt-1];
      optimise_text_buffer(buffer);
      c_cnt = 0;
      state = 1;
    }
    else {
      if (c_cnt > 0) buffer[c_cnt-1] = cur_str[c_cnt-1];
      if (cur_str[c_cnt] == '\n') {
        buffer[c_cnt] = '\n';
        c_cnt = skip_spaces(buffer, cur_str, c_cnt+1);
      }
      else {
        buffer[c_cnt] = '&';
        c_cnt++;
      }
    }
    break;
  }
  case 1: {
    if (c_cnt < display_time) {
      c_cnt++;
    }
    else {
      c_cnt = skip_spaces(buffer, cur_str, 0);
      state = 2;
    }
    break;
  }
  case 2: {
    if (cur_str[c_cnt] == '\0') {
      buffer[c_cnt] = '\0';
      if (c_cnt > 0) buffer[c_cnt-1] = cur_str[c_cnt-1] == '\n' ? '\n' : ' ';
      c_cnt = 0;
      state = 3;
    }
    else {
      if (c_cnt > 0) buffer[c_cnt-1] = cur_str[c_cnt-1] == '\n' ? '\n' : ' ';
      if (cur_str[c_cnt] == '\n') {
        buffer[c_cnt] = '\n';
        c_cnt = skip_spaces(buffer, cur_str, c_cnt+1);
      }
      else {
        buffer[c_cnt] = '&';
        c_cnt++;
      }
    }
    break;
  }
  case 3: {
#ifdef USE_DOUBLE_BUFFER
    if (c_cnt < 1) {
      c_cnt++; // Wait to clean double buffer
    }
    else {
#endif
      clear_text_buffer(buffer);
      s_cnt = printer_talk[s_cnt+1][0] == '\0' ? 0 : s_cnt+1; // loop or not
      cur_str = printer_talk[s_cnt];
      display_time = printer_timing[s_cnt];
      c_cnt = skip_spaces(buffer, cur_str, 0);;
      state = 0;
#ifdef USE_DOUBLE_BUFFER
    }
#endif
    break;
  }
  }
}

void update_printer(char* buffer, unsigned short clk) {
  static old_clk;
  clk &= 0x04; // Trigerring on bit 3 change
  // Every 4 clocks i.e 12.5 times per second.
  if (clk != old_clk) {
    next_step(buffer);
    old_clk = clk;
  }
}
