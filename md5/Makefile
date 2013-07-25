
HEADERS=md5.h
OBJS=md5.o
TESTS=test_md5.cpp

CFLAGS=-ggdb
CXXFLAGS=$(CFLAGS)

.PHONY: clean

compile: $(OBJS) $(HEADERS)

md5.o: md5.h

test: $(TESTS) $(OBJS)
	g++ -o test $(TESTS) $(OBJS)
	./test

clean:
	rm -rf *.o *.a *.so.*

