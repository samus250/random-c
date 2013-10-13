#Makefile for primes program

objects = main.o err.o fprint.o time.o semiprime_gen.o

semiprimes: $(objects)
	gcc $(objects) -o semiprimes

main.o: main.c primes.h
	gcc -c main.c

err.o: err.c primes.h
	gcc -c err.c

fprint.o: fprint.c primes.h
	gcc -c fprint.c
	
time.o: time.c primes.h
	gcc -c time.c

semiprime_gen.o: semiprime_gen.c primes.h
	gcc -c semiprime_gen.c

clean:
	rm -f $(objects)
