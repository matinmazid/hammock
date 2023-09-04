#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
/**************** STRUCTS *****************/
struct point
{
	int x;
	int y;
};

FILE *openFile(char *fileName)
{

	FILE *ptr = fopen(fileName, "r");
	if (ptr == NULL)
	{
		perror("cant open file");
		exit(-1);
	}

	return ptr;
}

/**************** FUNCTIONS *****************/

void myLoop(int max_x, int max_y, FILE *pfile)
{
	fprintf(stderr, "good bad world\n");
	printw("kdkd %d %d\n", max_x, max_y);
	int ch;
	struct point myPoint;
	myPoint.x = 0;
	myPoint.y = 0;

	int x, y, prev;
	while ((ch = fgetc(pfile)) != EOF)
	{
		getyx(stdscr, y, x);
		if (y == (max_x - 1))
		{
			printw("press any key ");
			getch();
			move(0, 0);
		}

		if (prev == '/' && ch == '*')
		{
			attron(COLOR_GREEN);
			getyx(stdscr, y, x);
			move(y, x - 1);
			printw("%c%c", '/', ch);
		}
		else
			printw("%c", ch);
		refresh();
		if (prev == '*' && ch == '/')
			attroff(COLOR_GREEN);
		prev=ch;
	}
}

/****************************** MAIN **********************/
int main()
{

init_color(20,20,20,20);

	FILE *ptr = openFile("src/gui.c");
	initscr(); /* Start curses mode			  */
	raw();
	keypad(stdscr, TRUE);
	noecho();
	printw("Type any character to see it in bold\n"); /* Print Hello World		  */
	int maxX, maxY;
	getmaxyx(stdscr, maxX, maxY);
	myLoop(maxX, maxY, ptr);
	endwin(); /* End curses mode		  */

	return 0;
}