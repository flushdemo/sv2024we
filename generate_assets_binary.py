#!/usr/bin/env python3

ASSET_FOND    = "Assets/fond.pi1"
ASSET_FONTE   = "Assets/fonte.pi1"
ASSET_FLOCONS = "Assets/flocons.pi1"
ASSET_MUSIQUE = "Assets/FXMS2401.SND"

OUTPUT = "compact_assets.bin"

DEGAS_RESO_HEADER = 2
DEGAS_FULL_HEADER = 34

LINE_WIDTH = 160
# 160 bytes per line, 16 lines per character 3 lines of characters total
FONT_SIZE = LINE_WIDTH*16*3

# snowflakes constants
N_FLAKES = 4
BIT_PLANES = 4
CHUNK_WIDTH = BIT_PLANES*2
EXT_LINE_WIDTH = N_FLAKES * 2*BIT_PLANES

def dump_flocons(data):
    for line in [data[i*EXT_LINE_WIDTH: (i+1)*EXT_LINE_WIDTH] for i in range(8)]:
        print([hex(d) for d in line])

def extract_flocons(degas):
    data = degas[DEGAS_FULL_HEADER:]
    res = []
    for j in range(8): # 8 lines
        for chunk in range(N_FLAKES//2): # flakes are interlaced in words
            # desinterlacing
            s = j*LINE_WIDTH + CHUNK_WIDTH*chunk
            flake0 = []
            flake1 = []
            for bp in range(BIT_PLANES):
                flake0.append(data[s + 2*bp])
                flake1.append(data[s + 2*bp + 1])

            # Extend bytes to word
            ext0 = []
            for e in flake0:
                ext0.extend([0, e])
            ext1 = []
            for e in flake1:
                ext1.extend([0, e])

            res.extend(ext0)
            res.extend(ext1)
    return bytes(res)


def main():
    with open(OUTPUT, "wb") as out:
        with open(ASSET_FOND, "rb") as fd:
            out.write( fd.read()[DEGAS_RESO_HEADER:] )
        with open(ASSET_FONTE, "rb") as fd:
            out.write( fd.read()[
                DEGAS_FULL_HEADER : DEGAS_FULL_HEADER + FONT_SIZE] )
        with open(ASSET_FLOCONS, "rb") as fd:
            out.write( extract_flocons(fd.read()) )
        with open(ASSET_MUSIQUE, "rb") as fd:
            out.write( fd.read() )

main()
