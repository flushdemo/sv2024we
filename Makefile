CFLAGS=-g

all: binfile main.prg

binfile:
	python3 create-img-bin-file.py 

main.prg: main.o misc.o printer.o sprite.o text.o text-opt.o vbl.o snowflake.o snowflake-opt.o
	vc +tos $(CFLAGS) -o $@ $^

%.o: %.s
	vasmm68k_mot -spaces -Faout -o $@ $^

%.o: %.c
	vc +tos -c $(CFLAGS) -o $@ $^

%.asm: %.c
	vc +tos -S $(CFLAGS) -o $@ $^

clean:
	rm -f *.prg *.tos *.o g[0-9]spr.bin

run: main.prg
	hatari --fast-boot true main.prg
