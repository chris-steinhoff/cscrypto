
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
	} else {
		std::cout << "PASSED" << std::endl;
	}

	str = "The quick brown fox jumps over the lazy dog";
	expected = "9e107d9d372bb6826bd81d3542a419d6";

	Md5 md_dog;
	md_dog.update((uint8_t*)str.data(), 0, str.length());
	md_dog.finish(digest);
	hash = Md5::digest_to_string(digest);
	if(expected.compare(hash) != 0) {
		std::cerr << "FAILED:" << std::endl << "  expected: " << expected <<
			std::endl << "    actual: " << hash << std::endl;
	} else {
		std::cout << "PASSED" << std::endl;
	}

	str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	expected = "f29939a25efabaef3b87e2cbfe641315";

	Md5 md_alpha;
	md_alpha.update((uint8_t*)str.data(), 0, str.length());
	md_alpha.finish(digest);
	hash = Md5::digest_to_string(digest);
	if(expected.compare(hash) != 0) {
		std::cerr << "FAILED:" << std::endl << "  expected: " << expected <<
			std::endl << "    actual: " << hash << std::endl;
	} else {
		std::cout << "PASSED" << std::endl;
	}

	str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	expected = "8c0b45ac70826fd5e9e12800bb53ccee";

	Md5 md_medium_alpha;
	md_medium_alpha.update((uint8_t*)str.data(), 0, str.length());
	md_medium_alpha.finish(digest);
	hash = Md5::digest_to_string(digest);
	if(expected.compare(hash) != 0) {
		std::cerr << "FAILED:" << std::endl << "  expected: " << expected <<
			std::endl << "    actual: " << hash << std::endl;
	} else {
		std::cout << "PASSED" << std::endl;
	}

	str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	expected = "0269bb6c2060579ecfd687c025ae2b47";

	Md5 md_long_alpha;
	md_long_alpha.update((uint8_t*)str.data(), 0, str.length());
	md_long_alpha.finish(digest);
	hash = Md5::digest_to_string(digest);
	if(expected.compare(hash) != 0) {
		std::cerr << "FAILED:" << std::endl << "  expected: " << expected <<
			std::endl << "    actual: " << hash << std::endl;
	} else {
		std::cout << "PASSED" << std::endl;
	}

	return 0;
}

