#include <menu.h>
#include <stdlib.h>
#include <curses.h>
#include <ncurses.h>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#include "log.h"
#include "gui.h"

char* choices[][2] = {
    {"header","header"},
    {"load file","load file"},
    {"save file","save file"},
    {"exit menue","exit"},
    {"quit all","quit all"},
    {(char*)NULL,(char*)NULL}
};

extern void doMenu();

void eraseWindow(WINDOW* my_menu_win)
{

    wborder(my_menu_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(my_menu_win);
    delwin(my_menu_win);
}

extern char* ContentTypes[];
void doMenu()
{

    ITEM** menuItemsList;
    MENU* headerMenu;
    WINDOW* my_menu_win;

    my_menu_win = newwin(10, 40, 1, 1);
    box(my_menu_win, 0, 0);

    // try to get the ArraySize Macro working
    /* available ContentType json
    ContentType csv
    ContentType text
    */
    int nMenuCount = ARRAY_SIZE(choices);
    menuItemsList = (ITEM**)calloc(nMenuCount + 1, sizeof(ITEM*));

    for (int i = 0;i < nMenuCount;++i)
    {
        menuItemsList[i] = new_item(choices[i][0], choices[i][1]);
        set_item_userptr(menuItemsList[i], (void*)choices[i][1]);

    }

    // use a loop later
    menuItemsList[nMenuCount] = (ITEM*)NULL;
    headerMenu = new_menu((ITEM**)menuItemsList);
    post_menu(headerMenu);
    set_menu_win(headerMenu, my_menu_win);
    wrefresh(my_menu_win);
    refresh();
    int c;
    nl();

    while ((c = getch()) != '\n')
    {
        switch (c)
        {
            case KEY_DOWN:
                menu_driver(headerMenu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(headerMenu, REQ_UP_ITEM);
                break;
            case '\n':
                log_error("Selected Content-Type: %s", (char*)item_userptr(current_item(headerMenu)));
                break;
            case 127:
            case  CTRL('Q') : // ESC key
                eraseWindow(my_menu_win);
                return;
                break;
        }
    }

    nonl();

    for (int i = 0;i < nMenuCount;++i)
    {
        free_item(menuItemsList[i]);
    }

    free_menu(headerMenu);
    return;
}
