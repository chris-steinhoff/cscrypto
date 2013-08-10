
HEADERS=md5.h basic_md5.h
OBJS=md5.o basic_md5.o
TEST_INC=/usr/local/include
BENCH_INC=../hayai/src
GTEST_LIB=/usr/local/lib64/libgtest.a

CFLAGS=-ggdb
CXXFLAGS=$(CFLAGS)

.PHONY: clean test benchmark

compile: $(OBJS) $(HEADERS)

test: $(OBJS) $(HEADERS)
	g++ -I$(TEST_INC) -pthread test_md5.cpp $(OBJS) $(GTEST_LIB) -o gtest
	./gtest

benchmark: $(OBJS) $(HEADERS)
	g++ -I$(BENCH_INC) benchmark_md5.cpp $(OBJS) -o benchmarker
	./benchmarker

clean:
	rm -rf *.o *.a *.so.* gtest benchmarker

