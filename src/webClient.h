#ifndef __HAMMOCK_REST_H
#define __HAMMOCK_REST_H 1

#include <curl/curl.h>
#include <stddef.h>
extern struct RestRequest doGet(char *url);

struct RestRequest
{
    char *url;
    char *responseBody;
    size_t size;
};

#endif