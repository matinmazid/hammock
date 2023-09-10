// /usr/include/x86_64-linux-gnu/curl/curl.h
// example  https://cat-fact.herokuapp.com/facts/

#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

//***
struct MemoryStruct
{
    char *memory;
    size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    printf("-------------\n");
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr)
    {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int main(char **args)
{
    curl_global_init(CURL_GLOBAL_ALL);
    printf("hello world ");
    CURL *curl = curl_easy_init();
    if (curl)
    {
        CURLcode res;
        struct MemoryStruct chunk;

        chunk.memory = malloc(1); /* will be grown as needed by the realloc above */
        chunk.size = 0;           /* no data at this point */
        curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");

        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

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
            printf(">>>>>> %s <<<<<< \n", (unsigned long)chunk.memory);
        }

        /* cleanup curl stuff */
        curl_easy_cleanup(curl);

        free(chunk.memory);

        /* we are done with libcurl, so clean it up */
        curl_global_cleanup();

        return 0;
    }
}