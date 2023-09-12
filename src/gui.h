
#ifndef __HAMMOCK_CURSES
#define __HAMMOC_CURSES 1
#include <ncurses.h>
#endif


#ifndef __HAMMOCK_GUI_H
#define __HAMMOCK_GUI_H 1
/**************** STRUCTS *****************/
struct guiWindow
{
	WINDOW *widowRef;
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