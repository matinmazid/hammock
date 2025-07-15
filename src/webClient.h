#ifndef __HAMMOCK_REST_H
#define __HAMMOCK_REST_H 1

#include <curl/curl.h>
#include <stddef.h>

#ifndef __HAMMOCK_WEB_CLIENT_COMMON_H
#include "webClientCommon.h"
#endif

// TODO fix this so _means hidden
// TODo create a typedef for RestRequest
typedef struct RestResponse _RestResponse;
extern char *methodNameList[];
extern char *ContentTypes[];
extern _RestResponse executeRest(char * url, int handlerIndex,char ** , char *);

enum STATUS_CODES{
    CLIENT_ERROR,
    CLIENT_SUCCESS
};



struct RestRequest{
    char *url;
    char **headers;
};

struct RestResponse
{
    char *url;
    char *responseBody;
    size_t size;
    enum STATUS_CODES statusCode;
    char *client_message;
};

extern char *STATUS_CODE_STRINGS[] ; 
typedef struct RestResponse (*methodPtr)(char*, char**,char * );
#endif