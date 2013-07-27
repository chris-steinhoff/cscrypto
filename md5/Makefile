
HEADERS=md5.h
OBJS=basic_md5.o
TEST_OBJS=test_md5.o
TEST_INC=/usr/local/include
GTEST_LIB=/usr/local/lib64/libgtest.a

CFLAGS=-ggdb
CXXFLAGS=$(CFLAGS)

.PHONY: clean test

compile: $(OBJS) $(HEADERS)

test: $(OBJS)
	g++ -I$(TEST_INC) -pthread test_md5.cpp $(OBJS) $(GTEST_LIB) -o gtest
	./gtest

clean:
	rm -rf *.o *.a *.so.* gtest

