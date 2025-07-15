
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

extern void doMenu();

int main(){

			/* matin remove this */ 
			static struct RestResponse foo;
			foo.responseBody = malloc(3); 
			const char *baloney="this is some baloney content ";
			static int ctr=0;
			static int totalValue;
			char intValue[200];
			for (int i = 0; i < 100; i++)
			{
				ctr++;
				bzero(intValue, 200);
				int len_baloney = strlen(baloney);
				totalValue = ctr + len_baloney  ;
				snprintf(intValue,sizeof(intValue), "%s, %d %d", baloney, totalValue,i);
				printf("%s\n", intValue);
			}

}