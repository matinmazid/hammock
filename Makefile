CFLAGS := -g
SRCDIR := src/
OBJECTS := objects/
BIN := bin/
export CPPFLAGS="-I/opt/homebrew/opt/curl/include"
export LDFLAGS="-L/opt/homebrew/opt/curl/lib"

hammock: $(SRCDIR)gui.c  webClient.o webClientCommon.o menu.o 
	gcc $(CFLAGS) $(SRCDIR)gui.c -o $(BIN)hammock \
	 $(OBJECTS)menu.o \
	 -lmenu -lncurses $(OBJECTS)webClient.o \
	 $(OBJECTS)webClientCommon.o  \
	 -lcurl 
	chmod u+x $(BIN)hammock

# hammock:  $(SRCDIR)gui.c 
# 	gcc  $(SRCDIR)gui.c -o hammock -L/mnt/c/Users/matin/src/c/ncurses/lib -lncurses -lmenu 


run_hammock: hammock
	./hammock

debug_hammock: hammock
	gdb -p `ps -ef|grep hammock |grep -v grep |sed 's/\s\+/ /g'| cut -d' ' -f2`

kill_hammock:
	kill `ps -ef|grep hammock |grep -v grep |sed 's/\s\+/ /g'| cut -d' ' -f2`

gad: gad.c
	gcc $(CFLAGS) gad.c   \
	-lmenu -lncurses 

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
