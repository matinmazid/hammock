CFLAGS := -g
SRCDIR := src/

hammock: $(SRCDIR)gui.c  webClient.o webClientCommon.o menu.o 
	gcc $(CFLAGS) $(SRCDIR)gui.c -o hammock \
	 menu.o \
	 -L/mnt/c/Users/matin/src/c/ncurses/lib -lmenu -lncurses webClient.o \
	 webClientCommon.o  \
	 -lcurl -L/usr/lib/x86_64-linux-gnu/ 
	chmod u+x hammock

# hammock:  $(SRCDIR)gui.c 
# 	gcc  $(SRCDIR)gui.c -o hammock -L/mnt/c/Users/matin/src/c/ncurses/lib -lncurses -lmenu 

run_hammock: hammock
	./hammock

foo: foo.c
	gcc $(CFLAGS) foo.c  -L/mnt/c/Users/matin/src/c/ncurses/lib \
	-lncurses -lmenu

webClient.o: $(SRCDIR)webClient.c  $(SRCDIR)webClientCommon.c 
	gcc $(CFLAGS) -c $(SRCDIR)webClient.c   
	gcc $(CFLAGS) -c $(SRCDIR)webClientCommon.c  

menu.o: $(SRCDIR)menu.c
	gcc $(CFLAGS) -c  $(SRCDIR)menu.c  

clean:
	rm -rf hammock webClient.o webClientCommon.o menu.o

touch: 
	touch $(SRCDIR)gui.c
