duMalloc: duMalloc.o mallocTestVersion4.o
	gcc duMalloc.o mallocTestVersion4.o -o proj1

duMalloc.o: duMalloc.c duMalloc.h
	gcc duMalloc.c -c

mallocTestVersion3.o: mallocTestVersion4.c duMalloc.h
	gcc mallocTestVersion4.c -c

clean:
	rm -f *.o
	rm -f proj1