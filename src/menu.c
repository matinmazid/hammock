#include "menu.h"

void doMenu(){

   WINDOW *my_menu_win;

    my_menu_win = newwin(10, 40, 4, 4);
    box(my_menu_win, 0, 0);

    refresh();
    int c;
    while((c = wgetch(my_menu_win)) != 263)
    {
        break;
    }

	wborder(my_menu_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(my_menu_win);
	delwin(my_menu_win);
    return ;
}