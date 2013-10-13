/**********************************************************
      Prime Numbers: err.c. Copyright © 2008 by samus250

This program generates prime numbers using a simple algorithm
and prints the result to a text file in the same directory the 
program was opened.

This program is distributed in the hopes that it will be useful and
 comes with ABSOLUTELY NO WARRANTY.

************************************************************/

#include <stdio.h>
#include "primes.h"

#ifdef WIN32
#include <windows.h> /*for Sleep function*/
#else
#define Sleep(X) sleep(X)
#endif


void negative_input(void) /*if the user types a negative number, show these messages*/
{
	printf("\nHey! Didn't I tell you NOT to enter negative numbers?\n");
	Sleep(2500);
	printf("Please don't do that again. Thank You.");
	Sleep(1800);
}
	
void bad_ranges(void) /*if the user types a bad range, show these messages*/
{
	printf("\nWhat the??? You entered an invalid range.\n");
	Sleep(2000);
	printf("Make sure that when I ask you to enter the HIGHEST range,\n"); 
	printf("you actually enter the highest. Thanks.");
	Sleep(2500);
}
	
void too_big(void) /*if the number that the user typed is too big, show these messages*/
{
	printf("\nSorry, but the ranges you entered are too big for me.\n");
	Sleep(2000);
	printf("Please enter other ranges. Thanks.\n");
	Sleep(1800);
}
	
void decimal_input(u_long_64 range, char *string) /*if the user types a float range, notify about the floor*/
{
	printf("\nWhy did you enter " "%s" "?",string);
	Sleep(2000);
	printf("\nI will asume you entered " VAL_FORMAT "\n\n",range);
	Sleep(1800);
}

void only_numbers(void) /*if the user types something that is not a number*/
{
	printf("\nWhy did you enter that???");
	Sleep(2000);
	printf("\nPlease enter only numbers. Thank You");
	Sleep(1800);
}

