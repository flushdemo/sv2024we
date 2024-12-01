## size types on 68k
BYTE_SIZE = 1
WORD_SIZE = 2
LONGWORD_SIZE = 4

# Atari ST graphics
NB_COLORS = 16
SCREEN_SIZE_IN_BYTES = 32000
SCREEN_WIDTH_IN_BYTES = 160

# Degas Elite picture
DEGAS_MARKER_SIZE = 2
PALETTE_INDEX = 2
PALETTE_SIZE = 16
PALETTE_SIZE_IN_BYTES = (WORD_SIZE*PALETTE_SIZE) + DEGAS_MARKER_SIZE
PICTURE_INDEX = PALETTE_SIZE_IN_BYTES

# Sprite Elements
NB_BLOCK_COPY = 7
NB_LINES_TO_COPY = 175
POSITION_LINE_SPRITE = 16*160
BLOCK_SIZE = 8

pi1_list = [
  "Assets/g0.pi1",  "Assets/g1.pi1",  "Assets/g2.pi1",  "Assets/g3.pi1",
  "Assets/g4.pi1",  "Assets/g5.pi1",  "Assets/g6.pi1",  "Assets/g7.pi1"
]
bin_list = [
  "g0spr.bin",  "g1spr.bin",  "g2spr.bin",  "g3spr.bin",
  "g4spr.bin",  "g5spr.bin",  "g6spr.bin",  "g7spr.bin"
]

nb_files_to_process = len(pi1_list)

for file_index in range(nb_files_to_process):
  with open(pi1_list[file_index], "rb") as inputfile:
    picture_file = bytes(inputfile.read())

  palette = picture_file[PALETTE_INDEX:PALETTE_SIZE_IN_BYTES]
  picture = picture_file[PICTURE_INDEX:SCREEN_SIZE_IN_BYTES]

  POSITION_LINE_SPRITE = 16*SCREEN_WIDTH_IN_BYTES

  with open(bin_list[file_index], "wb") as outfile:
      for i in range(NB_LINES_TO_COPY):
        for b in range(NB_BLOCK_COPY):
          index_start = POSITION_LINE_SPRITE + (i*SCREEN_WIDTH_IN_BYTES) + (b * BLOCK_SIZE)
          index_end = index_start + BLOCK_SIZE
          #print("%08x => %08x i = %i b = %i" % (index_start, index_end, i, b))
          outfile.write(picture[index_start:index_end])