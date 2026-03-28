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

WINDOW *drawWindow(int yStart, int xStart, int numLines, int numCols)
{

	log_debug("creating new window");
	WINDOW *windowsPtr = newwin(numLines, numCols, yStart, xStart);
	log_debug("drawing box");
	box(windowsPtr, 0, 0);
	return windowsPtr;
}

int calculateRequiredPadHeight(char *content, int childWidth)
{
	int lengthOfContent=0;
	int rowCount=0;
	while (*content != '\0')	{
		if (*content != '\n') {
			lengthOfContent++;
			if (lengthOfContent >= childWidth) {
				rowCount++;
				lengthOfContent = 0; // reset the length of content for the next row
			}
		}
		else {
			rowCount++;
			lengthOfContent = 0; // reset the length of content for the next row
		}
		content++;
	}
	return rowCount;
}
WINDOW *drawChildWindow(int parentPtr)
{
	log_debug("creating child window for parent %d", parentPtr);

	struct guiWindow parent = windows[parentPtr];
	int windowYPos = 0, windowXPos = 0, windowsXsize = 0, windowsYsize = 0;
	getbegyx(windows[parentPtr].boarderWindowRef, windowYPos, windowXPos);
	log_debug("child widow - parent pos  y=%d, x=%d", windowYPos, windowXPos);

	getmaxyx(windows[parentPtr].boarderWindowRef, windowsYsize, windowsXsize);
	log_debug("child widow - parent size  y=%d, x=%d", windowsYsize, windowsXsize);

	// caclucate the max width of a child window
	int childWidth = windowsXsize - 2; // -2 for the border

	//int lengthOfContent = strlen(parent.content);
	int numberOfLines = calculateRequiredPadHeight(parent.content, childWidth)+1;
	// int numberOfLines = lengthOfContent / childWidth + 3; // +1 for the last line
	WINDOW *child = newpad(numberOfLines, childWidth);

	if (windows[parentPtr].textWindowRef != NULL)
	{
		destroy_win(windows[parentPtr].textWindowRef);
		parent.textWindowRef = NULL;
	}
	// protect against null pointer
	if (child == NULL)
	{
		log_error("Error creating child window: %s", strerror(errno));
		return NULL;
	}

	if (windows[parentPtr].content != NULL ||
		strlen(windows[parentPtr].content) > 0)
	{
		wprintw(child, "%s", windows[parentPtr].content);
		prefresh(child, 0, 0, windowYPos + 1, windowXPos + 1,
				 windowYPos + windowsYsize - 2, windowXPos + windowsXsize - 1);
	}

	// why does this not get set right
	windows[parentPtr].padTextRows = windowYPos + numberOfLines;
	windows[parentPtr].padTextCols = windowXPos + childWidth;
	windows[parentPtr].padTextTotalLines = numberOfLines;
	windows[parentPtr].padTextTotalCols = childWidth;
	return child;
}

void redrawAllWindows(void)
{
	// -- LEFT window
	log_debug("active window is %d", ACTIVE_WINDOW);
	log_debug("draw left window");
	windows[LEFT].boarderWindowRef = drawWindow(3, 0, LINES - 3, COLS / 2);
	wnoutrefresh(windows[LEFT].boarderWindowRef);
	windows[LEFT].textWindowRef = drawChildWindow(LEFT);

	// -- RIGHT window
	log_debug("draw right windows");
	windows[RIGHT].boarderWindowRef = drawWindow(3, COLS / 2, LINES - 3, COLS / 2);
	wnoutrefresh(windows[RIGHT].boarderWindowRef);
	windows[RIGHT].textWindowRef = drawChildWindow(RIGHT);

	// -- URL window
	log_debug("draw url window");
	windows[URL].boarderWindowRef = drawWindow(0, 0, 3, COLS);
	mvwprintw(windows[URL].boarderWindowRef, 1, 1, "%s %s",
			  methodNameList[restMethod_ptr % 5],
			  windows[URL].content);
	wnoutrefresh(windows[URL].boarderWindowRef);

	// doupdate(); // refresh the screen with the new windows

	if (ACTIVE_WINDOW == URL)
	{
		wmove(windows[ACTIVE_WINDOW].boarderWindowRef, 1,
			  strlen(windows[ACTIVE_WINDOW].content) +
				  2 + strlen(methodNameList[restMethod_ptr % 5]));
		doupdate();
	}
	else
	{
		// calculate the dimensions of the pad to display the correct portion of the content

		int windowBeginYPos = 0, windowBeginXPos = 0, windowsXsize = 0, windowsYsize = 0;
		getbegyx(windows[ACTIVE_WINDOW].boarderWindowRef, windowBeginYPos, windowBeginXPos);
		getmaxyx(windows[ACTIVE_WINDOW].boarderWindowRef, windowsYsize, windowsXsize);

		if (windows[ACTIVE_WINDOW].padTextTotalLines > windowsYsize)
		{
			wmove(windows[ACTIVE_WINDOW].textWindowRef,
				  windowsYsize - 3,
				  0);
		}
		else
		{
			wmove(windows[ACTIVE_WINDOW].textWindowRef,
				  windows[ACTIVE_WINDOW].padTextRows,
				  windows[ACTIVE_WINDOW].padTextCols);
		}

		prefresh(windows[ACTIVE_WINDOW].textWindowRef, 
			windows[ACTIVE_WINDOW].scrollOffset, 0,							  // start of pad
				 windowBeginYPos + 1, windowBeginXPos + 1,								  // start of screen
				 windowBeginYPos + windowsYsize - 2, windowBeginXPos + windowsXsize - 1); // end of screen (we want to display the whole pad, so we set the end of the screen to the end of the pad)
	}

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
	char *newStr = calloc(newStrLen + 5, sizeof(char));
	memset(newStr, '\0', newStrLen + 4); // clear the memory
	windows[ACTIVE_WINDOW].content = memcpy(newStr, oldPtr, newStrLen);
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
	initscr(); /* start curses mode 		*/
	raw();
	noecho();
	keypad(stdscr, true); /* i need that nifty f1 	*/

	// set logging
	FILE *logFile = fopen("hammock.log", "a");
	if (logFile != NULL)
		log_add_fp(logFile, LOG_DEBUG);

	log_set_quiet(true); // set quiet mode to true, so we don't print to stdout

	// create a valid pointer
	windows[RIGHT].content = calloc(0, sizeof(char));
	windows[RIGHT].scrollOffset = 0;
	
	windows[LEFT].content = calloc(2, sizeof(char));
	windows[LEFT].scrollOffset = 0;

	windows[URL].content = calloc(2, sizeof(char));
	refresh();
	// draw the initial window
	redrawAllWindows();

	scrollok(windows[RIGHT].boarderWindowRef, true);
	scrollok(windows[LEFT].boarderWindowRef, true);
	scrollok(windows[URL].boarderWindowRef, true);

	// int activeWindowPtr = 0;
	// int restMethod_ptr = 0;
	// start us  off by printting out a GET instructions
	mvwprintw(windows[ACTIVE_WINDOW].boarderWindowRef, 1, 1, "%s", windows[ACTIVE_WINDOW].content);

	// eventLoop
	while (true)
	{

		redrawAllWindows();

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
		else if (ch == KEY_RESIZE)
		{
			// resize the windows
			log_debug("Resizing windows");
			redrawAllWindows();
		}
		else if (ch == CTRL('L'))
		{ // ctrl L for clear screen
			log_debug("Clearing screen");
			redrawAllWindows();
			continue;
		}
		else if (ch == CTRL('C'))
		{ // ctrl C for clear screen
			// I have to rethink this at some point might be a memory leak here
			log_debug("Clearing content");
			memset(windows[ACTIVE_WINDOW].content, '\0', strlen(windows[ACTIVE_WINDOW].content));
			redrawAllWindows();
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
			windows[ACTIVE_WINDOW].boarderWindowRef = drawWindow(0, 0, 3, COLS);
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
		else if (ch == KEY_UP &&  
			(ACTIVE_WINDOW == RIGHT || ACTIVE_WINDOW == LEFT))
		{
			if (windows[ACTIVE_WINDOW].scrollOffset <= 0) {
				continue; // we are already at the top of the window, so we can't scroll up anymore
			}

			windows[ACTIVE_WINDOW].scrollOffset -= 1;
			continue;
		}
		else if (ch == KEY_DOWN && 
			(ACTIVE_WINDOW == RIGHT || ACTIVE_WINDOW == LEFT))
		{
			// scroll down the right window
			windows[ACTIVE_WINDOW].scrollOffset += 1;
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

			windows[RIGHT].content = restResult.responseBody;
			log_trace("REST call executed: %s", restResult.responseBody);
			windows[RIGHT].content = restResult.responseBody;
			mvwprintw(windows[RIGHT].textWindowRef, 1, 2, "%s ", windows[RIGHT].content);
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
			if (ch == '\n' || (!iscntrl(ch)))
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
