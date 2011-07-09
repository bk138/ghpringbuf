
all: ringbuftest

ringbuftest: ringbuftest.o ghpringbuf.o
	$(CC) -o $@ $^

clean:
	$(RM) *.o ringbuftest