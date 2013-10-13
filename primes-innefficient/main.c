/**********************************************************
      Prime Numbers: main.c. Copyright © 2008 by samus250

This program generates prime numbers using a simple algorithm
and prints the result to a text file in the same directory the 
program was opened.

This program is distributed in the hopes that it will be useful and
 comes with ABSOLUTELY NO WARRANTY.

************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "primes.h"

/*struct for program options*/

struct options {
	bool progress;
} options;

u_long_64 first_range,second_range; /*range limits*/
u_long_64 prime_numbers = 0; /*primes found*/

void welcome(void)
{
	printf("\nWelcome to Prime Numbers " VERSION"! Written by samus250. Copyright 2008.\n"
		"This program comes with ABSOLUTELY NO WARRANTY!\n\n");
	printf("This program will first ask you for a range.\nBut please, don't enter negative numbers.");
}

int main(void)
{
	
	char first_range_string[128], second_range_string[128];
	char *endptr;

	welcome();
	
	for(;;){
	
	// first, lets clean up just in case
	memset(first_range_string,'\0',128);
	memset(second_range_string,'\0',128);
	first_range=second_range=prime_numbers=0;
	
	//! First Range
	
	printf("\n\nPlease enter the lowest range number: ");
	scanf("%s",&first_range_string);
	
	if(!strcasecmp("exit",first_range_string)) return 0;
	else if(first_range_string[0] == '-' && first_range_string[1] == '-')
	{
		optparse(&first_range_string[2]);
		continue;
	}
	
	if(first_range_string[0] == '-')
	{
		negative_input(); /*if the input is negative, show the error message*/
		continue; // you can see continue just like goto
	}
	
	first_range = strtoull(first_range_string,&endptr,0);
	
	if(*endptr == '.') /*if the user types a float number, show this message*/
		decimal_input(first_range, first_range_string);
	
	if((*endptr) && *endptr != '.')
	{
		only_numbers();
		continue;
	}

	//! Second Range
	
	printf("Please enter the highest range number: ");
	scanf("%s",&second_range_string);
	
	if(!strcasecmp("exit",second_range_string)) return(0);
	else if(first_range_string[0] == '-' && first_range_string[1] == '-')
	{
		optparse(&first_range_string[2]);
		continue;
	}
	
	if(second_range_string[0] == '-')
	{
		negative_input(); /*if the input is negative, show the error message*/
		continue;
	}
	
	second_range = strtoull(second_range_string,&endptr,0);
	
	if(*endptr == '.')
		decimal_input(second_range, second_range_string); /*if the user types a float number, show this message*/
	
	if((*endptr) && *endptr != '.')
	{
		only_numbers();
		continue;
	}
	
	//! Check Ranges
	
	if(first_range>=second_range) /*if the user types a bad range, show error message*/
	{	
		bad_ranges();
		continue;
	}
	
	//! End error check
	
	printf("\n");
	
	file_init();
	
	if(!options.progress)
		primegen();
	else
		primegen_prog();
	
	results();
	
	//! After Results Given
	
	printf("\nCheck the \"primes.txt\" file, Generated in the same Directory this Program is In.\n");
	
	} //! End for
	
	return 0;
}

bool optparse(char *string)
{
	if(!strcasecmp(string, "progress"))
	{
		if(options.progress)
		{
			options.progress = 0;
			printf("\nProgress turned off");
			return 1;
		}
		else
		{
			options.progress = 1;
			printf("\nProgress turned on");
			return 1;
		}
	}
	else
	{
		printf("\nPlease enter a valid option, thanks.");
		return 0;
	}
}

