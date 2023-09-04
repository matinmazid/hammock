CFLAGS := -g
SRCDIR := src/

hammock: $(SRCDIR)gui.c
	gcc $(CFLAGS) $(SRCDIR)gui.c -o hammock -L/usr/lib -lncurses
	chmod u+x hammock

run_new: hammock
	./hammock

clean:
	rm -r hammock
touch: 
	touch $(SRCDIR)gui.c
