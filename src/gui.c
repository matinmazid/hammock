#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
void destroy_win(WINDOW *local_win);

/**************** STRUCTS *****************/
struct guiWindow
{
	int x;
	int y;
	WINDOW *widowRef;
};

/**************** FUNCTIONS *****************/

WINDOW *drawRightWindow(void)
{

	WINDOW *windowsPtr = newwin(LINES, COLS / 2, 0, 0);
	printw("hell world %d %d", (COLS / 2), (COLS / 2));
	box(windowsPtr, 0, 0);
	wrefresh(windowsPtr);

	return windowsPtr;
}

void destroy_win(WINDOW *local_win)
{
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}
/****************************** MAIN **********************/
int main()
{
	int startx, starty, width, height;
	int ch;
	printf("hell world");
	struct guiWindow inputBox;
	struct guiWindow outputBox;
	initscr();			  /* Start curses mode 		*/
	cbreak();			  /* Line buffering disabled, Pass on
					 * everty thing to me 		*/
	keypad(stdscr, TRUE); /* I need that nifty F1 	*/
	refresh();
	inputBox.widowRef = drawRightWindow();
	while ((ch = getch()) != KEY_F(1))
	{
		inputBox.widowRef = drawRightWindow();
	}

	endwin(); /* End curses mode		  */
	return 0;
}

