/**********************************************************
   Prime Numbers: primegen.c. Copyright © 2008 by samus250

This program generates prime numbers using a simple algorithm
and prints the result to a text file in the same directory the 
program was opened.

This program is distributed in the hopes that it will be useful and
 comes with ABSOLUTELY NO WARRANTY.

************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include "primes.h"

// #define PROGRESS // undef if not needed

extern u_long_64 first_range,second_range;
extern u_long_64 prime_numbers; /*primes found*/
extern FILE *primefile;

void primegen(void)
{
	u_long_64 number_to_check,file_linecount=2; /*lines then start at 2*/
	u_short filecount=1; /*this is the file count, to insert a newline on the generated text file*/
	bool line_number_switch=0,prime=1; /*prime detection*/
	bool pair=0;
	u_long_64 i;
	
	number_to_check=first_range;
	
	/*the second range number will be like a limit for the while loop*/
	if(number_to_check==1)
	{
		number_to_check=2;
		pair=1;
	}
	
	pair = number_to_check % 2;
	
	while(number_to_check<=second_range) /*check numbers until limit is met*/
	{
		if(number_to_check % 2 == 0 && number_to_check != 2) //remove if-else for original implementation.
		{
			prime = 0;
		}
		else
		{
			for(i=3; i*i <= number_to_check; i+=2)
			{
				if(number_to_check % i == 0)
				{
					prime = 0;
					break;
				}
			}
		}
		
		if(prime) /*if the number was prime*/
		{
			printf(VAL_FORMAT "\t",number_to_check);
			
			if(line_number_switch==0) /*first line*/
			{
				fprintf(primefile,"1.");
				line_number_switch=true;
			}
			fprintf(primefile,"\t" VAL_FORMAT ,number_to_check); /*print the number to the text file*/
			if(filecount==10) /*it will insert a new line on the text file. change the value if you want*/
			{
				fprintf(primefile,"\n" VAL_FORMAT ".",file_linecount);
				filecount=0; /*reset*/
				file_linecount++; /*number of written lines*/
			}
			prime_numbers++; /*count one everytime a prime number is found*/
			filecount++; /*number of printed primes*/
		}		
		if(pair) // as in !pair since %2 == 0
		{
			number_to_check+=2;
		}
		else
		{
			number_to_check++;
			pair=1; // we now know that we skipped a pair
		}
		prime=1;
	}
	line_number_switch=0;
	filecount=1;
	file_linecount=2;
}
