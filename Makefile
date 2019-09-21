all: kdwm

kdwm: kdwm.o window_manager.o
	cc -lX11 kdwm.o window_manager.o -o kdwm

kdwm.o: kdwm.c window_manager.h config.h
	cc -c kdwm.c -o kdwm.o

window_manager.o: window_manager.c window_manager.h config.h
	cc -c window_manager.c -o window_manager.o

clean:
	rm *.o
	rm kdwm

install:
	cp kdwm /bin/kdwm

run: kdwm
	clear
	./kdwm
