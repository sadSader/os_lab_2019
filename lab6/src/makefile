CC=gcc
CFLAGS=-I.


test_server: clean all servers_start
	./client --k 5 --mod 10 --servers ./servers.txt

servers_start: server
	./server --tnum 4 --port 20001&
	./server --tnum 4 --port 20002&
	./server --tnum 4 --port 20003&
all: client server

server : server.o utils.o
	$(CC) -o server server.o utils.o -lpthread  $(CFLAGS)

client: client.o utils.o
	$(CC) -o client client.o utils.o  $(CFLAGS)

server.o: server.c
	$(CC) -o server.o -c server.c $(CFLAGS)

utils : utils.o
	$(CC) -o utils utils.o -lpthread  $(CFLAGS)

utils.o: utils.c
	$(CC) -o utils.o -c utils.c $(CFLAGS)

client.o: client.c
	$(CC) -o client.o -c client.c $(CFLAGS)

clean: 
	pkill server
	rm *.o