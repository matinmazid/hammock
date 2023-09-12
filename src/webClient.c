// /usr/include/x86_64-linux-gnu/curl/curl.h
// example  https://cat-fact.herokuapp.com/facts/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gui.h"
#include "webClient.h"
//***

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    windows[LEFT].widowRef;
    size_t realsize = size * nmemb;
    struct RestRequest *mem = (struct RestRequest *)userp;

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

struct RestRequest doGet(char *url)
{
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    if (curl)
    {
        CURLcode res;
        struct RestRequest getRest;
        getRest.url=url;
        getRest.responseBody = malloc(1); /* will be grown as needed by the realloc above */
        getRest.size = 0;           /* no data at this point */
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

        free(getRest.responseBody);

        /* we are done with libcurl, so clean it up */
        curl_global_cleanup();

        return getRest;
    }
}
