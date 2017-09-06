CC=gcc
CFLAGS=-I. -O2 --std=c99

LDFLAGS=-lm

DEP = ttaverify.h crc32.h

OBJ = crc32.o main.o ttaverify.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ttaverify: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f *.o