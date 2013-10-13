/*Auto-Manual*/

#include "dino.h"

extern struct arguments options;

//Auto

void automatic(int argc, char *argv[])
{
	FILE *file;
	FILE *write_file;
	unsigned long int code_count = 0;
	char ch;
	
	file = fopen(argv[1], "r");
	if(file == NULL) /*if the file does not exist, or if an invalid option was entered*/
	{
		printf("\nProgram not used correctly!\n\n"
			"Make sure that the file you are trying to translate (%s) exists.\n"
			"Type --help in the command line for more information.\n", argv[1]
		);
		exit(0);
	}
	write_file = fopen(argv[2], "w");
	if(write_file == NULL) /*if the file could not be created or written*/
	{
		printf("\nProgram not used correctly!\n\n"
			"Make sure that the file you are trying to modify (%s) exists.\n"
			"Type --help in the command line for more information.\n", argv[2]
		);
		exit(0);
	}
	
	start:
	while( (ch = fgetc(file)) != EOF) /*scan the file one character at a time*/
	{
		translate: /*If you find just one #, jump here*/
		if((isalpha(ch) <1 || isalpha(ch) >2) && ch != '#') /*print symbols*/
		{
			fprintf(write_file, "%c", ch);
			/*
			Explanation for isalpha(ch)<1 || isalpha(ch)>2)
			Apparently, isalpha returns 1 if the character is uppercase,
			returns 2 if it is lower case,
			returns 0 if it is a symbol, and
			returns 3 or more if it is unknown.
			So, I only include 1 and 2 in the condition.
			*/
		}
		/*Start Code Detection*/
		else if(ch == '#')
		{
			ch = fgetc(file);
			if(ch == EOF)
			{
				fprintf(write_file, "#");
				exit(0); /*print the missed # and finish*/
			}
			if(ch == '#')
			{	
				code_count++;
				printf("I found a code!\n");
				keep_code: /*Start here if you found a # inside a code*/
				ch = fgetc(file);
				while(ch != '#') /*Break if you find a #*/
				{
					fprintf(write_file, "%c", ch);
					ch = fgetc(file);
					if(ch == EOF)
					{
						printf("\nHey! You started a code and never finished it!\n"
							"Please, revise the file and make sure that is what you want.\n"
						);
						exit(0);
					}
				}
				ch = fgetc(file); /*Read the next character for end-of-code confirmation*/
				if(ch == '#')
				{	
					printf("The code finished!\n\n");
					goto start; /*Return to normal procedure*/
				}
				else if(ch != '#')
				{
					fprintf(write_file, "#"); /*print the missed #*/
					fprintf(write_file, "%c", ch);
					goto keep_code; /*Keep bypassing translation*/
				}
			}
			else
			{
				fprintf(write_file, "#"); /*print the missed #*/
				goto translate; /*keep normal procedure*/
			}
		}
		/*End Code Detection*/
		else
		{
			fprintf(write_file, "%c", (options.dino) ? din_eng(ch) : eng_din(ch));
		}
	}
	printf("\nSuccessfully translated file with a total of %d codes\n", code_count);
	exit(0);
}

// Manual

void manual(int argc, char *argv[])
{
	char ch;
	unsigned long int code_count=0;
	
	start:
	/*Read the comments on the function above... its the exact same thing, but we print to stdin instead*/
	while( (ch = fgetc(stdin)) != EOF)
	{
		translate: /*If you find just one #, jump here*/
		if((isalpha(ch) <1 || isalpha(ch) >2) && ch != '#') /*print symbols*/
		{
			fprintf(stdout,"%c",ch);
		}
		/*Start Code Detection*/
		else if(ch == '#')
		{
			ch = fgetc(stdin);
			if(ch == EOF)
			{
				fprintf(stdout, "#");
				exit(0);
			}
			if(ch == '#')
			{	
				code_count++;
				keep_code:
				ch = fgetc(stdin);
				while(ch != '#')
				{
					fprintf(stdout, "%c", ch);
					ch=fgetc(stdin);
				}
				ch = fgetc(stdin);
				if(ch == '#')
				{	
					goto start;
				}
				else if(ch != '#')
				{
					fprintf(stdout, "#");
					fprintf(stdout, "%c", ch);
					goto keep_code;
				}
			}
			else
			{
				fprintf(stdout, "#");
				goto translate;
			}
		}
		/*End Code Detection*/
		else
		{
			fprintf(stdout, "%c", (options.dino) ? din_eng(ch) : eng_din(ch));
		}
	}
	printf("\nSuccessfully translated input with a total of %d codes\n", code_count);
	exit(0);
}

