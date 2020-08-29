all: kdwmc

kdwmc_dev: kdwmc.c
	cc -o kdwmc_dev kdwmc.c

clean:
	rm -f kdwmc_dev

install: kdwmc_dev
	cp -f kdwmc_dev /bin/kdwmc_dev

run: kdwmc_dev
	./kdwmc_dev
