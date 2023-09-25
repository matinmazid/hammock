#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "webClient.h"
#include "webClientCommon.h"

size_t readFromMemoryCallback(char *buffer, size_t size, size_t nitems, void *userdata){

    size_t writtenLength=0;
    size_t dataLenth=strlen(userdata);
    
    if (size*nitems < dataLenth)
        writtenLength =size*nitems;
    else
        writtenLength=dataLenth;
    
    buffer=(char *)userdata;

    return writtenLength;
}

size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct RestResponse *mem = (struct RestResponse *)userp;

    char *ptr = realloc(mem->responseBody, mem->size + realsize + 1);
    if (!ptr)
    {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->responseBody = ptr;
    memcpy(&(mem->responseBody[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->responseBody[mem->size] = 0;

    return realsize;
}