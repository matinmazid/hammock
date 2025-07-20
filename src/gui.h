
#ifndef __HAMMOCK_CURSES
#define __HAMMOC_CURSES 1
#include <ncurses.h>
#endif


#ifndef __HAMMOCK_GUI_H
#define __HAMMOCK_GUI_H 1

#define CTRL(x) ((x) & 0x1f)

/**************** STRUCTS *****************/
struct guiWindow
{
	WINDOW *boarderWindowRef;
	WINDOW *textWindowRef;
	char *content;
};

/**************** GLOBALS *****************/
// used to point to the active window
enum WINDOWS
{
	URL,
	LEFT,
	RIGHT
};

struct guiWindow windows[3];
#endif