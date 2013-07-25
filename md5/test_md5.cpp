
#include <iostream>
#include "md5.h"

int main() {
	std::string str = "";
	std::string expected = "d41d8cd98f00b204e9800998ecf8427e";

	Md5 md;
	std::string hash = md.finish();
	if(expected.compare(hash) != 0) {
		std::cerr << "FAILED:" << std::endl << "  expected: " << expected <<
			std::endl << "    actual: " << hash << std::endl;
	}

	str = "The quick brown fox jumps over the lazy dog";
	expected = "9e107d9d372bb6826bd81d3542a419d6";

	Md5 md_dog;
	md.update((uint8_t*)str.data(), 0, str.length());
	hash = md_dog.finish();
	if(expected.compare(hash) != 0) {
		std::cerr << "FAILED:" << std::endl << "  expected: " << expected <<
			std::endl << "    actual: " << hash << std::endl;
	}

	return 0;
}

