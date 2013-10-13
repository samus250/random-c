/*****************************************************************

SemiPrime Numbers: semiprime_gen.c. Copyright © 2008 by samus250

This program generates prime numbers using a simple algorithm
and prints the result to a text file in the same directory the 
program was opened.

This program is distributed in the hopes that it will be useful 
and comes with ABSOLUTELY NO WARRANTY.

******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "primes.h"

#define BUFFER 1000000

u_long_64 *semiprimegen(u_long_64 first_range, u_long_64 second_range, u_long_64 *semiprimes_count)
{
	bool prime=1, store_2 = 1; 	// prime detection
	u_long_64 number_to_check, prime_range, file_linecount=2;	 	// lines then start at 2
	u_long_64 i, x, y, z; 		// for internal loops
	u_long_64 *primes_address = NULL, *semi_prime_address = NULL; // memory management
	u_long_64 prime_count = 0; // temps
	u_long_64 buff_mult = 1;
	u_long_64 semi_prime = 0; // no need to keep in this scope, reasearch more if created multiple in for loop
	int perc = 0;

	// this loop generates all the prime numbers needed
	// It will then store them to RAM, dynamically
	prime_range = second_range / 2;

#ifdef PROGRESS
	printf("Generating Primes...\n");
#ifdef WIN32
	fflush(stdout);
#else
	fpurge(stdout);
#endif
	progress(false, 0); // calling progress for first time
#endif
	
	for(number_to_check=3; number_to_check <= prime_range; number_to_check+=2) 
	{
		prime = 1;
		if(store_2)
		{
			primes_address = (u_long_64 *)malloc(sizeof(u_long_64) * BUFFER);
			*primes_address = 2;
			prime_count = 1;
			store_2 = false;
		}
		
		for(i=3; i*i <= number_to_check; i+=2)
		{
			if(number_to_check % i == 0)
			{
				prime = 0;
				break;
			}
		}
		
		if(prime) 
		{
			// store the prime to dynamically allocated memory 
			prime_count++;
			if(prime_count >= BUFFER * buff_mult)
			{
				buff_mult++;
				primes_address = (u_long_64 *)realloc(primes_address, (sizeof(u_long_64) * BUFFER * buff_mult));
			}
			*(primes_address + prime_count-1) = number_to_check;
		}
#ifdef PROGRESS
		if( (perc+1) <= (int)(100.0 * number_to_check / prime_range)) {
			perc = (int)(100.0 * number_to_check / prime_range);
			progress(false, perc);
		}
#endif
		
	}

	// calculate semiprimes
#ifdef PROGRESS
	progress(false,100);
	printf("\n\n");
	printf("Generating Semiprimes...\n");
#ifdef WIN32
	fflush(stdout);
#else
	fpurge(stdout);
#endif
	progress(false, 0);
#endif
	
	*semiprimes_count = buff_mult = perc = 0;
	
	for(y=0; y<prime_count; y++)
	{
		for(z=y; z<prime_count; z++)
		{	
			semi_prime = (*(primes_address + y)) * (*(primes_address + z));
			if(semi_prime < first_range)
				continue;
			else if(semi_prime > second_range)
				break; // don't keep multiplying by z, just break.
			else // if the newly generated prime is between ranges
			{
				(*semiprimes_count)++;
				if(*semiprimes_count >= BUFFER * buff_mult)
				{
					buff_mult++;
					semi_prime_address = (u_long_64 *)realloc(semi_prime_address, (sizeof(u_long_64) * BUFFER * buff_mult));
				}
				*(semi_prime_address + (*semiprimes_count)-1) = semi_prime;
			}	
		}
		if(y == z) // if you break on the first time of the inner for loop, means that y^2 is too big, break entirely
			break;
	}
	
	// unload primes
	free(primes_address);
	
	// sort semiprimes
#ifdef PROGRESS
	progress(false, 100);
	printf("\n\n");
	printf("Sorting Semiprimes...\n");
#ifdef WIN32
	fflush(stdout);
#else
	fpurge(stdout);
#endif
#endif

#ifdef PROGRESS
	progress(false, 0);
#endif
	qsort(semi_prime_address, *semiprimes_count, sizeof(u_long_64), compare_semis);
#ifdef PROGRESS
	progress(false, 100);
	printf("\n");
#ifdef WIN32
	fflush(stdout);
#else
	fpurge(stdout);
#endif
#endif
	
	return semi_prime_address;
}

int compare_semis(const void *a, const void *b)
{
	const u_long_64 *aa = a, *bb = b;
	
	if(*aa < *bb)
		return -1;
	else
		return (*aa == *bb) ? 0 : 1;
}

void progress(bool start, int perc)
{
	int i;
	
	// maybe get time of day if start is on
	
	printf("\r%3d%%",perc);
	printf("|");
	for (i = 0; i < 33; i++) { // 33 == 100 / 3
		if (i == perc / 3)
			printf("*");
		else
			printf("-");
	}
	printf("|");
	fflush(stdout);
	
	return;
}

