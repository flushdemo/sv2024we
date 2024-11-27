all: img.prg

img.prg: img.s
	vasmm68k_mot -Ftos -o $@ $^

main-run: main.prg
	hatari --fast-boot true main.prg

main.prg: main.o text.o low-level.o
	vc +tos -o $@ $^

%.o: %.s
	vasmm68k_mot -Faout -o $@ $^

%.o: %.c
	vc +tos -c -o $@ $^

clean:
	rm -f *.prg *.tos *.o

run: img.prg
	hatari --fast-boot true img.prg
