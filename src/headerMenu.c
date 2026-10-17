#include <menu.h>
#include <stdlib.h>
#include <curses.h>
#include <ncurses.h>
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#include "log.h"
#include "gui.h"

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
ITEM **generateMenu();
void destroyMenu(ITEM**menuItemsList, MENU *my_menu);
void update_description(WINDOW *description_win, const char *description);
MENU *my_menu;
extern char *ContentTypes[];

void doMenu()
{

    ITEM **menuItemsList;
    WINDOW *my_menu_win;
    WINDOW *description_win;

    menuItemsList = generateMenu();
    my_menu = new_menu((ITEM **)menuItemsList);

    description_win = newwin(3, 38, 12, 2); // TODO calcuate it
    update_description(description_win, choices[0].description); // print the description of the first item

    my_menu_win = newwin(10, 40, 2, 2);
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
                update_description(description_win, item_description(current_item(my_menu)));
                break;
            case KEY_UP:
                menu_driver(my_menu, REQ_UP_ITEM);
                update_description(description_win, item_description(current_item(my_menu)));
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

void update_description(WINDOW *description_win, const char *description)
{
    // Clear the description window
    werase(description_win);
    box(description_win, 0, 0);

    // // Get the description of the current item
    // const char *desc = item_description(current_item(my_menu));

    // Print the description in the window
    mvwprintw(description_win, 1, 1, "%s", description);
    wrefresh(description_win);
}

void destroyMenu(ITEM **menuItemsList, MENU *my_menu)
{
    unpost_menu(my_menu);
    free_menu(my_menu);
    for (int i = 0; i < ARRAY_SIZE(choices) - 1; ++i)
        free_item(menuItemsList[i]);
    free(menuItemsList);
}

ITEM **generateMenu()
{

    ITEM **menuItemsList;
    int choiceCount = ARRAY_SIZE(choices);
    menuItemsList = (ITEM **)calloc(choiceCount, sizeof(ITEM *));
    // populate the menu items end
    for (int i = 0; i < choiceCount; ++i)
    {
        menuItemsList[i] = new_item(choices[i].name, choices[i].description);
    }
    return menuItemsList;
}