CFLAGS := -g
SRCDIR := src/

hammock: $(SRCDIR)gui.c
	gcc $(CFLAGS) $(SRCDIR)gui.c -o hammock -L/usr/lib -lncurses
	chmod u+x hammock

run_hammock: hammock
	./hammock


foo: foo.c
	gcc $(CFLAGS) foo.c  -L/usr/lib -lncurses

rest: $(SRCDIR)webClient.c
	gcc $(CFLAGS) $(SRCDIR)webClient.c -lcurl -L/usr/lib/x86_64-linux-gnu/ -o webClient

run_rest: rest	
	./webClient

clean:
	rm -r hammock
touch: 
	touch $(SRCDIR)gui.c
