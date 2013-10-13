#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#define FILE_LOCATION_STRING_SIZE 128

typedef unsigned long long u_int64;

void welcome(void)
{
    printf("Welcome to SpeedTest!!!\n\n");
}

void progress(u_int64 written, u_int64 total) {
    int i;
    double perc = (double) written / total * 100.0;
    
    printf("\r%.2lf%%",perc);
    printf(" |");
    for (i = 0; i < 100 / 3; i++) {
        if (i == (int)perc / 3)
            printf("*");
        else
            printf("-");
    }
    printf("| ");
    printf("%llu bytes written." , written);
    
    fflush(stdout);
}

int main(void)
{
    FILE *output;
    clock_t ticks;
    
    int i;
    bool done;
    double seconds, megabytes_written;
    long chunk_size, file_size;
    u_int64 written = 0;
    
    char file_location[FILE_LOCATION_STRING_SIZE];
    char file_name[] = "speedtest";
    char drive_letter[16], c;
    
    unsigned char *data = NULL;
    
    // first of all
    welcome();
    
    // init var and ask for drive
    memset(drive_letter, '\0', 16);
    printf("Drive: ");
    scanf("%15s", &drive_letter);
    
    // check if the input looks like drive letter
    if(!(isalpha(drive_letter[0]) && drive_letter[1] == ':')) {
        printf("Error: Input does not look like a drive letter!\n"
            "Format: [Letter]:\n"
        );
        return 0;
    }
    drive_letter[0] = toupper(drive_letter[0]);
    
    // ask for file size
    printf("Testfile size (in MB): ");
    scanf("%ld", &file_size);
    
    if(file_size < 0 && file_size > 4096) {
        printf("Error: Testfile size must be between 0 and 4096MB\n");
        return 0;
    }
    
    // ask for the chunk size
    printf("Chunk size (in KB): ");
    scanf("%ld", &chunk_size);
    
    if(chunk_size < 0 || chunk_size > 32768) {
        printf("Error: Chunk size must be from 0 to 32768 KB\n");
        return 0;
    }
    
    // print what you have and confirm
    putchar('\n');
    printf(
        "YOU ENTERED\n"
        "Drive:\t\t%c:\n"
        "Testfile size:\t%ld MB\n"
        "Chunk size:\t%ld KB\n\n"
        "Is this correct (Y/N)? ",
        drive_letter[0],
        file_size,
        chunk_size
    );
    
    // loop until the user gets it right
    do {
        fflush(stdin);
        scanf("%c", &c);
        c = toupper(c);
        
        switch(c) {
            case 'Y':
                done = true;
                break;
            case 'N':
                return 0;
                break;
            default:
                putchar('\n');
                printf("Please enter Y or N: ");
                done = false;
        }
    } while(!done);
    
    // tell the user that you are preparing to write
    printf("\nPreparing to write...\n");
    // prepare the chunk to be written
    data = malloc(chunk_size * 1024);
    for(i = 0; i < chunk_size * 1024; i++) {
        data[i] = 0xFF;
    }
    
    // prepare the file_location string and open the file
    memset(file_location, '\0', FILE_LOCATION_STRING_SIZE);
    sprintf(file_location, "%c:\\%s", drive_letter[0], file_name);
    output = fopen(file_location, "wb");
    
    // now write the info
    printf("Writing...\n");
    ticks = clock();
    while(written < file_size * 1024 * 1024) {
        written += fwrite(data, sizeof(unsigned char), chunk_size * 1024, output);
        progress(written, file_size * 1024 * 1024);
    }
    ticks = clock() - ticks;
    
    // close the file
    fclose(output);
    
    // finished, now calculate and print the info
    printf("\n\n");
    seconds = ticks / (double)CLOCKS_PER_SEC;
    megabytes_written = written / 1024 / 1024;
    printf("Operation took %.2f seconds.\nAverage speed was %.2f MB/s\n", seconds, megabytes_written / seconds);

    return 0;
}
