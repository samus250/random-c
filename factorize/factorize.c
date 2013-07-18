#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The first 16 factors are bigger than 64 bit llong.
#define MAX_NUM_FACTORS 15
#define VERSION "0.1.1b"
#define MAX_INPUT_LENGTH 32

#ifdef WIN32
#define FACTOR_VAL_FORMAT "%I64u"
#else
#define FACTOR_VAL_FORMAT "%llu"
#endif

typedef unsigned long long valtype;
typedef unsigned int u_int;
typedef struct Factor {
  valtype value;
	u_int count; // May change for size_t.
} Factor;

u_int factorize(valtype number, Factor *factors);
int compare_factors(const void *lhs, const void *rhs);
void print_factors(FILE *write_file, const Factor *factors, u_int num_factors);
void print_factor(FILE *write_file, Factor factor);

void welcome(void) {
	printf("Welcome to Prime Factors " VERSION "\n");
}

int main(void) {
	valtype number; // The input number.
	Factor factors[MAX_NUM_FACTORS]; // Factor info.
	u_int num_factors = 0; // No. of facts found.
	char input[MAX_INPUT_LENGTH];
	char *endptr;
	
	// The absolutely first thing to do :-)
	welcome();
	
	start:
	memset(input, '\0', MAX_INPUT_LENGTH);
	
	printf("Enter an integer to be factorized (no negatives, please): ");
	scanf("%s", input);
	
	// Sanity checks.
	if (input[0] == '-') {
		printf("\nHey! Didn't I tell you NOT to enter negative numbers?\n");
		printf("Please don't do that again. Thank You.\n");
		goto start; // This is an example of me using goto. Thats how I roll.
	}
	
	number = strtoull(input, &endptr, 0);
	
	if (*endptr) {
		printf("\nWhy did you enter %s?\n", input);
		printf("I will use " FACTOR_VAL_FORMAT " instead.\n\n", number);
	}
	
	// Do the job.
	num_factors = factorize(number, factors);
	
  // Sort in ascending order.
	qsort(factors, num_factors, sizeof(Factor), compare_factors);
	// Sorts an array. The beginning of the array is pointed to by base. 
	// The array is n items long with each element in the array size bytes long.
	
	printf(FACTOR_VAL_FORMAT " = ", number);
	print_factors(stdout, factors, num_factors);
	
	goto start;
	
	return 0;
}

int compare_factors(const void *lhs, const void *rhs) {
	valtype a = (valtype)((Factor *)lhs)->value;
	valtype b = (valtype)((Factor *)rhs)->value;
	
	if (a < b) {
		return -1;
	} else {
		return (a == b) ? 0 : 1;
  }
}

u_int factorize(valtype number, Factor *factors) {
	u_int num_factors = 0;
	valtype i;
	
	// Entered number <= 2 ? store factors.
	if (number <= 2) {
		factors[0].value = number;
		factors[0].count = 1;
		return 1;
	}
	
	// Check for factors of 2
	if (number % 2 == 0) {
		factors[num_factors].value = 2; // Store the value.
		factors[num_factors].count = 1; // Store the first count.
		number /= 2;
		while (number % 2 == 0) { // Keep dividing.
			++factors[num_factors].count;
			number /= 2;
		}
		++num_factors;
	}
	
	// Check for factors going up.
	for (i = 3; i*i <= number; i+=2) {
		if (number % i == 0) {
			factors[num_factors].value = i; // Store val.
			factors[num_factors].count=1;
			number /= i;
			while (number % i == 0) {
				++factors[num_factors].count;
				number /= i;
			}
			++num_factors; // Next factor var to store.
		}
	}
	
	if (number > 1) {
		factors[num_factors].value = number;
		factors[num_factors].count = 1;
		++num_factors;
	}
	
	return num_factors;
}

void print_factors(FILE *write_file, const Factor *factors, u_int num_factors) {
	u_int i;
	
	print_factor(write_file, factors[0]);
	for (i=1; i < num_factors; ++i) {
		fprintf(write_file, " * ");
		print_factor(write_file, factors[i]);
	}
	fputc('\n', write_file);
	
}

void print_factor(FILE *write_file, Factor factor) {
	fprintf(write_file, FACTOR_VAL_FORMAT, factor.value);
	if (factor.count > 1)
		fprintf(write_file, "^%u", factor.count);
}
