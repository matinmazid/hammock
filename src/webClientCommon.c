#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "webClient.h"
#include "webClientCommon.h"
#include "log.h"

char *STATUS_CODE_STRINGS[] = {
    "CLIENT_ERROR",
    "CLIENT_SUCCESS"
};

size_t readFromMemoryCallback(char *buffer, size_t size, size_t nitems, void *userdata){

    log_debug("readFromMemoryCallback: size=%zu, nitems=%zu, userdata=%p", size, nitems, userdata);
    size_t writtenLength=0;
    size_t dataLenth=strlen(userdata);
    
    if (size*nitems < dataLenth)
        writtenLength =size*nitems;
    else
        writtenLength=dataLenth;
    log_debug(" attempting move ");
    memmove(userdata,buffer,writtenLength);
   return writtenLength; 
}

size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    if (contents == NULL || userp == NULL)
    {
        log_error("writeMemoryCallback: contents or userp is NULL");
        return 0;
    }
    size_t realsize = size * nmemb;
    struct RestResponse *mem = (struct RestResponse *)userp;

    char *ptr = realloc(mem->responseBody, mem->size + realsize + 1);
    if (!ptr)
    {
        mem->client_message="Not enough memory (realloc failed)";
        mem->statusCode = CLIENT_ERROR;
        return 0;
    }
    log_debug("writeMemoryCallback: reallocating memory to %zu bytes", mem->size + realsize + 1);
    mem->responseBody = ptr;
    bzero(&(mem->responseBody[mem->size]), realsize + 1);
    memcpy(&(mem->responseBody[mem->size]), contents, realsize);
    mem->statusCode = CLIENT_SUCCESS;
    mem->size += realsize;

    return realsize;
}