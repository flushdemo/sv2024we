Une nouvelle démo ! Incroyable !

Pour lire les image .pi1, utiliser grafx2 sous Linux.

# Performance tracking

v0.1 :  8.88 fps
v0.2 :  9.19 fps [3.5%]  (removed division in font_position)
v0.3 : 12.50 fps [36%]   (precompute font masks)
v0.4 : 12.90 fps [3.2%]  (decreasing loops)
v0.5 : 18.18 fps [40.9%] (rewrote `display_character_opt` in assembly)
v0.6 : 20.25 fps [11.4%] (improve `display_character_opt` inner loop)
v0.7 : 27.11 fps [33.9%] (unroll inner loop)
v0.8 : 29.70 fps [9.5%]  (don't display spaces)
v0.9 : 36.36 fps         (Without FPS display on the moving zone)

# Memory usage improvement

Information from Hatari debugger mode.

## Before font file optimisation
text: 0x30f0, data: 0x1342c, bss: 0x1f894, symtab: 0x690

| text  | 12528  |
| data  | 78892  |
| bss   | 129172 |
|-------|--------|
| total | 220592 |

## After font file  optimisation:
text: 0x31a0, data: 0x1342c, bss: 0x13a94, symtab: 0x6ac

| text  | 12704  |
| data  | 78892  |
| bss   | 80532  |
|-------|--------|
| total | 172128 |


