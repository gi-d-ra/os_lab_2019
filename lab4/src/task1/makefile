CC=gcc
CFLAGS=-I.

# ./parallel_min_max.out --seed 3 --array_size 10 --pnum 2 --timeout 10
# ./parallel_min_max.out --seed 6 --array_size 1000000000 --pnum 2 --timeout 1

all : parallel_min_max

parallel_min_max : utils.o find_min_max utils.h find_min_max.h
	$(CC) -o parallel_min_max.out utils.o find_min_max.o parallel_min_max.c -lm $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

clean :
	rm utils.o find_min_max.o parallel_min_max.out -f 2> /dev/null
