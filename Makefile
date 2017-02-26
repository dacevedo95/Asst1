all: memgrind.c mymalloc.o mymalloc.h
	gcc memgrind.c mymalloc.o

mymalloc.o: mymalloc.c mymalloc.h
	gcc -c mymalloc.c

clean:
	rm *.o ; rm a.out
