CFLAGS := -g
SRCDIR := src/

hammock: $(SRCDIR)gui.c  webClient.o webClientCommon.o menu.o
	gcc $(CFLAGS) $(SRCDIR)gui.c -o hammock menu.o webClient.o  webClientCommon.o -L/usr/lib -lncurses -lmenu  -lcurl -L/usr/lib/x86_64-linux-gnu/ 
	chmod u+x hammock

run_hammock: hammock
	./hammock


foo: foo.c
	gcc $(CFLAGS) foo.c  -L/usr/lib -lncurses

webClient.o: $(SRCDIR)webClient.c  $(SRCDIR)webClientCommon.c 
	gcc $(CFLAGS) -c $(SRCDIR)webClient.c   
	gcc $(CFLAGS) -c $(SRCDIR)webClientCommon.c  

menu.o: $(SRCDIR)menu.c
	gcc $(CFLAGS) -c $(SRCDIR)menu.c

clean:
	rm -rf hammock webClient.o webClientCommon.o menu.o

touch: 
	touch $(SRCDIR)gui.c
