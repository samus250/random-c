/*****************************************************************

SemiPrime Numbers: main.c. Copyright © 2008 by samus250

This program generates prime numbers using a simple algorithm
and prints the result to a text file in the same directory the 
program was opened.

This program is distributed in the hopes that it will be useful 
and comes with ABSOLUTELY NO WARRANTY.

******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "primes.h"

void welcome(void)
{
	printf("\nWelcome to SemiPrime Numbers " VERSION"! Written by samus250. Copyright 2008.\n"
		"This program comes with ABSOLUTELY NO WARRANTY!\n\n");
	printf("This program will first ask you for a range.\nBut please, don't enter negative numbers.");
}

int main(void)
{
	FILE *primefile;
	char first_range_string[128], second_range_string[128];
	char *endptr;
	unsigned char filecount;
	bool line_number_switch;
	u_long_64 file_linecount, *semiprimes = NULL, semiprimes_count = 0;
	u_long_64 first_range,second_range;
	unsigned int x;
	int perc = 0;

	welcome();
	
	for(;;) {
	
	// first, lets clean up
	memset(first_range_string,'\0',128);
	memset(second_range_string,'\0',128);
	first_range=second_range=perc=0;
	line_number_switch=0;
	filecount=1;
	file_linecount=2;
	
	//! First Range
	
	printf("\n\nPlease enter the lowest range number: ");
	scanf("%s",&first_range_string);
	
	if(!strcasecmp("exit",first_range_string)) return 0;
	
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
	
	primefile = file_init(primefile);
	semiprimes = semiprimegen(first_range, second_range, &semiprimes_count);
	
#ifdef PROGRESS
	printf("\nWriting to file...\n");
#ifdef WIN32
	fflush(stdout);
#else
	fpurge(stoud);
#endif
#endif

	// prints semiprimes found
	for(x=0; x<semiprimes_count; x++) 
	{
		// printf(VAL_FORMAT "\t", *(semiprimes + x));
		if(line_number_switch==0) // first line
		{
			fprintf(primefile,"1.");
			line_number_switch=true;
		}
		fprintf(primefile,"\t" VAL_FORMAT , *(semiprimes + x)); // print the number to the text file
		if(filecount==10) // it will insert a new line in the text file, change value if desired
		{
			fprintf(primefile,"\n" VAL_FORMAT ".",file_linecount);
			filecount=0; // reset
			file_linecount++; // number of lines written
		}
		filecount++; // number of printed primes
#ifdef PROGRESS
		if( (perc+1) <= (int)(100.0 * x / semiprimes_count)) {
			perc = (int)(100.0 * x / semiprimes_count);
			progress(false, perc);
		}
#endif
	}

#ifdef PROGRESS
	progress(false, 100);
	printf("\n");
#ifdef WIN32
	fflush(stdout);
#else
	fpurge(stoud);
#endif
#endif
	
	results(primefile, first_range, second_range, semiprimes_count);
	
	//! After Results Given
	
	// unload
	free(semiprimes);
	printf("\nCheck the \"semiprimes.txt\" file,\nGenerated in the same Directory this Program is In.\n");
	
	} //! End for
	
	return 0;
}

