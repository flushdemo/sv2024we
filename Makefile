CFLAGS=-g

all: main.prg

main.prg: main.o misc.o printer.o sprite.o text.o text-opt.o vbl.o snowflake.o
	vc +tos $(CFLAGS) -o $@ $^

%.o: %.s
	vasmm68k_mot -spaces -Faout -o $@ $^

%.o: %.c
	vc +tos -c $(CFLAGS) -o $@ $^

%.asm: %.c
	vc +tos -S $(CFLAGS) -o $@ $^

clean:
	rm -f *.prg *.tos *.o

run: main.prg
	hatari --fast-boot true main.prg
