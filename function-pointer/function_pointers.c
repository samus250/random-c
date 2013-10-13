/* Pointer to function test */
#include <stdio.h>

// a simple pointer
int (*progress)(void *, double, double, double, double) = progress_callback;

void printer(int (*func)(void));
int number(void);
int letter(void);

typedef int (FUNCTION)(void);
// or something like
// typedef double (*FUNCTION)(double, int, double)
// typedef ((int)(*)(double *, double *))(*FUNCTION)(double, float, (int (*)(void *, void *)))

typedef ((int)(*)(double *, double *))(*FUNCTION2)(double, float, (int (*)(void *, void *)))

int main(void)
{
	char string[128];
	
	printf("1 for number, a for letter: ");
	scanf("%s",string);
	if(string[0] == '1') {
		printer(number);
	} else if(string[0] == 'a') {
		printer(letter);
	}
	printf("\nAddress: %p",(FUNCTION*) letter);

	return 0;
}

void printer(int (*func)(void))
{
	(*func)(); // call from pointer
}

int number(void)
{
	printf("2\n");
	return 2;
}

int letter(void)
{
	printf("b\n");
	return 1;
}

