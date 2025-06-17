CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: tsim

tsim: tester.o sim.o
	$(CC) $(CFLAGS) -o tsim tester.o sim.o

tester.o: tester.c
	$(CC) $(CFLAGS) -c tester.c -o tester.o

sim.o: sim.c
	$(CC) $(CFLAGS) -c sim.c -o sim.o

clean:
	rm -f tsim *.o