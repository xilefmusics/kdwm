all: clean kdwm

kdwm:
	cc -lX11 kdwm.c -o kdwm

clean:
	rm kdwm

install:
	cp -f kdwm /bin/kdwm

run: kdwm
	clear
	./kdwm
