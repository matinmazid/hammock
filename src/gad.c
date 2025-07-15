
#include <stdlib.h>
// #include <strings.h>
#include <string.h>
#ifndef __HAMMOCK_GUI_H
#include "gui.h"
#endif
#include "webClient.h"
#include <ctype.h>
#include <curl/curl.h>

#include <menu.h>
#include "menu.h"
#include <curses.h>
#include "webClient.h"
#include "log.h"
extern void doMenu();

int main(){

	char* request[]={
		"https://google.com",
		"https://yahoo.com",
		"https://disney.com",
		"https://google.ca",
		"https://yahoo.ca",
		"https://google.com",
		"https://bing.com",
		"https://duckduckgo.com",
		"https://github.com"
	};

	size_t requestSize = sizeof(request) / sizeof(request[0]);

	for (int x=0; x<requestSize; x++){
		struct RestResponse r;
		log_info("Executing request: %s", request[x]);
		int strLen= strlen(request[x]);
		char *urlPtr= calloc(strLen + 1, sizeof(char));
		if (urlPtr == NULL) 
			log_error("Memory allocation failed for URL pointer");

		memcpy(urlPtr, request[x], strLen);	
		r=executeRest(urlPtr,0, ContentTypes, "");
		printf("Response URL: %s\n", r.url);
		printf("Response Body: %s\n", r.responseBody);
		printf("Response Size: %zu\n", r.size);
		printf("Response Status Code: %d\n", r.statusCode);
		printf("Response Client Message: %s\n", r.client_message);	
		log_info("--------- DONE ---------- %s %d ", request[x], x);
	}

}