
files := src/main.c

all: $(files)
	gcc $(files) -o server

clean:
	rm server