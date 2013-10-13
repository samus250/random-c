#Makefile for fabitest.cpp

fabitest.exe: fabitest.o
	g++ -s -o fabitest.exe fabitest.o -lcurl -lz -lwldap32 -lws2_32

fabitest.o: fabitest.cpp
	g++ -O2 -Wall -DCURL_STATICLIB -DHAVE_LIBZ -DHAVE_ZLIB_H -c fabitest.cpp

