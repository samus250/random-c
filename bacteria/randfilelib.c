/***************************************************************************
 *   randfilelib.c - Written by samus250, Copyright (C) 2008.              *
 *   These functions provide a simple way to generate random filenames.    *
 *                                                                         *
 ***************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// perhaps the extension length shouldn't be too much
// it would make the program a lot faster
#define EXTENSION_LENGTH 32768

static char extension[EXTENSION_LENGTH];

char *rand_file_extension(char *filename)
{
    int y = 0, x;
	char pre_extension[EXTENSION_LENGTH];
    
    memset(extension, '\0', EXTENSION_LENGTH);
	memset(pre_extension, '\0', EXTENSION_LENGTH);
	
    x = strlen(filename) - 1; // strlen includes null
    while((pre_extension[y++] = filename[x--]) != '.')
        ;

    // invert the extension that was loaded
    x = strlen(pre_extension) - 1;
    y = 0;
    
    while(x >= 0)
        extension[y++] = pre_extension[x--];
    
    return extension;
}

char *rand_filename(char *random_filename, unsigned int min_length, unsigned int max_length, bool add_extension)
{
    int x, length, num;
    static bool selector;
    static long long runtime = 0;
    
    // pseudo randomly determine file length
    srand((unsigned)clock() + runtime + (time(NULL) - 800500));
    while((length = rand() % max_length) < min_length)
        ;
    
    if(selector)
        selector = false;
    else
        selector = true;
    
    srand((unsigned)time(NULL) + runtime);
    for(x = 0; x < length; x++) {
        num = rand() % 63;
        if(selector)
            num = 62 - num;
        
        switch(num) {
            case 0:
                random_filename[x] = '0'; break;
            case 1:
                random_filename[x] = '1'; break;
            case 2:
                random_filename[x] = '2'; break;
            case 3:
                random_filename[x] = '3'; break;
            case 4:
                random_filename[x] = '4'; break;
            case 5:
                random_filename[x] = '5'; break;
            case 6:
                random_filename[x] = '6'; break;
            case 7:
                random_filename[x] = '7'; break;
            case 8:
                random_filename[x] = '8'; break;
            case 9:
                random_filename[x] = '9'; break;
            case 10:
                random_filename[x] = 'a'; break;
            case 11:
                random_filename[x] = 'b'; break;
            case 12:
                random_filename[x] = 'c'; break;
            case 13:
                random_filename[x] = 'd'; break;
            case 14:
                random_filename[x] = 'e'; break;
            case 15:
                random_filename[x] = 'f'; break;
            case 16:
                random_filename[x] = 'g'; break;
            case 17:
                random_filename[x] = 'h'; break;
            case 18:
                random_filename[x] = 'i'; break;
            case 19:
                random_filename[x] = 'j'; break;
            case 20:
                random_filename[x] = 'k'; break;
            case 21:
                random_filename[x] = 'l'; break;
            case 22:
                random_filename[x] = 'm'; break;
            case 23:
                random_filename[x] = 'n'; break;
            case 24:
                random_filename[x] = 'o'; break;
            case 25:
                random_filename[x] = 'p'; break;
            case 26:
                random_filename[x] = 'q'; break;
            case 27:
                random_filename[x] = 'r'; break;
            case 28:
                random_filename[x] = 's'; break;
            case 29:
                random_filename[x] = 't'; break;
            case 30:
                random_filename[x] = 'u'; break;
            case 31:
                random_filename[x] = 'v'; break;
            case 32:
                random_filename[x] = 'w'; break;
            case 33:
                random_filename[x] = 'x'; break;
            case 34:
                random_filename[x] = 'y'; break;
            case 35:
                random_filename[x] = 'z'; break;
            case 36:
                random_filename[x] = 'A'; break;
            case 37:
                random_filename[x] = 'B'; break;
            case 38:
                random_filename[x] = 'C'; break;
            case 39:
                random_filename[x] = 'D'; break;
            case 40:
                random_filename[x] = 'E'; break;
            case 41:
                random_filename[x] = 'F'; break;
            case 42:
                random_filename[x] = 'G'; break;
            case 43:
                random_filename[x] = 'H'; break;
            case 44:
                random_filename[x] = 'I'; break;
            case 45:
                random_filename[x] = 'J'; break;
            case 46:
                random_filename[x] = 'K'; break;
            case 47:
                random_filename[x] = 'L'; break;
            case 48:
                random_filename[x] = 'M'; break;
            case 49:
                random_filename[x] = 'N'; break;
            case 50:
                random_filename[x] = 'O'; break;
            case 51:
                random_filename[x] = 'P'; break;
            case 52:
                random_filename[x] = 'Q'; break;
            case 53:
                random_filename[x] = 'R'; break;
            case 54:
                random_filename[x] = 'S'; break;
            case 55:
                random_filename[x] = 'T'; break;
            case 56:
                random_filename[x] = 'U'; break;
            case 57:
                random_filename[x] = 'V'; break;
            case 58:
                random_filename[x] = 'W'; break;
            case 59:
                random_filename[x] = 'X'; break;
            case 60:
                random_filename[x] = 'Y'; break;
            case 61:
                random_filename[x] = 'Z'; break;
            case 62:
                random_filename[x] = '_'; break;
            default:
                // something really weird happened.
                random_filename[x] = '_';
                break;
        }
    }
    if(add_extension)
        strcat(random_filename, extension);
    
    runtime++;
    return random_filename;

}
