all: kdwmc

kdwmc: kdwmc.c
	cc -o kdwmc kdwmc.c

clean:
	rm -f kdwmc

install: kdwmc
	cp -f kdwmc /bin/kdwmc

run: kdwmc
	./kdwmc
