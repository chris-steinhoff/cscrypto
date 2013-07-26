
#include <iostream>
#include "md5.h"

int main() {
	ui8 digest[16];
	std::string str = "";
	std::string expected = "d41d8cd98f00b204e9800998ecf8427e";

	Md5 md;
	md.finish(digest);
	std::string hash = Md5::digest_to_string(digest);
	if(expected.compare(hash) != 0) {
		std::cerr << "FAILED:" << std::endl << "  expected: " << expected <<
			std::endl << "    actual: " << hash << std::endl;
	}

	str = "The quick brown fox jumps over the lazy dog";
	expected = "9e107d9d372bb6826bd81d3542a419d6";

	Md5 md_dog;
	md.update((uint8_t*)str.data(), 0, str.length());
	md.finish(digest);
	hash = Md5::digest_to_string(digest);
	if(expected.compare(hash) != 0) {
		std::cerr << "FAILED:" << std::endl << "  expected: " << expected <<
			std::endl << "    actual: " << hash << std::endl;
	}

	return 0;
}

