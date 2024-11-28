// Prints (and erase) characters in the text buffer

#define DISPLAY_TIME 64

static char *talking[] = {
  " HOWDY FOLKS\n\nWE ARE BACK @\n\nSILLY VENTURE\n",
  " GFX EXOCET\n\n   MSX DMA\n\nCDX FLEW/MARA\n",
  ""
};

static void clear_text_buffer(char* buffer) {
  while (*buffer != '\0') {
    *buffer = '\0';
    buffer++;
  }
}

static void next_step(char* buffer) {
  static unsigned short s_cnt = 0; // String counter into the talking table
  static unsigned short c_cnt = 0; // Character counter into current string
  static unsigned short state = 0;

  switch (state) {
  case 0: {
    if (talking[s_cnt][c_cnt] == '\0') {
      c_cnt = 0;
      state = 1;
    } else {
      buffer[c_cnt] = talking[s_cnt][c_cnt];
      c_cnt++;
    }
    break;
  }
  case 1: {
    if (c_cnt < DISPLAY_TIME) {
      c_cnt++;
    } else {
      c_cnt = 0;
      state = 2;
    }
    break;
  }
  case 2: {
    if (talking[s_cnt][c_cnt] == '\0') {
      clear_text_buffer(buffer);
      s_cnt++;
      if (talking[s_cnt][0] == '\0') s_cnt = 0; // loop
      c_cnt = 0;
      state = 0;
    } else {
      if (talking[s_cnt][c_cnt] != '\n' && talking[s_cnt][c_cnt] != ' ') {
        buffer[c_cnt] = '#';
      }
      c_cnt++;
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
