CSRCFILES:= $(wildcard src/*.c src/*/*.c)

all:
	gcc -mwindows $(CSRCFILES) -o./bin/main.exe