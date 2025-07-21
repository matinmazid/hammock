#include <stdlib.h>
// #include <strings.h>
#include <string.h>

#ifndef __HAMMOCK_GUI_H
#include "gui.h"
#endif

#include "webClient.h"
#include <ctype.h>
#include <curl/curl.h>
#include <menu.h>
#include "menu.h"
#include <errno.h>
#include <curses.h>
#include <stdio.h>
#include "log.h"
#define ACTIVE_WINDOW (activeWindowPtr % 3)

extern void doMenu();

extern struct guiWindow windows[3];


	static int activeWindowPtr = 0;
	static int restMethod_ptr = 0;
void destroy_win(WINDOW *local_win);

/**************** FUNCTIONS *****************/

WINDOW *drawRightWindow()
{
	int y=0,x=0;
	destroy_win(windows[RIGHT].boarderWindowRef);
	WINDOW *windowsPtr = newwin(LINES - 3, (COLS / 2), 3, COLS / 2);
	getmaxyx(windowsPtr, y, x);
	log_debug("Right window dimensions, y=%d, x=%d", y, x);
	box(windowsPtr, 0, 0);
	return windowsPtr;
}

WINDOW *drawUrlBox()
{

	int y=0,x=0;
	destroy_win(windows[URL].boarderWindowRef);
	WINDOW *windowsPtr = newwin(3, COLS, 0, 0);
	getmaxyx(windowsPtr, y, x);
	log_debug("url window dimensions, y=%d, x=%d", y, x);
	box(windowsPtr, 0, 0);
	return windowsPtr;
}

WINDOW *drawLeftWindow()
{

	int y=0,x=0;
	destroy_win(windows[LEFT].boarderWindowRef);
	WINDOW *windowsPtr = newwin(LINES - 3, (COLS / 2), 3, 0);
	getmaxyx(windowsPtr, y, x);
	log_debug("left window dimensions, y=%d, x=%d", y, x);
	box(windowsPtr, 0, 0);
	return windowsPtr;
}

WINDOW * createChildWindow(WINDOW *parent)
{
	int y=0,x=0;
	getmaxyx(parent, y, x);
	log_debug("parent dim , y=%d, x=%d", y, x);

	WINDOW *child = derwin(parent, y-2, x-2, 1, 1);
	if (child == NULL)
	{
		log_error("Error creating child window: %s", strerror(errno));
		return NULL;
	}
	getmaxyx(child, y, x);
	log_debug("child dim , y=%d, x=%d", y, x);
	return child;
}

void repaintWindows(void)
{

	// -- RIGHT window
	windows[RIGHT].boarderWindowRef = drawRightWindow();
	windows[RIGHT].textWindowRef =createChildWindow(windows[RIGHT].boarderWindowRef);
	mvwprintw(windows[RIGHT].textWindowRef, 1, 1, "%s", 
		windows[RIGHT].content);
	wnoutrefresh(windows[RIGHT].boarderWindowRef);
	wnoutrefresh(windows[RIGHT].textWindowRef);

	// -- LEFT window
	windows[LEFT].boarderWindowRef = drawLeftWindow();
	windows[LEFT].textWindowRef=createChildWindow(windows[LEFT].boarderWindowRef);
	mvwprintw(windows[LEFT].textWindowRef, 1, 1, "%s", 
		windows[LEFT].content);

	wnoutrefresh(windows[LEFT].boarderWindowRef);
	wnoutrefresh(windows[LEFT].textWindowRef);

	// -- URL window
	windows[URL].boarderWindowRef = drawUrlBox();
	mvwprintw(windows[URL].boarderWindowRef, 1, 1, "%s %s", 
		methodNameList[restMethod_ptr % 5], 
		windows[URL].content);
	wnoutrefresh(windows[URL].boarderWindowRef);

	doupdate(); // refresh the screen with the new windows
	return;
}

void appendChar(int newChar, int activeWindowPtr)
{

	int newStrLen;
	char *oldPtr = windows[ACTIVE_WINDOW].content;
	newStrLen = strlen(windows[ACTIVE_WINDOW].content); 

	// we are adding wasting 5 bytes here to make sure we dont buffer overrun
	// I'll fix it later
	char * newStr = calloc(newStrLen+5, sizeof(char));
	memset(newStr, '\0', newStrLen + 4); // clear the memory
	windows[ACTIVE_WINDOW].content = memcpy(newStr, oldPtr, newStrLen );
	windows[ACTIVE_WINDOW].content[newStrLen] = (char)newChar;
	free(oldPtr); // free the old pointer
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
	// set up logging
	int ch;
	initscr(); /* Start curses mode 		*/
	raw();
	noecho();
	keypad(stdscr, TRUE); /* I need that nifty F1 	*/

	// set logging
	FILE *logFile = fopen("hammock.log", "a");
	if (logFile != NULL)
		log_add_fp(logFile,LOG_DEBUG);

	log_set_quiet(true); // set quiet mode to true, so we don't print to stdout

	// create a valid pointer
	windows[RIGHT].content = calloc(0, sizeof(char));
	windows[LEFT].content = calloc(2, sizeof(char));
	windows[URL].content = calloc(2, sizeof(char));
	refresh();
	// draw the initial window
	repaintWindows();

	scrollok(windows[RIGHT].boarderWindowRef, true);
	scrollok(windows[LEFT].boarderWindowRef, true);
	scrollok(windows[URL].boarderWindowRef, true);

	// int activeWindowPtr = 0;
	// int restMethod_ptr = 0;
	// start us  off by printting out a GET instructions
	mvwprintw(windows[ACTIVE_WINDOW].boarderWindowRef, 1, 1, "%s", windows[ACTIVE_WINDOW].content);

	// log current dimenions
	log_debug("Current terminal dimensions, y=%d, x=%d", LINES, COLS);
	// eventLoop
	while (true)
	{

		wclear(windows[ACTIVE_WINDOW].boarderWindowRef);
		if (windows[ACTIVE_WINDOW].boarderWindowRef == windows[URL].boarderWindowRef)
		{
			mvwprintw(windows[ACTIVE_WINDOW].boarderWindowRef, 1, 1, "%s %s", 
				methodNameList[restMethod_ptr % 5], 
				windows[ACTIVE_WINDOW].content);
		}
		else
		{
			mvwprintw(windows[ACTIVE_WINDOW].textWindowRef, 1, 1, "%s", 
				windows[ACTIVE_WINDOW].content);
		}

		repaintWindows();
		wrefresh(windows[ACTIVE_WINDOW].textWindowRef);
		ch = getch();

		if (ch == CTRL('Q'))
			// exit the program loop
			break;
		else if (ch == ERR)
		{
			log_error("Error reading input: %s", strerror(errno));
			// skip the error and keep going
			continue;
			
		}
		else if (ch== KEY_RESIZE){
			// resize the windows
			log_debug("Resizing windows");
			repaintWindows();
		}
		else if (ch == CTRL('L'))
		{ // ctrl L for clear screen
			log_debug("Clearing screen");
			repaintWindows();
			continue;
		}
		else if (ch == CTRL('C'))
		{ // ctrl C for clear screen
			log_debug("Clearing content");
			memset(windows[ACTIVE_WINDOW].content, '\0', strlen(windows[ACTIVE_WINDOW].content));
			wrefresh(windows[ACTIVE_WINDOW].boarderWindowRef);
		}
		else if (ch == CTRL('H'))
		{ // ctrl H for Headers

			doMenu();
			// post_menu(headerMenu);
			box(windows[LEFT].boarderWindowRef, 0, 0);
			box(windows[RIGHT].boarderWindowRef, 0, 0);
			wrefresh(windows[LEFT].boarderWindowRef);
			wrefresh(windows[RIGHT].boarderWindowRef);
		}
		else if ((ch == KEY_DOWN) && (ACTIVE_WINDOW == URL)) // cycle down
		{
			restMethod_ptr++;
			windows[ACTIVE_WINDOW].boarderWindowRef = drawUrlBox();
		}
		else if ((ch == KEY_UP) && (ACTIVE_WINDOW == URL)) // cycle up
		{
			// 0 1 2 3 4
			if (restMethod_ptr == 0)
			{
				restMethod_ptr = 4; // cycle back to last method
			}
			else
				restMethod_ptr--; // this will cycle through 0-4

		}
		else if (ch==KEY_UP && ACTIVE_WINDOW==RIGHT)
		{
			// windows[ACTIVE_WINDOW].boarderWindowRef ;
			// int row=0,col=0;
			// getyx(windows[ACTIVE_WINDOW].boarderWindowRef, row, col);

			continue;
		}
		else if (ch == CTRL('\t')) // switch window
		{
			activeWindowPtr++;

		}
		else if ((ch == CTRL('e')) || (ch == '\n' && ACTIVE_WINDOW == URL))
		{
			log_trace("Executing REST call: %s", windows[URL].content);
			struct RestResponse restResult = executeRest(windows[URL].content, restMethod_ptr % 5,
					ContentTypes, windows[LEFT].content);
		
			windows[RIGHT].content=restResult.responseBody;
			log_trace("REST call executed: %s", restResult.responseBody);
			windows[RIGHT].content=restResult.responseBody; 
			mvwprintw(windows[RIGHT].textWindowRef, 1, 2, "%s ", windows[RIGHT].content );
			wrefresh(windows[RIGHT].textWindowRef);
		
		}
		else if (ch == 127) // what is back space? just del
		{
			unsigned int existingLength = strlen(windows[ACTIVE_WINDOW].content);
			if (existingLength > 0)
			{
				windows[ACTIVE_WINDOW].content[existingLength - 1] = '\0';	
			}
		}
		else
		{
			// if the character is not a control character or a newline
			// and we are not in the URL window
			if (ch=='\n'||(!iscntrl(ch)))
			{
				appendChar(ch, activeWindowPtr);
			}
		}

		int x=0,y=0;
		getyx(windows[ACTIVE_WINDOW].boarderWindowRef, y, x);
		log_debug("Current active window co ord , y=%d, x=%d char=%c",x,y,(char)ch);
	}

	endwin(); /* End curses mode		  */

	// close log file
	if (logFile != NULL)
	{
		fclose(logFile);
	}
	return 0;
}

