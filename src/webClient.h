#ifndef __HAMMOCK_REST_H
#define __HAMMOCK_REST_H 1

#include <curl/curl.h>
#include <stddef.h>


// TODO fix this so _means hidden
// TODo create a typedef for RestRequest
typedef struct RestResponse _RestResponse;
extern char *methodNameList[];
extern char *CommonHeaders[];
extern _RestResponse executeRest(char * url, int handlerIndex,char ** , char *);

struct RestRequest{
    char *url;
    char **headers;
};

struct RestResponse
{
    char *url;
    char *responseBody;
    size_t size;
};

typedef struct RestResponse (*methodPtr)(char*, char**,char * );
#endif