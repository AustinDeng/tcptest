main: utils.o server.o transaction.o
	gcc utils.o server.o transaction.o test.c -o test -lpthread

server.o: server.h
	gcc -c server.c

utils.o: utils.h
	gcc -c utils.c

transaction.o: transaction.h
	gcc -c transaction.c

.PHONY: test
test:
	./test

clean:
	rm -rf utils.o
	rm -rf server.o
	rm -rf transaction.o
	rm -rf test
