Dino.exe - A program that translates English to Dino language and vice-versa
===============================================================================

What is Dino Language?
======================

Dino Language is an invented language used in the game StarFox Adventures for 
the Nintendo Gamecube. The language itself is based of english, since letters
are translated, not words.

In the StarFox Adventures booklet it says:
"This language has been spoken on Dinosaur Planet for thousands of years, since
their first encounter with off-worlders. Before that, they got off by with
grunts and roars; a few still do, but most have adopted this relatively new
language.

You can use this language to write secret messages to your friends and maybe 
even try speaking it too!"

How to write Dino Language:
===========================

Rearrange the vowel sounds:
English: A E I O U
Dino:    U O A E I

The letter 'M' remains the same: M=M

Replace the letter 'Y' with an 'O' - this means that the letter 'Y' will not exist 
in this language: Y=O

Rearrange the remaining letters as follows:

English: B C D F G H J K L N P Q R S T V W X Z
Dino:    R S T V W X Z B C D F G H J K L N P Q

Note that the names of people and places are not affected, remaining in their
original Language.

Program Usage:
==============

If you double click the dino.exe file, you will automatically enter manual
mode. In this mode, you just type in text and the program will translate it. 
You can also select which language you want to translate to and the 'Y' 
bypass option.

The 'Y' bypass option... what is that?:
Since in the Dino language the Y does not exist, I made an option that does NOT
translate the Y letter. This way, you can translate Dino langauge BACK to
English.

Wait a second, how can I NOT translate names then?
Easy, you just enclose the name in double pound signs "##".
Example: No-one can defeat ##General Scales##.

You can also put pound signs inside the untranslated space. But NOT double
pound signs. So, you can do this: ##I am #1##, but if you do this: ##I am ##1##
then the translation will turn out "I am 1" and the next characters will NOT be
translated, since they are all inside another set of "##".

If you want to use the program at full potential, you must use the command
line. Usage syntax is like follows:

dino.exe [TXT LOCATION] [TRANSLATED TXT LOCATION] [OPTIONS]

[TXT LOCATION]: Is the full path name of the text file you want to translate.
Example: "C:\My Files\this_file.txt"

[TRANSLATED TXT LOCATION]: Is the full path name of the text file that will
contain the translation. If the file does not exist, the program will create it
for you.
Example: "C:\My Files\this_file_translated.txt"

[OPTIONS]: Here you enter the options you want to include with your query. The
options are as follows:

--help : This option will show a help text.
--m    : This option enter Manual mode. 
--d    : This option will translate the file from Dino language to English.
--y    : This option will bypass the 'Y' translation. It will have no effect
         when translating from Dino to English.

If you just enter the file names, the program will translate them, by default,
from English to Dino language, and will translate the y letter too.

If you want to exit the program, close the command line windows or press
Ctrl+C. This will terminate the program.

Command Line Usage:
===================

Ok, if you know how to use the command line, then skip this. If not, then you
should pay attention. It's pretty easy though.

1. Press Start -> Run and type "cmd" (without quotations). Click 'OK'.
2. The Command window will open. Now, you just have to enter the program
   location and the program's options. You can enter the full path of the
   program and the files, or you can drag-and drop them to the command line.
3. Hit ENTER, and the program will do its job.

Ok wait... I got lost. How do I drag and drop the files?

Easy, just open the command window and drag the dino.exe file into the window.
The full pathname will appear. Hit the space bar and enter (the same way)
the file you want to translate. Hit the space bar again and enter the path of
the file you want to make. I suggest simply entering the file you want to 
translate again, and modify the name. And last, enter the options you want to
include.

You may see something like this:

C:\Documents And Settings\User>"C:\Documents And Settings\User\My Documents\
dino.exe" "C:\Documents And Settings\User\My Documents\file.txt" "C:\Documents
And Settings\User\My Documents\translate.txt --y

Hit enter, and let the program do its job. A 1MB file usually takes less than 
a second to process. A larger file may take more time, depending on your
computer.

Version History:
================
v1.0.4 - Fixed a small issue and a typo.
v1.0.3 - Freed up some memory with a small structural change.
v1.0.2 - Fixed some minor issues regarding errors.
v1.0 - First Public Release!
