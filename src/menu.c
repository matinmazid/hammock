#include "menu.h"
#include <menu.h>
#include "menu.h"
#include <stdlib.h>
#include <curses.h>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

char *choices[] = {
	"json",
    "csv"

};
extern void doMenu();

void eraseWindow(WINDOW *my_menu_win)
{

    wborder(my_menu_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(my_menu_win);
    delwin(my_menu_win);
}

extern char *ContentTypes[];
void doMenu()
{

    WINDOW *my_menu_win;
    ITEM **menuItemsList;
    ITEM *currentMenuItem;
    MENU *headerMenu;

    my_menu_win = newwin(10, 40, 0, 0);
    box(my_menu_win, 1, 1);

    // try to get the ArraySize Macro working
    /* available ContentType json
    ContentType csv
    ContentType text
    */
    int nHeaderTypeCount = ARRAY_SIZE(choices);
    menuItemsList = (ITEM **)calloc(nHeaderTypeCount + 1, sizeof(ITEM *));

    for(int i=0;i<nHeaderTypeCount;++i)
    {
        menuItemsList[i]= new_item(choices[i],choices[i]);
    }
    // // use a loop later
    menuItemsList[nHeaderTypeCount] = (ITEM *)NULL;
    headerMenu = new_menu((ITEM **)menuItemsList);
    post_menu(headerMenu);
    refresh();
    int c;
    while ((c = wgetch(my_menu_win)) != 'h')
    {
        switch (c)
        {
            case KEY_DOWN:
                menu_driver(headerMenu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
            default:
                break;
        }
        break;
    }

    free_item(menuItemsList[0]);
    free_item(menuItemsList[1]);
    free_menu(headerMenu);
    return;
}
