#include <menu.h>
#include <stdlib.h>
#include <curses.h>
#include <ncurses.h>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#include "log.h"
#include "gui.h"

char* choices[] = {
    "header",
    "load file",
    "save file",
    "exit menu",
    "quit all",
    (char*)NULL
};

extern void doMenu();
MENU* generateMenu( ITEM*** menuItemsList);
// void eraseWindow(WINDOW* my_menu_win)
// {

//     wborder(my_menu_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
//     wrefresh(my_menu_win);
//     delwin(my_menu_win);
// }

extern char* ContentTypes[];
void doMenu()
{

    ITEM** menuItemsList;
    WINDOW* my_menu_win;


    // MENU* my_menu=generateMenu( &menuItemsList);


    MENU* my_menu;
    // calculate items in the menu
    int choiceCount = 0;
    choiceCount = ARRAY_SIZE(choices) ; 

    // allocate memory for the menu items
    menuItemsList = (ITEM**)calloc(choiceCount , sizeof(ITEM*)); 
    // populate the menu items end 
    for (int i = 0; i < choiceCount; ++i)
    {
        menuItemsList[i] = new_item(choices[i], choices[i]);
    }
    // menuItemsList[choiceCount] = (ITEM*)NULL; // NULL terminate the array
    my_menu=new_menu((ITEM**)menuItemsList);


    my_menu_win = newwin(10, 40, 2, 2);
    keypad(my_menu_win, TRUE);
    box(my_menu_win, 0, 0);
    set_menu_win(my_menu, my_menu_win);
    refresh();
    post_menu(my_menu);
    wrefresh(my_menu_win);

    int c;
    while((c=wgetch(my_menu_win))!=KEY_F(1))
    {   switch(c)
        {   case KEY_DOWN:
                menu_driver(my_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(my_menu, REQ_UP_ITEM);
                break;
        }
        wrefresh(my_menu_win);
    } // end while

    unpost_menu(my_menu);
    free_menu(my_menu);
    for(int i=0; i<ARRAY_SIZE(choices)-1; ++i)
        free_item(menuItemsList[i]);
    free(menuItemsList);    
    delwin(my_menu_win);
    return;
}

MENU* generateMenu( ITEM*** menuItemsList)
{

    MENU* my_menu;
    // calculate items in the menu
    int choiceCount = 0;
    choiceCount = ARRAY_SIZE(choices) - 1; // subtract 1 for the NULL terminator

    // allocate memory for the menu items
    menuItemsList = (ITEM**)calloc(choiceCount + 1, sizeof(ITEM*)); // +1 for the NULL terminator

    // populate the menu items end 
    for (int i = 0; i < choiceCount; ++i)
    {
        menuItemsList[i] = new_item(choices[i], choices[i]);
    }
    menuItemsList[choiceCount] = (ITEM*)NULL; // NULL terminate the array
    my_menu=new_menu((ITEM**)menuItemsList);
    return my_menu;
}