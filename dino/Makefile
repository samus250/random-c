#Makefile for dino program

dino: main.o a_m.o e_d.o d_e.o
	gcc main.o a_m.o e_d.o d_e.o -o dino
	
main.o: main.c dino.h
	gcc -c main.c

a_m.o: a_m.c dino.h
	gcc -c a_m.c

e_d.o: e_d.c dino.h
	gcc -c e_d.c
	
d_e.o: d_e.c dino.h
	gcc -c d_e.c
	
clean:
	rm -f d_e.o e_d.o a_m.o main.o