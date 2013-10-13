/*English to Dino Module*/

#include "dino.h"

extern struct arguments options;

char eng_din(char ch)
{
	bool l_switch = 0;
	
	if(isupper(ch))
	{
		ch = tolower(ch);
		l_switch = 1; /*Turn on switch*/
	}
	switch(ch)
	{
		case 'a': return((l_switch) ? 'U' : 'u'); break;
		case 'b': return((l_switch) ? 'R' : 'r'); break;
		case 'c': return((l_switch) ? 'S' : 's'); break;
		case 'd': return((l_switch) ? 'T' : 't'); break;
		case 'e': return((l_switch) ? 'O' : 'o'); break;
		case 'f': return((l_switch) ? 'V' : 'v'); break;
		case 'g': return((l_switch) ? 'W' : 'w'); break;
		case 'h': return((l_switch) ? 'X' : 'x'); break;
		case 'i': return((l_switch) ? 'A' : 'a'); break;
		case 'j': return((l_switch) ? 'Z' : 'z'); break;
		case 'k': return((l_switch) ? 'B' : 'b'); break;
		case 'l': return((l_switch) ? 'C' : 'c'); break;
		case 'm': return((l_switch) ? 'M' : 'm'); break;
		case 'n': return((l_switch) ? 'D' : 'd'); break;
		case 'o': return((l_switch) ? 'E' : 'e'); break;
		case 'p': return((l_switch) ? 'F' : 'f'); break;
		case 'q': return((l_switch) ? 'G' : 'g'); break;
		case 'r': return((l_switch) ? 'H' : 'h'); break;
		case 's': return((l_switch) ? 'J' : 'j'); break;
		case 't': return((l_switch) ? 'K' : 'k'); break;
		case 'u': return((l_switch) ? 'I' : 'i'); break;
		case 'v': return((l_switch) ? 'L' : 'l'); break;
		case 'w': return((l_switch) ? 'N' : 'n'); break;
		case 'x': return((l_switch) ? 'P' : 'p'); break;
		case 'y': /*case Y is a little bit longer, since you can make the bypass Y option*/
			if(options.y_bypass != 1)
				return((l_switch) ? 'O' : 'o');
			else
				return((l_switch) ? 'Y' : 'y');
			break;
		case 'z': return((l_switch) ? 'Q' : 'q'); break;
		default: break;
	}
	l_switch = 0;
}

