#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webClient.h"
#include "webClientCommon.h"
#include "log.h"
//***

char *ContentTypes[] = {
    "Content-Type: application/json",
    "Content-Type: application/psv",
    "Content-Type: application/csv",
    "Content-Type: application/json"};

extern struct RestResponse doGet(char *url, char **header, char *);
extern struct RestResponse doPost(char *url, char **header, char *);
extern struct RestResponse doDelete(char *url, char **header, char *);
extern struct RestResponse doPatch(char *url, char **header, char *);
extern struct RestResponse doPut(char *url, char **header, char *);
extern char *webclient_statusCodeStr(enum STATUS_CODES statusCode);

char *methodNameList[] = {"GET", "PUT", "POST", "DELETE", "PATCH"};
methodPtr methodList[] = {&doGet, &doPut, &doPost, &doDelete, &doPatch};

struct RestResponse errorMessageGenerator(CURLcode res, struct RestResponse restData);
struct RestResponse processResponse(CURLcode res, struct RestResponse restData);
struct RestResponse initFailureResponse(char *url);

char *webclient_statusCodeStr(enum STATUS_CODES statusCode)
{

    return STATUS_CODE_STRINGS[statusCode];
}

_RestResponse executeRest(char *url, int handlerIndex, char *headers[], char *body)
{

    return methodList[handlerIndex % 5](url, headers, body);
}

struct RestResponse doPut(char *url, char **header, char *requestBody)
{
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    if (curl)
    {
        CURLcode res;
        struct RestResponse restData;
        restData.url = url;
        restData.responseBody = malloc(1); /* will be grown as needed by the realloc */
        restData.size = 0;                 /* no data at this point */
        struct curl_slist *headerList = NULL;
        curl_slist_append(headerList, header[0]);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestBody);
        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&restData);
        /* some servers do not like requests that are made without a user-agent field, so we provide one */
        res = curl_easy_perform(curl);
        /* get it! */

        restData = processResponse(res, restData);

        curl_slist_free_all(headerList);
        /* cleanup curl stuff */
        curl_easy_cleanup(curl);
        curl_global_cleanup();

        // there is a memory leak b/c we keep mallocing
        return restData;
    }
    return initFailureResponse(url);
}

struct RestResponse doDelete(char *url, char **header, char *body)
{

    struct RestResponse r = {0};
    return r;
}
struct RestResponse doPatch(char *url, char **header, char *body)
{

    struct RestResponse r = {0};
    return r;
}

struct RestResponse doPost(char *url, char **header, char *body)
{
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    if (curl)
    {
        CURLcode res;
        struct RestResponse restData;
        restData.url = url;
        restData.responseBody = NULL; /* will be grown as needed by the realloc */
        restData.size = 0;            /* no data at this point */
        struct curl_slist *headerList = NULL;
        curl_slist_append(headerList, header[0]);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(body));

        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&restData);

        /* some servers do not like requests that are made without a user-agent field, so we provide one */
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
        res = curl_easy_perform(curl);
        /* get it! */

        restData = processResponse(res, restData);
        /* cleanup curl stuff */
        curl_easy_cleanup(curl);

        /* we are done with libcurl, so clean it up */
        curl_global_cleanup();

        curl_slist_free_all(headerList);
        return restData;
    }
    return initFailureResponse(url);
}

struct RestResponse doGet(char *url, char **header, char *body)
{
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    struct RestResponse restData;
    if (curl)
    {
        CURLcode res;

        restData.url = url;
        restData.responseBody = NULL;
        restData.size = 0;

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&restData);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        log_debug("sub system url execution for %s", url);
        res = curl_easy_perform(curl);
        log_debug("sub system url execution url %s bytes read %d", url, restData.size);

        restData = processResponse(res, restData);

        /* cleanup curl stuff */
        curl_easy_cleanup(curl);

        /* we are done with libcurl, so clean it up */
        curl_global_cleanup();

        return restData;
    }
    return initFailureResponse(url);
}

struct RestResponse processResponse(CURLcode res, struct RestResponse restData)
{
    if (res == CURLE_OK)
    {
        restData.statusCode = CLIENT_SUCCESS;
        restData.client_message = webclient_statusCodeStr(CLIENT_SUCCESS);
    }
    else
    {
        return errorMessageGenerator(res, restData);
    }
    return restData;
}

struct RestResponse initFailureResponse(char *url)
{
    struct RestResponse restData;
    restData.url = url;
    restData.responseBody = NULL;
    restData.size = 0;
    restData.statusCode = CLIENT_ERROR;
    restData.client_message = "Initialization failed";
    return restData;
}

struct RestResponse errorMessageGenerator(CURLcode res, struct RestResponse restData)
{

    const char *errString = curl_easy_strerror(res);
    size_t errorStrLength = strlen(errString) + 10; // 20 is for the "failed: " prefix and null terminator
    char *errorMessage = malloc(errorStrLength);
    restData.statusCode = CLIENT_ERROR;
    if (errorMessage == NULL)
    {
        log_error("Memory allocation failed for error message");
        restData.responseBody = "fail";
        restData.client_message = "Memory allocation failed";
        return restData;
    }
    memset(errorMessage, '\0', errorStrLength);
    log_error("curl_easy_perform() failed: %s", errString);
    asprintf(&errorMessage, "failed: %s", errString);
    restData.responseBody = "fail";
    restData.client_message = errorMessage;
    return restData;
}
