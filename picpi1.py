#!/usr/bin/env python3
import struct

PI1_LOW_RES_UNCOMPRESS = 0x0000
PI1_FATAL              = 0xFFFF
PAL_SIZE = 16
PI1_LINES = 200
PI1_PLANES_8_BYTES_GROUP = 20

pal_tab = []
p1_tab = []
p2_tab = []
p3_tab = []
p4_tab = []

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

def pi1_plane_to_array(infile):
    for y in range(PI1_LINES):
        for x in range(PI1_PLANES_8_BYTES_GROUP):
            p1_value, p2_value, p3_value, p4_value = struct.unpack(">HHHH", infile.read(8))
            p1_tab.append(p1_value)
            p2_tab.append(p2_value)
            p3_tab.append(p3_value)
            p4_tab.append(p4_value)

    print(len(p1_tab))
    print(len(p2_tab))
    print(len(p3_tab))
    print(len(p4_tab))

def pi1_pic_array_to_file(p1, p2, p3, p4):
    print("verif")
    print(len(p1))
    print(len(p2))
    print(len(p3))
    print(len(p4))
    
    with open("adn_pic_p1.bin", "wb") as outfile_p1:
        for i in range(len(p1)):
            outfile_p1.write(struct.pack(">H", p1[i]))
            
    with open("adn_pic_p2.bin", "wb") as outfile_p2:
        for i in range(len(p2)):
            outfile_p2.write(struct.pack(">H", p2[i]))

    with open("adn_pic_p3.bin", "wb") as outfile_p3:
        for i in range(len(p3)):
            outfile_p3.write(struct.pack(">H", p3[i]))

    with open("adn_pic_p4.bin", "wb") as outfile_p4:
        for i in range(len(p4)):
            outfile_p4.write(struct.pack(">H", p4[i]))

with open(FILENAME_INPUT, "rb") as infile:
     
    if check_degas_version(infile) == PI1_FATAL:
          print("[!] The picture seems not a degas elite (pi1) picture !")
          exit()
     
    pi1_pal_to_array(infile)

#     pi1_pal_array_to_file(pal_tab)
    pi1_plane_to_array(infile)

    pi1_pic_array_to_file(p1_tab, p2_tab, p3_tab, p4_tab)
