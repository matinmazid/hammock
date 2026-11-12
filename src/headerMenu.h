#ifndef HAMMOCK_HEADERMENU
#define HAMMOCK_HEADERMENU 1

struct choice {
    char *name;
    char *description;
    int choiceIndex;
};

extern struct choice choices[];


enum MenuAction{
    ACTION_REQUEST_HEADER,
    ACTION_SAVE_FILE,
    ACTION_EXIT_MENU,
    ACTION_QUIT
};
#endif