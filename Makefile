CFLAGS := -g
SRCDIR := src/

hammock: $(SRCDIR)gui.c  webClient.o
	gcc $(CFLAGS) $(SRCDIR)gui.c -o hammock webClient.o  -L/usr/lib -lncurses  -lcurl -L/usr/lib/x86_64-linux-gnu/ 
	chmod u+x hammock

run_hammock: hammock
	./hammock


foo: foo.c
	gcc $(CFLAGS) foo.c  -L/usr/lib -lncurses

webClient.o: $(SRCDIR)webClient.c
	gcc $(CFLAGS) -c $(SRCDIR)webClient.c   


clean:
	rm -r hammock webClient.o
touch: 
	touch $(SRCDIR)gui.c
