/*Main*/

#include "dino.h"

struct arguments options = {
0, 0, 0, 0, 0};

int main(int argc, char *argv[])
{
	int x;
	char m_lang[32];
	
	/*scan options and store to struct*/
	for(x=1; x<argc; x++)
	{
		if(strcasecmp(argv[x], "--help") ==0 )
			options.help = 1;
		else if(strcasecmp(argv[x], "--m") == 0)
			options.manual = 1;
		else if(strcasecmp(argv[x], "--d") == 0)
			options.dino = 1;
		else if(strcasecmp(argv[x], "--y") == 0)
			options.y_bypass = 1;
		else
			options.other++;
	}
	
	/*enter manual mode by default*/
	if(argc == 1)
	{
		printf("Welcome to Dino.exe! " VERSION " Copyright 2008, samus250\n\n"
			"Entered Manual Mode.\n"
			"Type dino.exe --help at the command line for more information.\n\n"
			"For Dino-to-English, enter --d\n"
			"For English-to-Dino, enter --e\n"
			"For English-to-Dino with 'Y' bypass, enter --y\n\n"
			"Language Choice: ");
		while(1) /*scan the choice*/
		{
			scanf("%s", &m_lang);
			if(strcasecmp(m_lang, "--d") == 0)
			{
				options.dino = 1;
				break;
			}
			else if(strcasecmp(m_lang, "--e") == 0)
			{
				options.dino = 0;
				break;
			}
			else if(strcasecmp(m_lang, "--y") == 0)
			{
				options.dino = 0;
				options.y_bypass = 1;
				break;
			}
			else
			{
				printf("??? - Please type it correctly: ");
				continue;
			}
		}
		printf("\nYour choice was %s%s. You can start typing now.\n",
			(options.dino) ? "Dino-to-English" : "English-to-Dino",
			(options.y_bypass) ? " with 'Y' bypass" : ""
		);
		manual(argc, argv);
	}
	
	/*enter help first, if multiple options are entered*/
	else if(options.help)
		help(argc, argv);
	
	/*enter manual mode second, if multiple options are entered*/
	else if(options.manual)
		manual(argc, argv);
	
	/*start automatic file tranlsating*/
	else
		automatic(argc, argv);
	
	return(0);
}

void help(int argc, char *argv[]) /*I passed argv so I can use argv[0] as the program name*/
{
	printf("\n"
		"Usage: dino.exe [TXT LOCATION] [TRANSLATED TXT LOCATION] [OPTIONS]\n\n"
		"Options:\n\n"
		"--help : Shows this help text\n"
		"--m    : Enter manual mode\n"
		"         (can only be used with --d)\n"
		"--d    : Translate from Dino to English\n"
		"--y    : Bypass 'Y' letter translation\n"
		"\n"
		"- The default translating language is from English to Dino. Use the --d option\n"
		"  if you want to translate the other way around.\n\n"
		"- The 'Y' letter does not exist in Dino Language. Use the --y option to \n"
		"  override the Y translation. This has no effect on the --d option\n\n" 
		"- Enclose something you do not want to be translated in double pound signs \"##\"\n"
		"  Example: \" ##General Scales##\"\n\n"
		"- Press Control+C to stop any operation and exit the program.\n"
	);
	exit(0);
}

