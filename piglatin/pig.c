#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define VERSION "0.0.1b"
#define FWRITE(X) (X) ? stdout : write_file
#define FREAD(X) (X) ? stdin : read_file

void translate(char *word);
void strlower(char *string);
void help(void);

int main(int argc, char *argv[]) {
	FILE *read_file;
	FILE *write_file;
	bool ffile = false;
	char word[32], ch; // 32 byte input buffer
	char *w;
	int x;
	
	if(argc==1) {
		help();
		ffile = true;
		printf("\n");
	} else if(argc==2) {
		printf("Incorrect number of arguments");
		return 1;
	} else if(argc>2) {
		read_file = fopen(argv[1],"r");
		write_file = fopen(argv[2],"w");
	}
	
	while((ch=fgetc(FREAD(ffile))) != EOF) {
		if(isalpha(ch)) {
			x = 0;
			while(isalpha(ch)) {
				word[x] = ch;
				x++;
				ch = fgetc(FREAD(ffile));
			}
			word[x] = '\0'; // finish the word
			translate(word);
			
			// write word
			w = word;
			while(*w) {
				fputc(*w,FWRITE(ffile));
				w++;
			}
			fputc(ch,FWRITE(ffile));
		}
		else {
			// non alpha char
			fputc(ch,FWRITE(ffile));
		}
	}
	
	return 0;
}

void translate(char *word) {
	char *w; // word
	char piglatin[32]; 
	char append[] = "h";
	char ch; // used for check
	bool sw = false;
	
	w = word;
	if(isupper(*word)) // first case check
		sw = true;
	strlower(word); // all lower case
	
	ch = *word;
	switch(ch) {
		// first rule: just add "ay" at end
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u':
			strcpy(piglatin,word);
			strcat(piglatin,"ay");
			break;
		
		/* 	second rule: if word starts with SH, CH, TH,
			PH, RH, WH and QU, move both letters at the
			end before adding "ay"
		*/
		case 'c':
		case 'p':
		case 'r':
		case 's':
		case 't':
		case 'w':
			if(*(word+1) == 'h') {
				word += 2;
				strcpy(piglatin,word);
				append[0] = ch;
				strcat(piglatin,append);
				strcat(piglatin,"hay");
				break;
			}
		case 'q':
			if(*(word+1) == 'u') {
				word +=2;
				strcpy(piglatin,word);
				append[0] = ch;
				strcat(piglatin,append);
				strcat(piglatin,"hay");
				break;
			}
		
		/* standard rule: move first letter to end
			and add "ay"
		*/
		default:
			word++;
			strcpy(piglatin,word);
			append[0] = ch;
			strcat(piglatin,append);
			strcat(piglatin,"ay");
			break;
	}
	
	strcpy(w,piglatin);
	if(sw)
		*w = toupper(*w);
	
}

void strlower(char *string) {
	int len, x;
	
	len = strlen(string);
	for(x=0;x<len;x++)
		*(string+x) = tolower(*(string+x));
}

void help(void) {
	printf(
		"Welcome to PiggyPig " VERSION"\n\n"
		"USAGE: pig.exe [INPUT] [OUTPUT]\n"
	);
}
