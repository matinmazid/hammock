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

extern void  doMenu();

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
	newStrLen = strlen(windows[activeWindowPtr % 2].content) + 1;
	// - there is a memmory leak here
	windows[activeWindowPtr % 2].content = calloc(newStrLen, 1);
	windows[activeWindowPtr % 2].content = memcpy(windows[activeWindowPtr % 2].content, oldPtr, newStrLen - 1);
	windows[activeWindowPtr % 2].content[newStrLen - 1] = newChar;
	windows[activeWindowPtr % 2].content[newStrLen] = '\0';
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

	refresh();
	windows[RIGHT].windowRef = NULL;
	windows[LEFT].windowRef = NULL;
	windows[URL].windowRef = NULL;

	// create a valid pointer
	windows[RIGHT].content = calloc(0, sizeof(char));
	windows[LEFT].content = calloc(0, sizeof(char));
	windows[URL].content = calloc(1, sizeof(char));
	// windows[URL].content = memcpy(windows[URL].content, "GET ", sizeof("GET") + 1);
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
			mvwprintw(windows[activeWindowPtr % 2].windowRef, 1, 1, "%s %s", methodNameList[restMethod_ptr % 4], windows[activeWindowPtr % 2].content);
		else
			mvwprintw(windows[activeWindowPtr % 2].windowRef, 1, 1, "%s", windows[activeWindowPtr % 2].content);

		box(windows[activeWindowPtr % 2].windowRef, 0, 0);
		wrefresh(windows[activeWindowPtr % 2].windowRef);

		ch = getch();

		if (ch == CTRL('Q'))
			break;
		else if (ch == ERR)
		{
			continue;
		}
		// else if (ch== 263){ // ctrl H for Headers
		else if (ch== CTRL('H')){ // ctrl H for Headers

			doMenu();
			// post_menu(headerMenu);
			box(windows[LEFT].windowRef, 0, 0);
			box(windows[RIGHT].windowRef, 0, 0);
			wrefresh(windows[LEFT].windowRef);
			wrefresh(windows[RIGHT].windowRef);
		}
		else if ((ch == KEY_DOWN) && (activeWindowPtr % 2==URL)) // cycle down
		{

				restMethod_ptr++;
				wclear(windows[URL].windowRef);
				// repaintWindows();
				windows[activeWindowPtr % 2].windowRef = drawUrlBox(windows[URL].windowRef);
				mvwprintw(windows[activeWindowPtr % 2].windowRef, 1, 1, "%s", windows[activeWindowPtr % 2].content);
		}
		else if ((ch == KEY_UP) && (activeWindowPtr % 2==URL)) // cycle up
		{
				// 0 1 2 3 4
				if (restMethod_ptr == 0)
					restMethod_ptr = 4; // Dont forget to change this when you add a http method
				else
					--restMethod_ptr;

				// split out the method and throw it away;

				wclear(windows[URL].windowRef);
				// repaintWindows();
				windows[URL].windowRef = drawUrlBox(windows[URL].windowRef);
				mvwprintw(windows[URL].windowRef, 1, 1, "%s", windows[URL].content);
		}
		else if (ch == CTRL('\t'))
		{
			activeWindowPtr++;

			mvwprintw(windows[activeWindowPtr % 2].windowRef, 1, 1, "%s", windows[activeWindowPtr % 2].content);
			wrefresh(windows[activeWindowPtr % 2].windowRef);
		}
		else if ((ch == CTRL('e'))||(ch=='\n')&& activeWindowPtr%2==URL)
		{

			// matin look here
			struct RestResponse restResult = executeRest(windows[URL].content, restMethod_ptr % 5,
														 ContentTypes, windows[LEFT].content);
			mvwprintw(windows[RIGHT].windowRef, 1, 2, "%s", restResult.responseBody);
			mvwprintw(windows[LEFT].windowRef,1,1,"%s", windows[LEFT].content);
			wrefresh(windows[RIGHT].windowRef);
		}
		else if (ch == 127) // what is back space? just del
		{
			unsigned int existingLength = strlen(windows[activeWindowPtr % 2].content);
			if (existingLength > 0)
			{
				char *tmp = calloc(existingLength - 1, 1);
				memcpy(tmp, windows[activeWindowPtr % 2].content, existingLength - 1);
				free(windows[activeWindowPtr % 2].content);
				windows[activeWindowPtr % 2].content = tmp;
				wclear(windows[activeWindowPtr % 2].windowRef);
				mvwprintw(windows[URL].windowRef, 1, 1, "%s %s", methodNameList[restMethod_ptr % 4], windows[URL].content);
			}
		}
		else
		{
			// if the character is not a control character or a newline	
			// and we are not in the URL window
			if (!iscntrl(ch) || ch != '\n')
			{
				appendChar(ch, activeWindowPtr);
			}
		}
	}

	endwin(); /* End curses mode		  */
	return 0;
}
