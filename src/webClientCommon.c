#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "webClient.h"
#include "webClientCommon.h"

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
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