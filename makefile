CFLAGS=-Wall
LIBS=-lcheck -lm -lpthread -lrt

test: rpn-converter-test
	./rpn-converter-test

rpn-converter-test: rpn-converter-test.o rpn-converter.o
	gcc -o rpn-converter-test rpn-converter.o rpn-converter-test.o $(LIBS)

rpn-converter-test.o: rpn-converter-test.c rpn-converter.h
	gcc $(CFLAGS) -c rpn-converter-test.c

rpn-converter-test.c: rpn-converter-test.check
	checkmk rpn-converter-test.check > rpn-converter-test.c

rpn-converter.o: rpn-converter.c rpn-converter.h
	gcc $(CFLAGS) -c rpn-converter.c
