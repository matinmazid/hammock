CFLAGS := -g
SRCDIR := src/
export CPPFLAGS="-I/opt/homebrew/opt/curl/include"
export LDFLAGS="-L/opt/homebrew/opt/curl/lib"

hammock: $(SRCDIR)gui.c  webClient.o webClientCommon.o menu.o 
	gcc $(CFLAGS) $(SRCDIR)gui.c -o hammock \
	 menu.o \
	 -lmenu -lncurses webClient.o \
	 webClientCommon.o  \
	 -lcurl 
	chmod u+x hammock

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

webClient.o: $(SRCDIR)webClient.c  $(SRCDIR)webClientCommon.c 
	gcc $(CFLAGS) -c $(SRCDIR)webClient.c   
	gcc $(CFLAGS) -c $(SRCDIR)webClientCommon.c  

menu.o: $(SRCDIR)menu.c
	gcc $(CFLAGS) -c  $(SRCDIR)menu.c  

clean:
	rm -rf hammock webClient.o webClientCommon.o menu.o
