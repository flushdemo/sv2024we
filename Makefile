CFLAGS=-g

all: img.prg

img.prg: img.s
	vasmm68k_mot -Ftos -o $@ $^

main-run: main.prg
	hatari --fast-boot true main.prg

main.prg: main.o vbl.o text.o misc.o sprite.o text-opt.o
	vc +tos $(CFLAGS) -o $@ $^

%.o: %.s
	vasmm68k_mot -spaces -Faout -o $@ $^

%.o: %.c
	vc +tos -c $(CFLAGS) -o $@ $^

%.asm: %.c
	vc +tos -S $(CFLAGS) -o $@ $^

clean:
	rm -f *.prg *.tos *.o

run: img.prg
	hatari --fast-boot true img.prg
