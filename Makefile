CC=gcc
CFLAGS=-O2 -march=native -std=gnu11 -Wall
PROGNAME=inode64

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROGNAME): inode64.o
	$(CC) $(CFLAGS) -o inode64 inode64.o

clean:
	rm -f $(PROGNAME) *.o
