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


WINDOW *drawWindow(int yStart, int xStart,int numLines, int numCols	)
{

	int y=0,x=0;

	log_debug("creating new window");
	WINDOW *windowsPtr = newwin(numLines, numCols, yStart, xStart);
	log_debug("drawing box");
	box(windowsPtr, 0, 0);
	return windowsPtr;
}

WINDOW* drawChildWindow(struct guiWindow parent)
{
	int windowYPos=0,windowXPos=0,windowsXsize=0,windowsYsize=0;
	getbegyx(parent.boarderWindowRef, windowYPos, windowXPos);
	log_debug("child widow - parent pos  y=%d, x=%d", windowYPos, windowXPos);

	getmaxyx(parent.boarderWindowRef, windowsYsize, windowsXsize);
	log_debug("child widow - parent size  y=%d, x=%d", windowsYsize, windowsXsize);

	// caclucate the max width of a child window
	int childWidth	= windowsXsize - 2; // -2 for the border

	int lengthOfContent = strlen(parent.content);
	int numberOfLines = lengthOfContent / childWidth + 3; // +1 for the last line
	WINDOW *child = newpad(numberOfLines, childWidth);

	if (parent.textWindowRef!= NULL)
	{
		destroy_win(parent.textWindowRef);
		parent.textWindowRef=NULL;
	}
	// protect against null pointer
	if (child == NULL)
	{
		log_error("Error creating child window: %s", strerror(errno));
		return NULL;
	}

	if (parent.content != NULL || strlen(parent.content) > 0)
		wprintw(child, "%s", parent.content);

	parent.padTextRows=windowYPos + numberOfLines;
	parent.padTextCols=windowXPos + childWidth ;
	// pad refresh
	// because this function knows the size of the parent window
	prefresh(child, 0, 0, windowYPos + 1, windowXPos + 1, 
		windowYPos + numberOfLines - 1, windowXPos + childWidth - 1);

	return child;
}

void redrawAllWindows(void)
{

	
	
	// -- LEFT window
	log_debug("draw left window");
	windows[LEFT].boarderWindowRef = drawWindow(3,0,LINES - 3,COLS / 2);
	wnoutrefresh(windows[LEFT].boarderWindowRef);
	windows[LEFT].textWindowRef=drawChildWindow(windows[LEFT]);

	// -- RIGHT window
	log_debug("draw right windows");
	windows[RIGHT].boarderWindowRef= drawWindow(3, COLS / 2, LINES - 3, COLS / 2);
	wnoutrefresh(windows[RIGHT].boarderWindowRef);
	windows[RIGHT].textWindowRef=drawChildWindow(windows[RIGHT]);

	// -- URL window
	log_debug("draw url window");
	windows[URL].boarderWindowRef = drawWindow(0, 0, 3, COLS);
	mvwprintw(windows[URL].boarderWindowRef, 1, 1, "%s %s", 
		methodNameList[restMethod_ptr % 5], 
		windows[URL].content);
	wnoutrefresh(windows[URL].boarderWindowRef);

	doupdate(); // refresh the screen with the new windows

	if (ACTIVE_WINDOW==URL)
		wmove(windows[ACTIVE_WINDOW].boarderWindowRef, 1, 
			strlen(windows[ACTIVE_WINDOW].content) + 
			2 + strlen(methodNameList[restMethod_ptr % 5]) );
	else
		wmove(windows[ACTIVE_WINDOW].boarderWindowRef, 
			windows[ACTIVE_WINDOW].padTextRows, 
			windows[ACTIVE_WINDOW].padTextCols);


	wrefresh(windows[ACTIVE_WINDOW].boarderWindowRef);
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
	initscr(); /* start curses mode 		*/
	raw();
	noecho();
	keypad(stdscr, true); /* i need that nifty f1 	*/

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
		else if (ch== KEY_RESIZE){
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
		else if (ch==KEY_UP && ACTIVE_WINDOW==RIGHT)
		{
			// scroll up the right window
			int scrollRtn=scroll(windows[RIGHT].textWindowRef);

			log_debug("Scrolling up the right window rtn= %d",scrollRtn);
			wrefresh(windows[RIGHT].textWindowRef);
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

	}

	endwin(); /* End curses mode		  */

	// close log file
	if (logFile != NULL)
	{
		fclose(logFile);
	}
	return 0;
}

