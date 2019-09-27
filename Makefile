all: kdwm

kdwm: kdwm.c config.h definitions.h
	cc -lX11 kdwm.c -o kdwm

clean:
	rm kdwm

install:
	cp kdwm /bin/kdwm

run: kdwm
	clear
	./kdwm
