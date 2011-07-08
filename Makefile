
all: ringbuftest

ringbuftest: test.o ghpringbuf.o
	$(CC) -o $@ $^
	
clean:
	$(RM) *.o ringbuftest