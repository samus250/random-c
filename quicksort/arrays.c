#include <stdio.h>
#include <string.h>

#define WORDS 20
char *afruit[WORDS];

int partition(int left, int right);
void swap(int a, int b);
void sort(int left, int right);

int main()
{
	char *fruit[WORDS] = {
			"pear", "apple", "papaya", "orange", "grape",
			"banana", "mango", "berry", "cherry", "coconut",
			"more", "new", "dog", "the", "quick", "brown",
			"fox", "jumped", "over", "lazay"};
	int x,first,last;

	first = 0;
	last = WORDS-1;

	for(x=0;x<WORDS;x++)
		afruit[x] = fruit[x];

	sort(first,last);

	printf("Before\tAfter\n");
	for(x=0;x<WORDS;x++)
		printf("%s\t%s\n",fruit[x],afruit[x]);
	return 0;
}

void sort(int left, int right)
{
		int p;

		if(left>=right)
				return;
		p = partition(left, right);

		sort(left,p-1);
		sort(p+1,right);
}

int partition(int left, int right)
{
	int first,pivot;

	first = left;
	pivot = right--;
	while(left<=right)
	{
		while(strcmp(afruit[left],afruit[pivot]) < 0)
			left++;
		while( (right>=first) && (strcmp(afruit[right],afruit[pivot])>=0) )
			right--;
		if(left<right)
		{
			swap(left,right);
			left++;
		}
	}

	if(left!=pivot)
		swap(left,pivot);

	return left;
}

void swap(int a, int b)
{
		char *temp;

		temp=afruit[a];
		afruit[a]=afruit[b];
		afruit[b]=temp;
}

