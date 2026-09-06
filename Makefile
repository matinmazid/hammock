CFLAGS := -g -Wall -Wno-unused-function
SANITIZER_FLAGS := -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer
SRCDIR := src/
OBJECTS := objects
BIN := bin
CPPFLAGS := $(shell pkg-config --cflags libcurl)
LDFLAGS := $(shell pkg-config --libs libcurl)

hammock: $(SRCDIR)gui.c  webClient.o webClientCommon.o headerMenu.o log.o
	@mkdir -p $(OBJECTS)
	@mkdir -p $(BIN)
	gcc $(CFLAGS) -g $(SRCDIR)gui.c  -o $(BIN)/hammock \
	 $(OBJECTS)/headerMenu.o \
	 -lmenu -lncurses $(OBJECTS)/webClient.o \
	 $(OBJECTS)/webClientCommon.o  \
	 $(OBJECTS)/log.o \
	 -lcurl 
	chmod u+x $(BIN)/hammock

scratch: src/scratch.c  webClient.o webClientCommon.o headerMenu.o log.o
	@mkdir -p $(OBJECTS)
	@mkdir -p $(BIN)
	gcc $(CFLAGS)  -g $(SRCDIR)scratch.c -o $(BIN)/scratch \
	 $(OBJECTS)/headerMenu.o \
	 -lmenu -lncurses $(OBJECTS)/webClient.o \
	 $(OBJECTS)/webClientCommon.o  \
	 -lcurl \
	 $(OBJECTS)/log.o
	chmod u+x $(BIN)/scratch

# -lmenu must come before -lncurses, otherwise you may get errors.
sanitize: $(SRCDIR)scratch.c webClient.o webClientCommon.o headerMenu.o log.o
	@mkdir -p $(OBJECTS)
	@mkdir -p $(BIN)
	gcc $(CFLAGS) $(SANITIZER_FLAGS) -g $(SRCDIR)scratch.c -o $(BIN)/scratch-sanitize \
	 $(OBJECTS)/headerMenu.o \
	 -lmenu -lncurses $(OBJECTS)/webClient.o \
	 $(OBJECTS)/webClientCommon.o  \
	 -lcurl \
	 $(OBJECTS)/log.o
	chmod u+x $(BIN)/scratch-sanitize

webClientCommon.o: $(SRCDIR)webClientCommon.c
	gcc $(CFLAGS) -c $(SRCDIR)webClientCommon.c -o $(OBJECTS)/webClientCommon.o

webClient.o: $(SRCDIR)webClient.c  $(SRCDIR)webClientCommon.c 
	gcc $(CFLAGS) -c $(SRCDIR)webClient.c -o $(OBJECTS)/webClient.o  
	gcc $(CFLAGS) -c $(SRCDIR)webClientCommon.c -o $(OBJECTS)/webClientCommon.o 

headerMenu.o: $(SRCDIR)headerMenu.c
	gcc $(CFLAGS) -c  $(SRCDIR)headerMenu.c -o $(OBJECTS)/headerMenu.o

log.o: $(SRCDIR)log.c $(SRCDIR)log.h
	gcc $(CFLAGS) -c  $(SRCDIR)log.c -o $(OBJECTS)/log.o

clean:
	rm -rf $(BIN)/* $(OBJECTS)/*
