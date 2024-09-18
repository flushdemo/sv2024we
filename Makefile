all: img.prg



img.prg: img.s
	vasmm68k_mot -Ftos -o $@ $^

clean:
	rm -f *.prg *.tos *.o

run: img.prg
	hatari --fast-boot true img.prg


