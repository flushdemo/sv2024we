
BYTE_SIZE = 1
WORD_SIZE = 2
LONGWORD_SIZE = 4
NB_COLORS = 16
SCREEN_SIZE_IN_BYTES = 32000
SCREEN_WIDTH_IN_BYTES = 160
DEGAS_MARKER_SIZE = 2
PALETTE_INDEX = 2
PALETTE_SIZE = 16
PALETTE_SIZE_IN_BYTES = (WORD_SIZE*PALETTE_SIZE) + DEGAS_MARKER_SIZE
PICTURE_INDEX = PALETTE_SIZE_IN_BYTES
NB_BLOCK_COPY = 7
NB_LINES_TO_COPY = 175
POSITION_LINE_SPRITE = 16*160
BLOCK_SIZE = 8


with open("Assets/g0.pi1", "rb") as inputfile:
    picture_file = bytes(inputfile.read())

palette = picture_file[PALETTE_INDEX:PALETTE_SIZE_IN_BYTES]
picture = picture_file[PICTURE_INDEX:SCREEN_SIZE_IN_BYTES]

POSITION_LINE_SPRITE = 16*SCREEN_WIDTH_IN_BYTES



with open("g0python.bin", "wb") as outfile:
    for i in range(NB_LINES_TO_COPY):
      for b in range(NB_BLOCK_COPY):
        index_start = POSITION_LINE_SPRITE + (i*SCREEN_WIDTH_IN_BYTES) + (b * BLOCK_SIZE)
        index_end = index_start + BLOCK_SIZE
        #print("%08x => %08x i = %i b = %i" % (index_start, index_end, i, b))
        outfile.write(picture[index_start:index_end])

