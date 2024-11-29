// Prints (and erase) characters in the text buffer

#define DISPLAY_TIME 64


static char *talking[] = {
  " HOWDY FOLKS\n\nWE ARE BACK @\n\nSILLY VENTURE",
  " WE WISH YOU\n\n ALL A MERRY\n\n  CHRISTMAS",
  "ALSO YOU ARE\n\n INVITED TO\n\nSHADOW PARTY",
  " GFX EXOCET\n\n   MSX DMA\n\nCDX FLEW/MARA",
  ""
};

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

static void next_step(char* buffer) {
  static unsigned short s_cnt = 0; // String counter into the talking table
  static unsigned short c_cnt = 0; // Character counter into current string
  static unsigned short state = 0;
  char* cur_str = talking[s_cnt];

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
      buffer[c_cnt] = cur_str[c_cnt] == '\n' ? '\n' : '&';
      if (c_cnt > 0) buffer[c_cnt-1] = cur_str[c_cnt-1];
      c_cnt++;
    }
    break;
  }
  case 1: {
    if (c_cnt < DISPLAY_TIME) {
      c_cnt++;
    }
    else {
      c_cnt = 0;
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
      buffer[c_cnt] = cur_str[c_cnt] == '\n' ? '\n' : '&';
      if (c_cnt > 0) buffer[c_cnt-1] = cur_str[c_cnt-1] == '\n' ? '\n' : ' ';
      c_cnt++;
    }
    break;
  }
  case 3: {
    if (c_cnt < 1) {
      c_cnt++; // Wait to clean display buffers
    }
    else {
      clear_text_buffer(buffer);
      s_cnt = talking[s_cnt+1][0] == '\0' ? 0 : s_cnt+1; // loop or not
      c_cnt = 0;
      state = 0;
    }
    break;
  }
  }
}

void update_printer(char* buffer, unsigned short clk) {
  static old_clk;
  clk &= 0x04; // Trigerring on bit 8 change
  if (clk != old_clk) {
    next_step(buffer);
    old_clk = clk;
  }
}
