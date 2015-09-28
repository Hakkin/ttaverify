CC=gcc
CFLAGS=-I. -O2 --std=c99

LIBS=-lm

DEP = ttaverify.h crc32.h

OBJ = crc32.o main.o ttaverify.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ttaverify: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o