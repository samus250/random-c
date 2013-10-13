/*****************************************************************

SemiPrime Numbers: err.c. Copyright © 2008 by samus250

This program generates prime numbers using a simple algorithm
and prints the result to a text file in the same directory the 
program was opened.

This program is distributed in the hopes that it will be useful 
and comes with ABSOLUTELY NO WARRANTY.

******************************************************************/

#include <stdio.h>
#include "primes.h"

void negative_input(void) // if the user types a negative number, show these messages
{
	printf("\nHey! Didn't I tell you NOT to enter negative numbers?\n");
	printf("Please don't do that again. Thank You.");
}
	
void bad_ranges(void) // if the user types a bad range, show these messages
{
	printf("\nWhat the??? You entered an invalid range.\n");
	printf("Make sure that when I ask you to enter the HIGHEST range,\n" 
		"you actually enter the highest. Thanks."
	);
}
	
void too_big(void) // if the number that the user typed is too big, show these messages
{
	printf("\nSorry, but the ranges you entered are too big for me.\n");
	printf("Please enter other ranges. Thanks.\n");
}
	
void decimal_input(u_long_64 range, char *string) // if the user types a float range, notify about the floor
{
	printf("\nWhy did you enter " "%s" "?\n",string);
	printf("I will asume you entered " VAL_FORMAT "\n\n",range);
}

void only_numbers(void) // if the user types something that is not a number
{
	printf("\nWhy did you enter that???\n");
	printf("Please enter only numbers. Thank You");
}


