all: img.prg


img.prg: img.o 
	vlink -bataritos -o $@ $^

%.o: %.s
	vasmm68k_mot -Felf -o $@ $^

clean:
	rm -f *.prg *.tos *.o

run: img.prg
	hatari --fast-boot true img.prg


