
#ifndef __HAMMOCK_WEB_CLIENT_COMMON_H
#define __HAMMOCK_WEB_CLIENT_COMMON_H 1
#include <stddef.h>

extern size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
extern size_t readFromMemoryCallback(char *buffer, size_t size, size_t nitems, void *userdata);
#endif
