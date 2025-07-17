CFLAGS := -g -Wall -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer -Wno-unused-function
SRCDIR := src/
OBJECTS := objects/
BIN := bin/
CPPFLAGS := $(shell pkg-config --cflags libcurl)
LDFLAGS := $(shell pkg-config --libs libcurl)

hammock: $(SRCDIR)gui.c  webClient.o webClientCommon.o menu.o log.o
	gcc $(CFLAGS) -g $(SRCDIR)gui.c  -o $(BIN)hammock \
	 $(OBJECTS)menu.o \
	 -lmenu -lncurses $(OBJECTS)webClient.o \
	 $(OBJECTS)webClientCommon.o  \
	 $(OBJECTS)log.o \
	 -lcurl 
	chmod u+x $(BIN)hammock

scratch: src/scratch.c
	gcc $(CFLAGS)  -g $(SRCDIR)scratch.c -o $(BIN)gad \
	 $(OBJECTS)menu.o \
	 -lmenu -lncurses $(OBJECTS)webClient.o \
	 $(OBJECTS)webClientCommon.o  \
	 -lcurl \
	 $(OBJECTS)log.o
	chmod u+x $(BIN)scratch

webClientCommon.o: $(SRCDIR)webClientCommon.c
	gcc $(CFLAGS) -c $(SRCDIR)webClientCommon.c -o $(OBJECTS)webClientCommon.o

webClient.o: $(SRCDIR)webClient.c  $(SRCDIR)webClientCommon.c 
	gcc $(CFLAGS) -c $(SRCDIR)webClient.c -o $(OBJECTS)webClient.o  
	gcc $(CFLAGS) -c $(SRCDIR)webClientCommon.c -o $(OBJECTS)webClientCommon.o 

menu.o: $(SRCDIR)menu.c
	gcc $(CFLAGS) -c  $(SRCDIR)menu.c -o $(OBJECTS)menu.o

log.o: $(SRCDIR)log.c $(SRCDIR)log.h
	gcc $(CFLAGS) -c  $(SRCDIR)log.c -o $(OBJECTS)log.o

clean:
	rm -rf $(BIN)* $(OBJECTS)/*
