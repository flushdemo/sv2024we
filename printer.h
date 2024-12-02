#ifndef __SV2024_PRINTER_H
#define __SV2024_PRINTER_H 1

void update_printer(char* buffer, unsigned short clk);

// printer talk and timing generated from printer_talk.txt
extern char *printer_talk[];
extern short printer_timing[];

#endif
