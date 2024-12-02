#!/usr/bin/env python3

ASSET_FOND    = "Assets/fond.pi1"
ASSET_FONTE   = "Assets/fonte.pi1"
ASSET_MUSIQUE = "Assets/FXMS2401.SND"

OUTPUT = "compact_assets.bin"

DEGAS_RESO_HEADER = 2
DEGAS_FULL_HEADER = 34

# 160 bytes per line, 16 lines per character, 3 lines of characters total
FONT_SIZE = 160*16*3

def main():
    with open(OUTPUT, "wb") as out:
        with open(ASSET_FOND, "rb") as fond:
            out.write( fond.read()[DEGAS_RESO_HEADER:] )
        with open(ASSET_FONTE, "rb") as fonte:
            out.write( fonte.read()[
                DEGAS_FULL_HEADER : DEGAS_FULL_HEADER + FONT_SIZE] )
        with open(ASSET_MUSIQUE, "rb") as musique:
            out.write( musique.read() )

main()
