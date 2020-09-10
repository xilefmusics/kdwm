all: kdwm

force: clean kdwm

kdwm: kdwm.c config.h definitions.h
	cc -lX11 -lXinerama -lpthread kdwm.c -o kdwm

clean:
	rm kdwm

install:
	cp -f kdwm /bin/kdwm

run: kdwm
	clear
	./kdwm
