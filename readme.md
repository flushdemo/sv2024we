Une nouvelle démo ! Incroyable !

Pour lire les image .pi1, utiliser grafx2 sous Linux.

# Performance tracking

v0.1 :  8.88 fps
v0.2 :  9.19 fps [3.5%]  (removed division in font_position)
v0.3 : 12.50 fps [36%]   (precompute font masks)
v0.4 : 12.90 fps [3.2%]  (decreasing loops)
v0.5 : 18.18 fps [40.9%] (rewrote `display_character_opt` in assembly)

# Possible improvements

- Reduce memory used by font and font mask
