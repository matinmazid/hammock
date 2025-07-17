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
#include <curses.h>
#include <stdio.h>
#include "log.h"
// tmp
#include<time.h>
extern void doMenu();

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

	windows[RIGHT].windowRef = drawRightWindow(windows[RIGHT].windowRef);
	windows[LEFT].windowRef = drawLeftWindow(windows[LEFT].windowRef);
	windows[URL].windowRef = drawUrlBox(windows[URL].windowRef);

	return;
}

void appendChar(int newChar, int activeWindowPtr)
{

	int newStrLen;
	char *oldPtr = windows[activeWindowPtr % 2].content;
	newStrLen = strlen(windows[activeWindowPtr % 2].content); 

	// we are adding wasting 5 bytes here to make sure we dont buffer overrun
	// I'll fix it later
	char * newStr = calloc(newStrLen+5, sizeof(char));
	memset(newStr, '\0', newStrLen + 4); // clear the memory
	windows[activeWindowPtr % 2].content = memcpy(newStr, oldPtr, newStrLen );
	windows[activeWindowPtr % 2].content[newStrLen] = (char)newChar;
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

	refresh();
	windows[RIGHT].windowRef = NULL;
	windows[LEFT].windowRef = NULL;
	windows[URL].windowRef = NULL;

	// create a valid pointer
	windows[RIGHT].content = calloc(0, sizeof(char));
	windows[LEFT].content = calloc(2, sizeof(char));
	windows[URL].content = calloc(2, sizeof(char));
	// draw the initial window
	repaintWindows();

	scrollok(windows[RIGHT].windowRef, true);
	scrollok(windows[LEFT].windowRef, true);
	scrollok(windows[URL].windowRef, true);

	int activeWindowPtr = 0;
	int restMethod_ptr = 0;
	// start us  off by printting out a GET instructions
	mvwprintw(windows[activeWindowPtr % 2].windowRef, 1, 1, "%s", windows[activeWindowPtr % 2].content);

	// eventLoop
	while (true)
	{

		wclear(windows[activeWindowPtr % 2].windowRef);
		if (windows[activeWindowPtr % 2].windowRef == windows[URL].windowRef)
		{
			mvwprintw(windows[activeWindowPtr % 2].windowRef, 1, 1, "%s %s", 
				methodNameList[restMethod_ptr % 5], 
				windows[activeWindowPtr % 2].content);
		}
		else
		{
			mvwprintw(windows[activeWindowPtr % 2].windowRef, 1, 1, "%s", 
				windows[activeWindowPtr % 2].content);
		}

		box(windows[activeWindowPtr % 2].windowRef, 0, 0);
		wrefresh(windows[activeWindowPtr % 2].windowRef);

		ch = getch();

		if (ch == CTRL('Q'))
			// exit the program loop
			break;
		else if (ch == ERR)
		{
			// skip the error and keep going
			continue;
			
		}
		else if (ch== KEY_RESIZE){
			// resize the windows
			log_debug("Resizing windows");
			repaintWindows();
			continue;
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
			memset(windows[activeWindowPtr % 2].content, '\0', strlen(windows[activeWindowPtr % 2].content));
			wrefresh(windows[activeWindowPtr % 2].windowRef);
		}
		else if (ch == CTRL('H'))
		{ // ctrl H for Headers

			doMenu();
			// post_menu(headerMenu);
			box(windows[LEFT].windowRef, 0, 0);
			box(windows[RIGHT].windowRef, 0, 0);
			wrefresh(windows[LEFT].windowRef);
			wrefresh(windows[RIGHT].windowRef);
		}
		else if ((ch == KEY_DOWN) && (activeWindowPtr % 2 == URL)) // cycle down
		{

			restMethod_ptr++;
			windows[activeWindowPtr % 2].windowRef = drawUrlBox(windows[URL].windowRef);
		}
		else if ((ch == KEY_UP) && (activeWindowPtr % 2 == URL)) // cycle up
		{
			// 0 1 2 3 4
			if (restMethod_ptr == 0)
			{
				restMethod_ptr = 4; // cycle back to last method
			}
			else
				restMethod_ptr--; // this will cycle through 0-4

		}
		else if (ch==KEY_UP)
		{
			// windows[activeWindowPtr % 2].windowRef ;
			// int row=0,col=0;
			// getyx(windows[activeWindowPtr % 2].windowRef, row, col);

			continue;
		}
		else if (ch == CTRL('\t')) // switch window
		{
			activeWindowPtr++;

		}
		else if ((ch == CTRL('e')) || (ch == '\n' && activeWindowPtr % 2 == URL))
		{
			log_trace("Executing REST call: %s", windows[URL].content);
			struct RestResponse restResult = executeRest(windows[URL].content, restMethod_ptr % 5,
					ContentTypes, windows[LEFT].content);
			
			log_trace("REST call executed: %s", restResult.responseBody);
			mvwprintw(windows[RIGHT].windowRef, 1, 2, "%s  ", restResult.responseBody );
			wrefresh(windows[RIGHT].windowRef);
		}
		else if (ch == 127) // what is back space? just del
		{
			unsigned int existingLength = strlen(windows[activeWindowPtr % 2].content);
			if (existingLength > 0)
			{
				windows[activeWindowPtr % 2].content[existingLength - 1] = '\0';	
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
	}

	endwin(); /* End curses mode		  */

	// close log file
	if (logFile != NULL)
	{
		fclose(logFile);
	}
	return 0;
}

