# Makefile for dirlist.c

dirlist.exe: dirlist.o file_init.o main.o
	gcc main.o dirlist.o file_init.o -o dirlist.exe

file_init.o: file_init.c dirlist.h
	gcc -c file_init.c
	
main.o: main.c dirlist.h
	gcc -c main.c

dirlist.o: dirlist.c dirlist.h
	gcc -c dirlist.c
	
clean:
	rm -f dirlist.o main.o file_init.o