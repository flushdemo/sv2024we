CFLAGS=-g

all: binfile main.prg

binfile:
	python3 create-img-bin-file.py 

main.prg: main_raw.prg
	upx -o $@ $<

main_raw.prg: main.o misc.o printer.o sprite.o text.o text-opt.o vbl.o snowflake.o snowflake-opt.o assets.o
	vc +tos $(CFLAGS) -o $@ $^

compact_assets.bin:
	python3 generate_assets_binary.py

assets.o: assets.s compact_assets.bin

%.o: %.s
	vasmm68k_mot -quiet -spaces -Faout -o $@ $<

%.o: %.c
	vc +tos -c $(CFLAGS) -o $@ $<

%.asm: %.c
	vc +tos -S $(CFLAGS) -o $@ $<

clean:
	rm -f *.prg *.tos *.o g[0-9]spr.bin compact_assets.bin

run: main.prg
	hatari --fast-boot true main.prg
