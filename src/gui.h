
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
	WINDOW *windowRef;
	char *content;
};

enum WINDOWS
{
	URL,
	LEFT,
	RIGHT
};

/**************** GLOBALS *****************/

struct guiWindow windows[3];
#endif