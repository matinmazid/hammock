CFLAGS := -g  -Wall -Wno-unused-function
SANITIZER_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer
SRCDIR := src/
OBJECTS := objects
BIN := bin
CPPFLAGS := $(shell pkg-config --cflags libcurl jansson ncurses)
LDFLAGS := $(shell pkg-config --libs libcurl jansson ncurses)
CC:=$(shell brew --prefix llvm)/bin/clang

ifeq ($(SANITIZE),1)
	CFLAGS += $(SANITIZER_FLAGS)
endif

hammock: $(SRCDIR)gui.c  webClient.o webClientCommon.o headerMenu.o log.o
	@mkdir -p $(OBJECTS)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -g $(SRCDIR)gui.c  -o $(BIN)/hammock \
	 $(OBJECTS)/headerMenu.o \
	 -lmenu -lncurses $(OBJECTS)/webClient.o \
	 $(OBJECTS)/webClientCommon.o  \
	 $(OBJECTS)/log.o \
	 -lcurl 
	chmod u+x $(BIN)/hammock

scratch: src/scratch.c  webClient.o webClientCommon.o headerMenu.o log.o
	@mkdir -p $(OBJECTS)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS)  -g $(SRCDIR)scratch.c -o $(BIN)/scratch \
	 $(OBJECTS)/headerMenu.o \
	 -lmenu -lncurses $(OBJECTS)/webClient.o \
	 $(OBJECTS)/webClientCommon.o  \
	 -lcurl \
	 $(OBJECTS)/log.o
	chmod u+x $(BIN)/scratch


webClientCommon.o: $(SRCDIR)webClientCommon.c
	$(CC) $(CFLAGS) -c $(SRCDIR)webClientCommon.c -o $(OBJECTS)/webClientCommon.o

webClient.o: $(SRCDIR)webClient.c  $(SRCDIR)webClientCommon.c 
	$(CC) $(CFLAGS) -c $(SRCDIR)webClient.c -o $(OBJECTS)/webClient.o  
	$(CC) $(CFLAGS) -c $(SRCDIR)webClientCommon.c -o $(OBJECTS)/webClientCommon.o 

headerMenu.o: $(SRCDIR)headerMenu.c
	$(CC) $(CFLAGS) -c  $(SRCDIR)headerMenu.c -o $(OBJECTS)/headerMenu.o

log.o: $(SRCDIR)log.c $(SRCDIR)log.h
	$(CC) $(CFLAGS) -c  $(SRCDIR)log.c -o $(OBJECTS)/log.o

# fileHandler.o: $(SRCDIR)fileHandler.c $(SRCDIR)fileHandler.h
# 	$(CC) $(CFLAGS) -c  $(SRCDIR)fileHandler.c -o $(OBJECTS)/fileHandler.o
clean:
	rm -rf $(BIN)/* $(OBJECTS)/*
