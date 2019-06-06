CC=gcc
CFLAGS=-I.

default: clean parallel_min_max

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)
	
wrapper : wrapper.o sequential_min_max
	$(CC) -o wrapper wrapper.o  $(CFLAGS)

wrapper.o : wrapper.c
	$(CC) -o wrapper.o -c wrapper.c $(CFLAGS)
	
sequential_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o sequential_min_max find_min_max.o utils.o sequential_min_max.c $(CFLAGS)



utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

clean :
	rm utils.o find_min_max.o sequential_min_max parallel_min_max wrapper.o wrapper
all: sequential_min_max parallel_min_max wrapper