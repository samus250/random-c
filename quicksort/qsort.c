#include <stdio.h>

int v[10] = { 446, 585, 212, 115, 129, 0, 402, 730, 344, 690 };

int partition(int left, int right);
void sort(int left, int right);
void swap(int a, int b);

int main(void)
{
	int x,first,last;

	first = 0;
	last = 9;

	puts("Before sort:");
	for(x=0;x<10;x++)
		printf("%3d ",v[x]);
	putchar('\n');

	sort(first,last);

	puts("After sort:");
	for(x=0;x<10;x++)
		printf("%3d ",v[x]);
	putchar('\n');

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
		while(v[left] < v[pivot])
			left++;
		while( (right>=first) && (v[right]>=v[pivot]) )
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
	int temp;

	temp=v[a];
	v[a]=v[b];
	v[b]=temp;
}

