main: append.o server.o
	gcc append.o server.o test.c -o test

server.o: server.h
	gcc -c server.c

append.o: append.h
	gcc -c append.c

.PHONY: test
test:
	./test

clean:
	rm -rf append.o
	rm -rf server.o
	rm -rf test

