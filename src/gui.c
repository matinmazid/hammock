#include <stdio.h>
#include <stdlib.h>
// #include <strings.h>
#include <string.h>
#include "gui.h"
#include "webClient.h"

#include <curl/curl.h>
extern struct guiWindow windows[3];
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
	initscr(); /* Start curses mode 		*/
	raw();
	noecho();
	keypad(stdscr, TRUE); /* I need that nifty F1 	*/
	printw("Press F1 to exit");
	refresh();

	windows[RIGHT].widowRef = NULL;
	windows[LEFT].widowRef = NULL;
	windows[URL].widowRef = NULL;

	repaintWindows();

	scrollok(windows[RIGHT].widowRef, true);
	scrollok(windows[LEFT].widowRef, true);
	scrollok(windows[URL].widowRef, true);

	char url[512];
	char requestBody[1024];
	bzero(url, sizeof(url));
	bzero(requestBody, sizeof(requestBody));
	char *methodList[]={"GET","POST","PUT","DELETE","PATCH"};

	int c = 0;
	int methodLocation=0;
	wmove(windows[URL].widowRef, 1, 1);
	wrefresh(windows[URL].widowRef);
	while (true)
	{
		ch = getch();
		if (ch == '\t')
			break;
		else if (ch == KEY_DOWN){
			repaintWindows();
			wclear(windows[URL].widowRef);
			mvwprintw(windows[URL].widowRef, 1, 1, "%s", methodList[methodLocation%5]);

			wmove(windows[URL].widowRef, 1,strlen(methodList[methodLocation%5])+1 );
			wrefresh(windows[URL].widowRef);
			methodLocation++;
		}
		else if (ch == KEY_F(1)){
			noecho();
			bzero(url,sizeof(url));
			repaintWindows();
			echo();
		}
		else if (ch == '\n')
		{
		
			// mvwscanw(windows[URL].widowRef, 1, 1, "%s", url);
			// struct RestRequest restResult = doGet(url);
			mvwprintw(windows[RIGHT].widowRef, 1, 2, ">>>%s<<<", "some-result");
			mvwprintw(windows[LEFT].widowRef, 1, 2, "--%s--", url);
			mvwprintw(windows[URL].widowRef, 1, 1, "%s %s", methodList[methodLocation], url);
		}
		else if (ch ==KEY_BACKSPACE){
			url[--c]='\0';
			wdelch(windows[URL].widowRef);
			mvwprintw(windows[URL].widowRef, 1, 1, "%s %s", methodList[methodLocation],url);
		}
		else
		{
			url[c++] = ch;
			mvwprintw(windows[URL].widowRef, 1, 1, "%s %s", methodList[methodLocation],url);
		}

		wrefresh(windows[RIGHT].widowRef);
		wrefresh(windows[LEFT].widowRef);
		wrefresh(windows[URL].widowRef);
		// mvwscanw(windows[LEFT].widowRef, 1, 1, "%s", requestBody);
	}

	endwin(); /* End curses mode		  */
	return 0;
}
