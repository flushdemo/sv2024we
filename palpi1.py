#!/usr/bin/env python3
import struct

PI1_LOW_RES_UNCOMPRESS = 0x0000
PI1_FATAL              = 0xFFFF
PAL_SIZE = 16

pal_tab = []

FILENAME_INPUT  = "adn-font3.pi1"
FILENAME_OUTPUT = "adn-font3.pal"

def check_degas_version(stream):
     pi1_marker, = struct.unpack(">H",infile.read(2))
     print(hex(pi1_marker))
     if pi1_marker == PI1_LOW_RES_UNCOMPRESS:
          return PI1_LOW_RES_UNCOMPRESS
     else:
          return PI1_FATAL

def pi1_pal_to_array(infile):
     for i in range(PAL_SIZE):
               pal_value,  = struct.unpack(">H", infile.read(2))
               pal_tab.append(pal_value)

def pi1_pal_array_to_file(pal_tab):
      for i in range(PAL_SIZE):
          print("index = %.2d => 0x%.4x" % (i, pal_tab[i]))
          with open(FILENAME_OUTPUT, "wb") as outfile:
               for i in range(PAL_SIZE):
                    outfile.write(struct.pack(">H", pal_tab[i]))


with open(FILENAME_INPUT, "rb") as infile:
     
     if check_degas_version(infile) == PI1_FATAL:
          print("[!] The picture seems not a degas elite (pi1) picture !")
          exit()
     
     pi1_pal_to_array(infile)

     pi1_pal_array_to_file(pal_tab)
     
