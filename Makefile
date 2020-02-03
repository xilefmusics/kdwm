all: kdwm_dev

force: clean kdwm_dev

kdwm_dev: kdwm.c config.h definitions.h
	cc -lX11 -lXinerama kdwm.c -o kdwm_dev

clean:
	rm kdwm_dev

install:
	cp -f kdwm_dev /bin/kdwm_dev

kill:
	pkill -9 -f Xephyr

run: force
	Xephyr -br -ac -resizeable -no-host-grab :1 &
	sleep 1
	DISPLAY=:1 ./kdwm_dev
