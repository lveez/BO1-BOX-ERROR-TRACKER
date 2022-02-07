SRCFILES:= $(wildcard src/*.c src/*/*.c)
WINDOWFILES:= $(wildcard src/Window/*.c)
WINSRCFILES:= $(filter-out src/climain.c, $(SRCFILES))
CLISRCFILES:= $(filter-out src/main.c, $(SRCFILES))
CLISRCFILES:= $(filter-out $(WINDOWFILES), $(CLISRCFILES))

all:
	gcc -mwindows $(WINSRCFILES) -o./bin/BET
	
cli:
	gcc -static -static-libgcc -static-libstdc++ $(CLISRCFILES) -o./bin/BETcli