Une nouvelle démo ! Incroyable !

# Notes

Pour lire les image .pi1, utiliser grafx2 sous Linux.

## Taille du gnome

origine x,y (0, 21), largeur,hauteur (96, 160)

# Todo

## Text

- display only when moving

## Snowflakes drawings

- use backsnow only in sprites zones
- use only required bitplanes
- critical code in assembly ?

## general

- All assets in a single binary, then in the prg
- Compress

## gnome

- 1 block xored, 4 blocks precalc, 1 block xored
- performance tuning


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
v1.1 : 45.39 fps [24.8%] (Redraw gnome only when needed)
v1.2 : 16.66 fps [-63%]  (Flocons with double buffer)
       23.52 fps [-48%]  (Flocons without double buffer)
v1.3 : 31.37 fps [33.4%] (Redraw flakes only when really moving) [no double buffer]
v1.4 : 38.09 fps [21.4%] (Redraw gnome only when needed) [no double buffer]
v1.5 : 37.20 fps [-3%]   (Smart snow flakes masks) [no double buffer]
v1.7 : 47.76 fps [28.4%] (Critical operations on longs instead of words + optim)
v2.1 : 36.36 fps [-24%]  (Gnome xored with background)
v2.2 : 31.37 fps [-24%]  (snowflakes sine move)
v2.3 : 106.66 fps [240%] (Smart text drawing, i.e only when changing)

# Memory usage

## target

| background      | 32000  |
| font            | 7680   |
| music-buffer    | 32768  |
| text-buffer     | 256    |
| backsnow-buffer | 32000  |
| video-buffer    | 32000  |
| video-ram       | 32000  |
| font-mask       | 7680   |
| gnome-picture   | 61440  |
| gnome-mask      | 15360  |
|-----------------|--------|
| total           | 253184 |

## v2.0

text: 0x36ac, data: 0x134a4, bss: 0x3684e, symtab: 0x70e

| text  | 13996  |
| data  | 79012  |
| bss   | 223310 |
|-------|--------|
| total | 326318 |

delta vs target = 59138 (- (- 326318 253184) 13996)

## test

text: 0x3694, data: 0x134a4, bss: 0x2cf32, symtab: 0x70e

| text  | 13972  |
| data  | 79012  |
| bss   | 184114 |
|-------|--------|
| total | 277098 |

Delta v2.0 : 49220


