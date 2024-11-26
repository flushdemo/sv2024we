Title: DEGAS file format

```
DEGAS           *.PI1 (low resolution)
                *.PI2 (medium resolution)
                *.PI3 (high resolution)
                (PI4, PI5, PI6)

1 word          resolution (0 = low res, 1 = medium res, 2 = high res)
                Other bits may be used in the future; use a simple bit
                test rather than checking for specific word values.
16 words        palette
16000 words     picture data (screen memory)
-----------
32034 bytes     total

Conny Pettersson of Unit Seventeen proposed to extend the Degas format with 
PI4 and PI5 for TT resolutions in a text file accompaning the viewst program:

For saving the pictures, I have extended the standard Degas-format PI0 to
the TT-resolutions. Following is a description of the format I use:
               ST-Low                  TT-Low                  TT-Medium
Extension      PI0                     PI4                     PI5
Resolution     1 word: $0000           1 word: $0007           1 word: $0004
Colormap       16 word                 256 word                16 word
Bitmap         32000 bytes             153600 bytes            153600 bytes
Total length   32034 bytes             154114 bytes            153634 bytes

This format is not in wide spread use as datacompression on those huge screens 
really makes sence. Nevertheless various file-extension decriptions on the 
internet assign the extensions PI4, PI5 and PI6 to TT Degas resolutions.
```

source: https://temlib.org/AtariForumWiki/index.php/DEGAS_file_format
