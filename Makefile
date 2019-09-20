all: kdwm

kdwm: kdwm.o
	cc -lX11 kdwm.o -o kdwm

kdwm.o: kdwm.c config.h
	cc -c kdwm.c -o kdwm.o

clean:
	rm *.o
	rm kdwm

install:
	cp kdwm /bin/kdwm

run: kdwm
	clear
	./kdwm
