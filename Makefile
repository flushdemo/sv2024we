all: img.prg

img.prg: img.s
	vasmm68k_mot -Ftos -o $@ $^

font-test.prg: font-test.c
	vc +tos $^ -o $@

font-test-run: font-test.prg
	hatari --fast-boot true font-test.prg

clean:
	rm -f *.prg *.tos *.o

run: img.prg
	hatari --fast-boot true img.prg
