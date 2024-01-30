#include "menu.h"
#include <menu.h>
#include "menu.h"
#include <stdlib.h>

extern void doMenu();
// void main(char ** args)
// {
//  doMenu();
// }


void eraseWindow(WINDOW *my_menu_win)
{

    wborder(my_menu_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(my_menu_win);
    delwin(my_menu_win);
}

extern char *ContentTypes[];
void doMenu()
{
//   initscr();
//         start_color();
//         cbreak();
//         noecho();
//         keypad(stdscr, TRUE);
//         init_pair(1, COLOR_RED, COLOR_BLACK);
//         init_pair(2, COLOR_GREEN, COLOR_BLACK);
//         init_pair(3, COLOR_MAGENTA, COLOR_BLACK);


    WINDOW *my_menu_win;
    ITEM **menuItemsList;
    ITEM *currentMenuItem;
    MENU *headerMenu;
    
    my_menu_win = newwin(10, 40, 4, 4);

    box(my_menu_win, 0, 0);

    // try to get the ArraySize Macro working
    /* available ContentType json
    ContentType csv
    ContentType text
    */    
    int nHeaderTypeCount = 4;
    menuItemsList=(ITEM**)calloc(nHeaderTypeCount +1,sizeof(ITEM *));

    // // use a loop later
    menuItemsList[0]=new_item("json","content type json");
    menuItemsList[1]=(ITEM *)NULL;
    headerMenu=new_menu((ITEM **) menuItemsList);

    refresh();
    int c;
    while ((c = wgetch(my_menu_win)) != 263)
    {
        break;
    }
    // eraseWindow(my_menu_win);
    return;
}
