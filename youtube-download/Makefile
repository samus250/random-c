#Makefile for Youtube Downloader

CC = gcc
CFLAGS = -g -O2 -Wall -DCURL_STATICLIB -DHAVE_LIBZ -DHAVE_ZLIB_H
LDFLAGS = -s
LIBS = -lcurl -lz -lwldap32 -lws2_32

LINK = $(CC) $(LDFLAGS)
COMPILE = $(CC) $(CFLAGS)

OBJS = main.o updater.o downloader.o progress.o

youtube_downloader.exe: $(OBJS)
	$(LINK) $(OBJS) $(LIBS) -o youtube_downloader.exe

main.o: main.c youtube_downloader.h
	$(COMPILE) -c -o main.o main.c

updater.o: updater.c progress.o youtube_downloader.h
	$(COMPILE) -c -o updater.o updater.c

downloader.o: downloader.c progress.o youtube_downloader.h
	$(COMPILE) -c -o downloader.o downloader.c


progress.o: progress.c
	$(COMPILE) -c -o progress.o progress.c

clean:
	rm -rf $(OBJS)