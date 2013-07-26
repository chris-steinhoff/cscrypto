
#include <gtest/gtest.h>
#include <iostream>
#include "md5.h"

ui8 digest[16];

TEST(MD5, NoUpdate) {
	Md5 md;
	md.finish(digest);
	ASSERT_STREQ(
		"d41d8cd98f00b204e9800998ecf8427e",
		Md5::digest_to_string(digest).c_str()
	);
}

TEST(MD5, EmptyUpdate) {
	Md5 md;
	md.update(NULL, 0, 0);
	md.finish(digest);
	ASSERT_STREQ(
		"d41d8cd98f00b204e9800998ecf8427e",
		Md5::digest_to_string(digest).c_str()
	);
}

TEST(MD5, Short_SingleUpdate) {
	Md5 md;
	std::string str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	md.update((uint8_t*)str.data(), 0, str.length());
	md.finish(digest);
	ASSERT_STREQ(
		"f29939a25efabaef3b87e2cbfe641315",
		Md5::digest_to_string(digest).c_str()
	);
}

TEST(MD5, Long_SingleUpdate) {
	Md5 md;
	std::string str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	                  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	                  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	                  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	md.update((uint8_t*)str.data(), 0, str.length());
	md.finish(digest);
	ASSERT_STREQ(
		"0269bb6c2060579ecfd687c025ae2b47",
		Md5::digest_to_string(digest).c_str()
	);
}

TEST(MD5, Short_MultiUpdate) {
	Md5 md;
	std::string str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	for(char i = 0 ; i < 2 ; ++i) {
		md.update((uint8_t*)str.data(), str.length()/2*i, str.length()/2);
	}
	md.finish(digest);
	ASSERT_STREQ(
		"f29939a25efabaef3b87e2cbfe641315",
		Md5::digest_to_string(digest).c_str()
	);
}

TEST(MD5, Long_MultiUpdate) {
	Md5 md;
	std::string str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	                  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	                  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	                  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	for(char i = 0 ; i < 4 ; ++i) {
		md.update((uint8_t*)str.data(), str.length()/4*i, str.length()/4);
	}
	md.finish(digest);
	ASSERT_STREQ(
		"0269bb6c2060579ecfd687c025ae2b47",
		Md5::digest_to_string(digest).c_str()
	);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

