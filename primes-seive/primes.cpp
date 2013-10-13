#include <stdio.h>

int main () {
    FILE *outfile = NULL;
    
    if(!(outfile = fopen("./primes.txt", "w"))) {
        fprintf(stderr, "Error opening file!!!\n");
        return 1;
    }
    
    for(;;) {
        int low_range, high_range, d, i;
        
        // read the ranges
        printf("Low Range: ");
        scanf("%d", &low_range);
        printf("High Range: ");
        scanf("%d", &high_range);
        
        // print output file
        fprintf(outfile, "Prime Numbers from %d to %d:\n", low_range, high_range);
        
        // change to 2
        if(low_range == 1) 
            low_range = 2;
        
        // calculate distance (array size)
        d = high_range - low_range + 1;
        
        // allocate memory for the distance
        bool *a = new bool[d];
        
        // initialize all the values to true
        for(i = 0; i < d; i++) a[i] = true;
        
        // strike all multiples of 2
        // a[0] is the number low_range, so if a[0] is impair,
        // strike of all impair indices (which are pair numbers)
        for(i = low_range % 2; i < d; i += 2) a[i] = false;
        
        for(i = 3; i * i <= high_range; i += 2) {
            // sieving, if a[i - low_range] is false,
            // then we do not strike all multiples
            if(i >= low_range && !a[i - low_range])
                continue;
            
            int j = low_range / i * i;
            
            if(j < low_range)
                j += i;
            
            if(j == i)
                j += i;
            
            j -= low_range;
            
            // remove all multiples
            for(; j < d; j += i){
                a[j] = false;
            }
        }
        
        // print the list
        for(i = low_range; i <= high_range; i++){
            if(i == 1) // if low_range == 1, then a[0] is false (1 is not prime)
                a[i - low_range] = false;
            
            if(i == 2) // if low_range == 2, then a[0] is true (2 is prime)
                a[i - low_range] = true;
            
            // print all other prime numbers
            if(a[i - low_range]) 
                fprintf(outfile, "%d\n", i);
        }
        
        fprintf(outfile, "\n");
        printf("\nFinished!\n\n");
    }
    
    return 0;
}
