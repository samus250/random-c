#include <stdio.h>

#define TAUT(P,Q) ((P & Q) | (~P) | (~Q))
// a tautology is a transformation wich its 
// values will always be true.

void binString(int n);

int main()
{
	unsigned short int P,Q,value;

	P = 0xA3; //put any values here, it will always be true
	Q = 0xF3;
	
	value = TAUT(P,Q);
	
	printf("Binary value is ");
	binString(value);
	putchar('\n');
	return(0);
}

void binString(int n)
{
	char bin[17];
	int x;

	for(x=0;x<16;x++)
	{
		bin[x] = n & 0x8000 ? '1' : '0';
		n <<= 1;
	}
	bin[16] = '\0';
	printf("%s",bin);
}

