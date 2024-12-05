#CFLAGS=-g
UPX_FLAGS=--force-overwrite

# Rebuild printer_talk.c everytime to embed the version number in the binary automatically
.PHONY: dist run printer_talk.c

all: gnoel.prg

dist: gnoel.zip

run: gnoel_raw.prg
	hatari --fast-boot true gnoel_raw.prg

gnoel.zip: file_id.diz gnoel.nfo gnoel.prg
	zip -z9	$@ $^ < $<

gnoel.prg: gnoel_raw.prg
	upx $(UPX_FLAGS) -o $@ $<

gnoel_raw.prg: main.o misc.o printer.o printer_talk.o sprite.o text.o text-opt.o vbl.o snowflake.o snowflake-opt.o assets.o
	vc +tos $(CFLAGS) -o $@ $^

_sprite_binaries.bin:
	python3 create-img-bin-file.py
	touch _sprite_binaries.bin

compact_assets.bin:
	python3 generate_assets_binary.py

printer_talk.c: printer_talk.txt
	python3 generate_printer_talk.py $< > $@

sprite.o: _sprite_binaries.bin

assets.o: assets.s compact_assets.bin

%.o: %.s
	vasmm68k_mot -quiet -spaces -Faout -o $@ $<

%.o: %.c
	vc +tos -c $(CFLAGS) -o $@ $<

%.asm: %.c
	vc +tos -S $(CFLAGS) -o $@ $<

clean:
	rm -f *.prg *.tos *.o *.bin *.zip printer_talk.c
