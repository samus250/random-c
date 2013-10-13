/**********************************************************
      Prime Numbers: time.c. Copyright © 2008 by samus250

This program generates prime numbers using a simple algorithm
and prints the result to a text file in the same directory the 
program was opened.

This program is distributed in the hopes that it will be useful and
 comes with ABSOLUTELY NO WARRANTY.

************************************************************/

#include <stdio.h>
#include <time.h>
#include "primes.h"

void get_date(FILE *primefile)
{
	int month;
	int day;
	int year;
	char *wdays[] = {
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday"
	};
	struct tm *t;
	time_t now;
	
	time(&now); /*get the current (epoch) time*/
	t = localtime(&now); /*get the tm structure*/
	
	month=t->tm_mon; /*interchange variables*/
	day=t->tm_mday;
	year=t->tm_year;
	
	fprintf(primefile,
		"*****"
		"                " /*20 characters of formating space*/
		"Generated %s, %d/%d/%d, at %d:%02d:%02d %s"
		"                "
		"*****\n\n",
		wdays[t->tm_wday],
		month+1,day,year+1900,
		t->tm_hour>12 ? t->tm_hour-12 : t->tm_hour,
		t->tm_min,
		t->tm_sec,
		t->tm_hour>12 ? "PM" : "AM"
	);
}

