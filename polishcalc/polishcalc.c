#include <stdio.h>
#include <stdlib.h>

#define NUMBER '0' //signal that num was found
#define MAXOP 100 // max size of operand or operator

int getop(char []);
void push(double);
double pop(void);

// reverse polish calculator
int main(void)
{
	int type;
	double op2;
	char s[MAXOP];
	
	while((type = getop(s)) != EOF) {
		switch(type) {
			case NUMBER:
				push(atof(s));
				break;
			case '+':
				push(pop() + pop());
				break;
			case '*':
				push(pop() * pop());
				break;
			case '-':
				op2 = pop();
				push(pop() - op2);
				break;
			case '/':
				op2 = pop();
				if(op2 != 0.0)
					push(pop() / op2);
				else
					printf("Error: zero divisor\n");
				break;
			case '\n':
				printf("\t%.8g\n", pop());
				break;
			default:
				printf("Error: unknown command %s\n", s);
				break;
		}
	}
	
	return 0;
}

#define MAXVAL 100 // maximum depth of val stack

int sp = 0; // next free stack position
double val[MAXVAL]; // value stack

// push: push f into value stack
void push(double f)
{
	if(sp < MAXVAL)
		val[sp++] = f;
	else
		printf("Error: stack full, can't push %g\n", f);
}

// pop and return top value from stack
double pop(void)
{
	if(sp > 0)
		return val[--sp];
	else {
		printf("Error: stack empty\n");
		return 0.0;
	}
}

#include <ctype.h>

int getch2(void);
void ungetch2(int);

// getop: get next operator or numeric operand
int getop(char s[])
{
	int i, c;
	
	while((s[0] = c = getch2()) == ' ' || c == '\t')
		;
	
	s[1] = '\0';
	if(!isdigit(c) && c != '.')
		return c; // not a number
	
	i = 0;
	if(isdigit(c)) // collect integer part
		while(isdigit(s[++i] = c = getch2()))
			;
	if(c == '.') // collect fraction part
		while(isdigit(s[++i] = c = getch2()))
			;
	s[i] = '\0';
	if(c != EOF)
		ungetch2(c);
	return NUMBER;
}

#define BUFSIZE 100

char buf[BUFSIZE]; // buffer for ungetch
int bufp = 0; // next free position in buf

int getch2(void) // get a (possibily pushed back) character
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch2(int c) // push character back on input
{
	if(bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}

