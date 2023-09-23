// /usr/include/x86_64-linux-gnu/curl/curl.h
// example  
// -L/usr/lib/x86_64-linux-gnu/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gui.h"
#include "webClient.h"
#include "webClientCommon.h"
//***

char *CommonHeaders[] = {"Content-Type: application/json"};
extern struct RestResponse doGet(char *url, char **header, char *);
extern struct RestResponse doPost(char *url, char **header, char *);
extern struct RestResponse doDelete(char *url, char **header, char *);
extern struct RestResponse doPatch(char *url, char **header, char *);
extern struct RestResponse doPut(char *url, char **header, char *);

char *methodNameList[] = {"GET", "PUT", "POST", "DELETE", "PATCH"};
methodPtr methodList[] = {&doGet, &doPut, &doPost, &doDelete, &doPatch};

_RestResponse executeRest(char *url, int handlerIndex, char *headers[], char *body)
{

    return methodList[handlerIndex % 5](url, headers, body);
}


struct RestResponse doPut(char *url, char **header, char *body)
{
    struct RestResponse r;
    return r;
}

struct RestResponse doDelete(char *url, char **header, char *body)
{

    struct RestResponse r;
    return r;
}
struct RestResponse doPatch(char *url, char **header, char *body)
{

    struct RestResponse r;
    return r;
}

struct RestResponse doPost(char *url, char **header, char *body)
{
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    struct curl_slist *headerList = NULL;
    if (curl)
    {
        CURLcode res;
        struct RestResponse getRest;
        getRest.url = url;
        getRest.responseBody = malloc(1); /* will be grown as needed by the realloc above */
        getRest.size = 0;                 /* no data at this point */
        curl_slist_append(headerList, header[0]);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen("{ \"  INPUT\": \"hello world\" }"));

        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&getRest);

        /* some servers do not like requests that are made without a user-agent field, so we provide one */
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
        res = curl_easy_perform(curl);
        /* get it! */

        /* check for errors */
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            getRest.responseBody = "fail";
            return getRest;
        }

        /* cleanup curl stuff */
        curl_easy_cleanup(curl);

        /* we are done with libcurl, so clean it up */
        curl_global_cleanup();

        curl_slist_free_all(headerList);
        // there is a memory leak b/c we keep mallocing
        return getRest;
    }
}
struct RestResponse doGet(char *url, char **header, char *body)
{
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    if (curl)
    {
        CURLcode res;
        struct RestResponse getRest;
        getRest.url = url;
        getRest.responseBody = malloc(1); /* will be grown as needed by the realloc above */
        getRest.size = 0;                 /* no data at this point */
        // curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
        curl_easy_setopt(curl, CURLOPT_URL, url);

        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&getRest);

        /* some servers do not like requests that are made without a user-agent
     field, so we provide one */
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        res = curl_easy_perform(curl);
        /* get it! */

        /* check for errors */
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }
        else
        {
            /*
     * Now, our chunk.memory points to a memory block that is chunk.size
     * bytes big and contains the remote file.
     *
     * Do something nice with it!
     */

            // printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
        }

        /* cleanup curl stuff */
        curl_easy_cleanup(curl);

        // free(getRest.responseBody);

        /* we are done with libcurl, so clean it up */
        curl_global_cleanup();

        return getRest;
    }
}
