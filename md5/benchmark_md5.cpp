#include <fstream>
#include <hayai.hpp>
#include "basic_md5.h"

#define DATA_SIZE 1000

class Md5Fixture : public Hayai::Fixture {
public:
	Md5* basic;
	char* data;//[DATA_SIZE];
	ui8* digest;//[DIGEST_SIZE];

	virtual void SetUp() {
		this->basic = new BasicMd5;
		this->data = new char[DATA_SIZE];
		this->digest = new ui8[DIGEST_SIZE];

		std::ifstream in;
		in.open("/dev/urandom", std::ifstream::in);
		in.get(data, DATA_SIZE);
		in.close();
	}

	virtual void TearDown() {
		delete this->basic;
		delete this->data;
		delete this->digest;
	}
};

BENCHMARK_F(Md5Fixture, BasicMd5, 100, 1000) {
	basic->update((ui8*)data, DATA_SIZE);
	basic->update((ui8*)data, DATA_SIZE);
	basic->update((ui8*)data, DATA_SIZE);
	basic->update((ui8*)data, DATA_SIZE);
	basic->update((ui8*)data, DATA_SIZE);
	basic->finish(digest);
}

int main() {
	Hayai::Benchmarker::RunAllTests();
	return 0;
}

