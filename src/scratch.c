
#include <stdlib.h>
// #include <strings.h>
#include <string.h>
#ifndef __HAMMOCK_GUI_H
#endif
#include "webClient.h"
#include <ctype.h>
#include <curl/curl.h>
#include "gui.h"
#include <menu.h>
#include "menu.h"
#include <curses.h>
#include "webClient.h"
#include "log.h"
extern void doMenu();

int calculateRequiredPadHeight(char *content, int childWidth)
{
	char * token;
	int lengthOfContent=0;
	int tokenLenghth = 0;
	token = strtok(content, "\n");
	token = strtok(NULL, "\n");
	// while (NULL!=(token=strtok(content, "\n") ))	
	// {
	// 	if (token == NULL)
	// 		break;
	// 	tokenLenghth=strlen(token);
	// 	lengthOfContent += (int) (tokenLenghth / childWidth) + 1; 
	// }
	return lengthOfContent ;
}
int main(){

	// char* request[]={
	// 	"https://google.com",
	// 	"https://yahoo.com",
	// 	"https://disney.com",
	// 	"https://google.ca",
	// 	"https://yahoo.ca",
	// 	"https://google.com",
	// 	"https://bing.com",
	// 	"https://duckduckgo.com",
	// 	"https://github.com"
	// };

	// size_t requestSize = sizeof(request) / sizeof(request[0]);

	// for (int x=0; x<requestSize; x++){
	// 	struct RestResponse r;
	// 	log_info("Executing request: %s", request[x]);
	// 	int strLen= strlen(request[x]);
	// 	char *urlPtr= calloc(strLen + 1, sizeof(char));
	// 	if (urlPtr == NULL) 
	// 		log_error("Memory allocation failed for URL pointer");

	// 	memcpy(urlPtr, request[x], strLen);	
	// 	r=executeRest(urlPtr,0, ContentTypes, "");
	// 	printf("Response URL: %s\n", r.url);
	// 	printf("Response Body: %s\n", r.responseBody);
	// 	printf("Response Size: %zu\n", r.size);
	// 	printf("Response Status Code: %d\n", r.statusCode);
	// 	printf("Response Client Message: %s\n", r.client_message);	
	// 	log_info("--------- DONE ---------- %s %d ", request[x], x);
	// }

	char * testStr = "this is a test string that should be long enough to require multiple lines \nin the child window";
	calculateRequiredPadHeight(testStr, 20);
	// initscr(); /* start curses mode 		*/
	// raw();
	// noecho();
	// keypad(stdscr, true); /* i need that nifty f1 	*/
	// WINDOW *pad = newpad(20, 3);

	// box(pad,0,0);
	// int x= getch();
	// mvwprintw(pad,2,2, "1234567890,");
	// prefresh(pad,5,2,5,5,25,15);
	// getch();
	// endwin();
}