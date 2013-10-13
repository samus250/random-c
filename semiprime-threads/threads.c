#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <process.h>
#include <time.h>
#include <windows.h>

// for unsinged long long format
#if (defined(_MSC_VER) && (_MSC_VER < 1400)) || \
    (defined(__MSVCRT_VERSION__) && (__MSVCRT_VERSION__ < 0x0800))
#   define I64_FORMATA "%I64"
#elif defined(__BORLANDC__)
#   define I64_FORMATA "%L"
#else
#   define I64_FORMATA "%ll"
#endif
#define VAL_FORMAT I64_FORMATA "u"

#define WRITE_PROGRESS

// typedefs
typedef unsigned long long u_long_64;
typedef unsigned short int u_short;

// prototypes
void progress_indicator(void * dummy);
void progress(bool start, int perc);
int compare_semis(const void *a, const void *b); // semiprimegen depends on this one
u_long_64 *semiprimegen(u_long_64 first_range, u_long_64 second_range, u_long_64 *semiprimes_count);

// global exit variable
HANDLE gev_exit = 0;

int main(void)
{
    FILE *primefile = NULL;
    u_long_64 *semiprimes = NULL;
    u_long_64 file_linecount = 2, semiprimes_count = 0;
    unsigned int x;
    unsigned char rowcount = 1;
    int perc = 0;
    clock_t ticks;
    int tid;
    
    printf("Generating Semiprimes... ");
    gev_exit = CreateEvent(NULL, TRUE, FALSE, NULL);
    if(!gev_exit)
    {
        printf("CreateEvent failed, le = %d", GetLastError());
        return 1;
    }
    
    // begin progress indicator thread
    if( (tid = _beginthread(progress_indicator, 0, NULL)) == -1)
    {
        printf("Error creating progress indicator thread");
        return 1;
    }
    
    // generate semiprimes and store to memory
    ticks = clock();
    semiprimes = semiprimegen(1, 10000000, &semiprimes_count);
    ticks = clock() - ticks;
    
    // modify global exit variable
    SetEvent(gev_exit);
    
    // create file
    if((primefile = fopen("./semiprimes.txt","w")) == NULL)
    {
        printf("Error opening semiprimes.txt for writing.");
        return 1;
    }
    
#ifdef WRITE_PROGRESS
    printf("\nWriting to file...\n");
#endif
    
    // prints semiprimes found
    
    //print first row number
    fprintf(primefile,"1.");
    
    for(x=0; x<semiprimes_count; x++) 
    {
        fprintf(primefile,"\t" VAL_FORMAT , semiprimes[x]); // print the number to the text file
        if(rowcount == 10) // it will insert a new line in the text file, change value if desired
        {
            fprintf(primefile,"\n" VAL_FORMAT ".",file_linecount);
            rowcount = 0; // reset
            file_linecount++; // number of lines written
        }
        rowcount++; // number of printed primes on current row
#ifdef WRITE_PROGRESS
        if( (perc+1) <= (int)(100 * x / semiprimes_count)) 
        {
            perc = (int)(100 * x / semiprimes_count);
            progress(false, perc);
        }
#endif
    }
    
#ifdef WRITE_PROGRESS
    progress(false, 100);
    printf("\n");
#endif
    
    // close the file
    if(fclose(primefile) == EOF)
        printf("Error closing file\n");
    
    // unload semiprimes
    free(semiprimes);
    
    printf("\n");
    printf("Took %.2f seconds to generate semiprimes.\n", ticks / (double)CLOCKS_PER_SEC);
    
    system("pause>nul");
    return 0;
}

void progress_indicator(void *dummy) 
{
    u_short steps = 0;
    
    while(1) 
    {
        // print the first character "|"
        switch(steps) 
        {
            case 0:
                printf("|");
                break;
            case 1:
                printf("/");
                break;
            case 2:
                printf("-");
                break;
            case 3:
                printf("\\");
                break;
            default:
                break;
        }
        if(steps < 3)
            steps++;
        else
            steps = 0;
        
        if(WaitForSingleObject(gev_exit, 1000) == WAIT_OBJECT_0)
            _endthread();
        else 
            printf("\b");
    }
}

void progress(bool start, int perc)
{
    int i;
    
    //!TODO maybe get time of day if start is on
    
    printf("\r%3d%%",perc);
    printf("|");
    for (i = 0; i < 33; i++) // (100 / 3)
    {
        if (i == perc / 3)
            printf("*");
        else
            printf("-");
    }
    printf("|");
    
    fflush(stdout);
    
    return;
}

/**
*** Semiprime Generator
**/
#define BUFFER 1000000

u_long_64 *semiprimegen(u_long_64 first_range, u_long_64 second_range, u_long_64 *semiprimes_count)
{
    bool prime = true; 	// prime detection
    u_long_64 number_to_check, prime_range;
    u_long_64 i, y, z; 		// for internal loops
    u_long_64 *primes_address = NULL, *semi_prime_address = NULL; // memory management
    u_long_64 prime_count = 1; // 2 is the first prime number
    u_long_64 buff_mult = 1; // primes_address will be initialized when 2 is stored to it
    u_long_64 semi_prime = 0;
    
    // first of all, store the number 2 as a prime
    primes_address = malloc(sizeof(u_long_64) * BUFFER);
    *primes_address = 2;
    
    // this loop generates all the prime numbers needed
    // It will then store them to RAM, dynamically
    prime_range = second_range / 2;
    for(number_to_check=3; number_to_check <= prime_range; number_to_check+=2) 
    {
        // suppose the number is prime
        prime = true;
        
        for(i=3; i*i <= number_to_check; i+=2)
        {
            if(number_to_check % i == 0)
            {
                prime = false;
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
                primes_address = realloc(primes_address, (sizeof(u_long_64) * BUFFER * buff_mult));
            }
            primes_address[prime_count - 1] = number_to_check;
        }
        
    }
    
    // calculate semiprimes
    *semiprimes_count = buff_mult = 0;
    
    for(y=0; y<prime_count; y++)
    {
        for(z=y; z < prime_count; z++)
        {	
            semi_prime = (primes_address[y]) * (primes_address[z]);
            if(semi_prime < first_range)
                continue;
            else if(semi_prime > second_range)
                break; // don't keep multiplying by z, just break.
            else // if the newly generated semiprime is between ranges
            {
                (*semiprimes_count)++;
                if(*semiprimes_count >= BUFFER * buff_mult)
                {
                    buff_mult++;
                    semi_prime_address = realloc(semi_prime_address, (sizeof(u_long_64) * BUFFER * buff_mult));
                }
                semi_prime_address[(*semiprimes_count) -1] = semi_prime;
            }	
        }
        if(y == z) // if you break on the first time of the inner for loop, means that y^2 is too big, break entirely
            break;
    }
    
    // unload primes, they are not needed anymore
    free(primes_address);
    
    // sort semiprimes
    qsort(semi_prime_address, *semiprimes_count, sizeof(u_long_64), compare_semis);
    
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

