#include <menu.h>
#include <stdlib.h>
#include <curses.h>
#include <ncurses.h>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#include "log.h"
#include "gui.h"
#include "string.h"

struct menuDetails {
    ITEM** itemListPtr;
    int windowWidth;
    int menuItemCount;
};

struct choice {
    char *name;
    char *description;

};

struct choice choices[] = {
    {"request header", "update request header"},
    {"save file", "save request and response to a file"},
    {"exit menu", "exit menu"},
    {"quit all", "quit the program"}
    };

extern void doMenu();
struct menuDetails generateMenu();
void destroyMenu(ITEM**menuItemsList, MENU *my_menu);
int calcMenuWidth(int, int, int);
MENU *my_menu;
extern char *ContentTypes[];

void doMenu()
{

    struct menuDetails menuItemsList;
    WINDOW *my_menu_win;

    menuItemsList = generateMenu();
    my_menu = new_menu(menuItemsList.itemListPtr);


    my_menu_win = newwin(menuItemsList.menuItemCount, menuItemsList.windowWidth, 2, 2);
    keypad(my_menu_win, TRUE);
    box(my_menu_win, 0, 0);
    set_menu_win(my_menu, my_menu_win);
    refresh();
    post_menu(my_menu);
    wrefresh(my_menu_win);

    int c;
    while ((c = wgetch(my_menu_win)) != ESC)
    {
        ITEM *cur;
        switch (c)
        {
            case KEY_DOWN:
                menu_driver(my_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(my_menu, REQ_UP_ITEM);
                break;
            case 10: // Enter key
                cur = current_item(my_menu);
                log_debug("Menu item selected: %s", item_name(cur));
                break;
        }
        wrefresh(my_menu_win);
    } // end while

    delwin(my_menu_win);
    return;
}


void destroyMenu(ITEM **menuItemsList, MENU *my_menu)
{
    unpost_menu(my_menu);
    free_menu(my_menu);
    for (int i = 0; i < ARRAY_SIZE(choices) - 1; ++i)
        free_item(menuItemsList[i]);
    free(menuItemsList);
}

struct menuDetails generateMenu()
{

    ITEM **menuItemsList;
    int choiceCount = ARRAY_SIZE(choices);
    menuItemsList = (ITEM **)calloc(choiceCount, sizeof(ITEM *));
    // populate the menu items end
    int menuWidth=40;
    int i = 0;
    for (; i < choiceCount; ++i)
    {
        long nameLen=strlen(choices[i].name) ;
        long descrLen=strlen(choices[i].description);
        menuWidth=calcMenuWidth(nameLen,descrLen,menuWidth);
        menuItemsList[i] = new_item(choices[i].name, choices[i].description);
        
    }
    
    struct menuDetails _menuDetails= { .itemListPtr=menuItemsList , 
        .windowWidth=menuWidth,
        .menuItemCount=i+1};
    return _menuDetails;
}

int calcMenuWidth(int nameLen,int descrLen,int max)
{
    int menuItemLeng=(long) nameLen>descrLen ? nameLen: descrLen;
    return max > menuItemLeng? max: menuItemLeng+2; // add two for padding
}
