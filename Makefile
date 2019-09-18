all: wwm

wwm: wwm.o
	cc wwm.o -o wwm

wwm.o: wwm.c config.h
	cc -c wwm.c -o wwm.o

clean:
	rm *.o
	rm wwm

install:
	cp wwm /bin/wwm

run: wwm
	clear
	./wwm
