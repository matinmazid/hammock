CFLAGS := -g -Wall
SRCDIR := src/
OBJECTS := objects/
BIN := bin/
export CPPFLAGS="-I/opt/homebrew/opt/curl/include"
export LDFLAGS="-L/opt/homebrew/opt/curl/lib"

hammock: $(SRCDIR)gui.c  webClient.o webClientCommon.o menu.o 
	gcc $(CFLAGS) -g $(SRCDIR)gui.c  -o $(BIN)hammock \
	 $(OBJECTS)menu.o \
	 -lmenu -lncurses $(OBJECTS)webClient.o \
	 $(OBJECTS)webClientCommon.o  \
	 -lcurl 
	chmod u+x $(BIN)hammock

# hammock:  $(SRCDIR)gui.c 
# 	gcc  $(SRCDIR)gui.c -o hammock -L/mnt/c/Users/matin/src/c/ncurses/lib -lncurses -lmenu 


run_hammock: hammock
	./hammock

gad: src/gad.c
	gcc $(CFLAGS)  -g $(SRCDIR)gad.c -o $(BIN)gad \
	 $(OBJECTS)menu.o \
	 -lmenu -lncurses $(OBJECTS)webClient.o \
	 $(OBJECTS)webClientCommon.o  \
	 -lcurl 
	chmod u+x $(BIN)gad

foo: foo.c
	gcc $(CFLAGS) foo.c   \
	-lmenu -lncurses 

webClientCommon.o: $(SRCDIR)webClientCommon.c
	gcc $(CFLAGS) -c $(SRCDIR)webClientCommon.c -o $(OBJECTS)webClientCommon.o

webClient.o: $(SRCDIR)webClient.c  $(SRCDIR)webClientCommon.c 
	gcc $(CFLAGS) -c $(SRCDIR)webClient.c -o $(OBJECTS)webClient.o  
	gcc $(CFLAGS) -c $(SRCDIR)webClientCommon.c -o $(OBJECTS)webClientCommon.o 

menu.o: $(SRCDIR)menu.c
	gcc $(CFLAGS) -c  $(SRCDIR)menu.c -o $(OBJECTS)menu.o 

clean:
	rm -rf $(BIN)* $(OBJECTS)/*
