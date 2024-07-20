
srcFiles := src/main.c
incFiles := src/include/tmp.h

all: $(srcFiles) $(incFiles)
	gcc $(srcFiles) $(incFiles) -o server


run: all
	./server


clean:
	rm server