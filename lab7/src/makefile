CC=gcc
CFLAGS=-I.


all: tcpclient tcpserver udpserver udpclient

tcpserver : tcpserver.o 
	$(CC) -o tcpserver tcpserver.o $(CFLAGS)

tcpclient: tcpclient.o 
	$(CC) -o tcpclient tcpclient.o  $(CFLAGS)

udpclient: udpclient.o 
	$(CC) -o udpclient udpclient.o  $(CFLAGS)

udpserver.o: udpserver.c
	$(CC) -o udpserver.o -c udpserver.c $(CFLAGS)

tcpserver.o: tcpserver.c
	$(CC) -o tcpserver.o -c tcpserver.c $(CFLAGS)

utils : utils.o
	$(CC) -o utils utils.o -lpthread  $(CFLAGS)

utils.o: utils.c
	$(CC) -o utils.o -c utils.c $(CFLAGS)

tcpclient.o: tcpclient.c
	$(CC) -o tcpclient.o -c tcpclient.c $(CFLAGS)

clean: 
	rm *.o