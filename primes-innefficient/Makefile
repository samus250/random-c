#Makefile for primes program

objects = main.o err.o fprint.o time.o primegen.o

primes: $(objects)
	gcc $(objects) -o primes

main.o: main.c primes.h
	gcc -c main.c

err.o: err.c primes.h
	gcc -c err.c

fprint.o: fprint.c primes.h
	gcc -c fprint.c
	
time.o: time.c primes.h
	gcc -c time.c

primegen.o: primegen.c primes.h
	gcc -c primegen.c

clean:
	rm -f $(objects)