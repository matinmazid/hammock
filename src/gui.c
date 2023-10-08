#include <stdlib.h>
// #include <strings.h>
#include <string.h>
#include "gui.h"
#include "webClient.h"
#include <ctype.h>
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

	windows[RIGHT].windowRef = drawRightWindow(windows[RIGHT].windowRef);
	windows[LEFT].windowRef = drawLeftWindow(windows[LEFT].windowRef);
	windows[URL].windowRef = drawUrlBox(windows[URL].windowRef);

	return;
}

void updateUserInputBox(struct guiWindow guiWindow, char userInputChar)
{

	if (guiWindow.windowRef == windows[URL].windowRef)
		mvwprintw(guiWindow.windowRef, 1, 1, "%s", guiWindow.content);
	else
		mvwprintw(guiWindow.windowRef, 1, 1, "%s", guiWindow.content);
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

	windows[RIGHT].windowRef = NULL;
	windows[LEFT].windowRef = NULL;
	windows[URL].windowRef = NULL;

	// create a valid pointer
	windows[RIGHT].content = calloc(0, sizeof(char));
	windows[LEFT].content = calloc(0, sizeof(char));
	windows[URL].content = calloc(0, sizeof("GET") + 1);
	windows[URL].content = memcpy(windows[URL].content, "GET ", sizeof("GET") + 1);
	repaintWindows();

	scrollok(windows[RIGHT].windowRef, true);
	scrollok(windows[LEFT].windowRef, true);
	scrollok(windows[URL].windowRef, true);

	int activeWindowPtr = 0;
	int urlCharIndex = 0;
	int restMethod_ptr = 0;
	// start us  off by printting out a GET instructions
	mvwprintw(windows[activeWindowPtr % 2].windowRef, 1, 1, "%s", windows[activeWindowPtr % 2].content);

	// eventLoop
	while (true)
	{
		wmove(windows[activeWindowPtr % 2].windowRef, 1, strlen(windows[activeWindowPtr % 2].content));
		wrefresh(windows[activeWindowPtr % 2].windowRef);

		ch = getch();
		if (ch == ('Q' & 0x1F))
			break;
		else if ((ch == KEY_DOWN) && (windows[URL].windowRef && windows[activeWindowPtr % 2].windowRef))
		{
			restMethod_ptr++;
			wclear(windows[URL].windowRef);
			// repaintWindows();
			windows[activeWindowPtr % 2].windowRef = drawUrlBox(windows[URL].windowRef);
			mvwprintw(windows[activeWindowPtr % 2].windowRef, 1, 1, "%s", windows[activeWindowPtr % 2].content);
		}
		else if ((ch == KEY_UP) && (windows[URL].windowRef && windows[activeWindowPtr % 2].windowRef))
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
		else if (ch == '\t')
		{
			activeWindowPtr++;

			mvwprintw(windows[activeWindowPtr % 2].windowRef, 1, 1, "%s", windows[activeWindowPtr % 2].content);
			wrefresh(windows[activeWindowPtr % 2].windowRef);
		}
		else if (ch == '\n')
		{

			struct RestResponse restResult = executeRest(windows[URL].content, restMethod_ptr % 5,
														 CommonHeaders, "{\"a\":\"b\"}");
			mvwprintw(windows[RIGHT].windowRef, 1, 2, "%s", restResult.responseBody);
			// mvwprintw(windows[URL].windowRef, 1, 1, "%s %s", methodNameList[restMethod_ptr % 5], windows[URL].content);
			wrefresh(windows[RIGHT].windowRef);
		}
		else if (ch == 127) // what is back space? just del
		{
			if (urlCharIndex > 0)
			{
				windows[URL].content[--urlCharIndex] = '\0';
				wclear(windows[URL].windowRef);
				mvwprintw(windows[URL].windowRef, 1, 1, "%s", windows[URL].content);
			}
		}
		else
		{
			unsigned int oldStrLen = strlen(windows[activeWindowPtr % 2].content);
			if (!iscntrl(ch))
			{
				char *tmp = calloc(oldStrLen + 1, sizeof(char));
				tmp[oldStrLen] = ch;
				memcpy(tmp, windows[activeWindowPtr % 2].content, oldStrLen);

				free(windows[activeWindowPtr % 2].content);
				windows[activeWindowPtr % 2].content = tmp;
			}

			updateUserInputBox(windows[activeWindowPtr % 2],ch);
			wmove(windows[activeWindowPtr % 2].windowRef, 1, oldStrLen);
		}
	}

	endwin(); /* End curses mode		  */
	return 0;
}
