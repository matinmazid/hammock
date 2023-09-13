#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "gui.h"
#include "webClient.h"


#include <curl/curl.h>
extern struct guiWindow windows[3] ;
void destroy_win(WINDOW *local_win);

/**************** FUNCTIONS *****************/

WINDOW *drawRightWindow(WINDOW *windowPtr)
{

	destroy_win(windowPtr);
	WINDOW *windowsPtr = newwin(LINES - 3, (COLS / 2), 3, COLS / 2);
	box(windowsPtr, 0, 0);
	wrefresh(windowsPtr);

	return windowsPtr;
}

WINDOW *drawUrlBox(WINDOW *windowPtr)
{

	destroy_win(windowPtr);
	WINDOW *windowsPtr = newwin(3, COLS, 0, 0);
	box(windowsPtr, 0, 0);
	wrefresh(windowsPtr);

	return windowsPtr;
}

WINDOW *drawLeftWindow(WINDOW *windowPtr)
{

	destroy_win(windowPtr);
	WINDOW *windowsPtr = newwin(LINES - 3, (COLS / 2), 3, 0);
	box(windowsPtr, 0, 0);
	wrefresh(windowsPtr);
	
	return windowsPtr;
}

void repaintWindows(void)
{

	windows[RIGHT].widowRef = drawRightWindow(windows[RIGHT].widowRef);
	windows[LEFT].widowRef = drawLeftWindow(windows[LEFT].widowRef);
	windows[URL].widowRef = drawUrlBox(windows[URL].widowRef);

	return;
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
	int ch;
	initscr();			  /* Start curses mode 		*/
	cbreak();			  /* Line buffering disabled, Pass on
					 		* everty thing to me 		*/
	keypad(stdscr, TRUE); /* I need that nifty F1 	*/
	printw("Press F1 to exit");
	refresh();

	windows[RIGHT].widowRef = NULL;
	windows[LEFT].widowRef = NULL;
	windows[URL].widowRef = NULL;

	repaintWindows();

	char url[512];
	char requestBody[1024];
	bzero(url, sizeof(url));
	bzero(requestBody,sizeof(requestBody));

	do {
		mvwscanw(windows[URL].widowRef, 1, 1, "%s", url);
		mvwscanw(windows[LEFT].widowRef, 1, 1, "%s", requestBody);

		struct RestRequest restResult= doGet(url);
		mvwprintw(windows[RIGHT].widowRef, 1, 2, ">>>%s<<<", restResult.responseBody );
		mvwprintw(windows[LEFT].widowRef, 1, 2, "--%s--",requestBody );
		mvwprintw(windows[URL].widowRef, 1, 1, "%s %s","GET", url );

		wrefresh(windows[RIGHT].widowRef);
		wrefresh(windows[LEFT].widowRef);
		wrefresh(windows[URL].widowRef);
	}
	while ((ch = getch()) != '\t');

	endwin(); /* End curses mode		  */
	return 0;
}
