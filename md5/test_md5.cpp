
#include <gtest/gtest.h>
#include <iostream>
#include "md5.h"
#include "basic_md5.h"
#include "sse_md5.h"

ui8 digest[16];

template <class T>
Md5* createMd5Impl();

template <>
Md5* createMd5Impl<BasicMd5>() {
	return new BasicMd5();
}

template <>
Md5* createMd5Impl<SseMd5>() {
	return new SseMd5();
}

template <typename T>
class Md5Test : public ::testing::Test {
public:
	Md5* const md;

	Md5Test() : md(createMd5Impl<T>()) {}
	virtual ~Md5Test() { delete md; }

};

typedef ::testing::Types<BasicMd5, SseMd5> Md5Impls;
TYPED_TEST_CASE(Md5Test, Md5Impls);

/*INSTANTIATE_TEST_CASE_P(Md5Impls, Md5Test,
		::testing::Values(new BasicMd5(), new SseMd5()));*/

TYPED_TEST(Md5Test, NoUpdate) {
	this->md->finish(digest);
	ASSERT_STREQ(
		"d41d8cd98f00b204e9800998ecf8427e",
		Md5::digest_to_string(digest).c_str()
	);
}

TYPED_TEST(Md5Test, EmptyUpdate) {
	this->md->update(NULL, 0);
	this->md->finish(digest);
	ASSERT_STREQ(
		"d41d8cd98f00b204e9800998ecf8427e",
		Md5::digest_to_string(digest).c_str()
	);
}

TYPED_TEST(Md5Test, Short_SingleUpdate) {
	std::string str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	this->md->update((uint8_t*)str.data(), str.length());
	this->md->finish(digest);
	ASSERT_STREQ(
		"f29939a25efabaef3b87e2cbfe641315",
		Md5::digest_to_string(digest).c_str()
	);
}

TYPED_TEST(Md5Test, Long_SingleUpdate) {
	std::string str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	                  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	                  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	                  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	this->md->update((uint8_t*)str.data(), str.length());
	this->md->finish(digest);
	ASSERT_STREQ(
		"0269bb6c2060579ecfd687c025ae2b47",
		Md5::digest_to_string(digest).c_str()
	);
}

TYPED_TEST(Md5Test, Short_MultiUpdate) {
	std::string str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	for(char i = 0 ; i < 2 ; ++i) {
		this->md->update((uint8_t*)str.data() + str.length()/2*i, str.length()/2);
	}
	this->md->finish(digest);
	ASSERT_STREQ(
		"f29939a25efabaef3b87e2cbfe641315",
		Md5::digest_to_string(digest).c_str()
	);
}

TYPED_TEST(Md5Test, Long_MultiUpdate) {
	std::string str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	                  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	                  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	                  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	for(char i = 0 ; i < 4 ; ++i) {
		this->md->update((uint8_t*)str.data() + str.length()/4*i, str.length()/4);
	}
	this->md->finish(digest);
	ASSERT_STREQ(
		"0269bb6c2060579ecfd687c025ae2b47",
		Md5::digest_to_string(digest).c_str()
	);
}

// Should add 0x80 byte and no 0 padding
TYPED_TEST(Md5Test, FinishOneLessThenLengthBoundary) {
	std::string str = "123456789012345678901234567890"
	                  "1234567890123456789012345";
	this->md->update((uint8_t*)str.data(), str.length());
	this->md->finish(digest);
	ASSERT_STREQ(
		"c9ccf168914a1bcfc3229f1948e67da0",
		Md5::digest_to_string(digest).c_str()
	);
}

// Should add 0x80 byte, add 0 padding to fill buffer and hash the buffer,
// then fill another buffer with padding and hash that.
TYPED_TEST(Md5Test, FinishOnLengthBoundary) {
	std::string str = "123456789012345678901234567890"
	                  "12345678901234567890123456";
	this->md->update((uint8_t*)str.data(), str.length());
	this->md->finish(digest);
	ASSERT_STREQ(
		"49f193adce178490e34d1b3a4ec0064c",
		Md5::digest_to_string(digest).c_str()
	);
}

// Should add 0x80 byte, add 0 padding to fill buffer and hash the buffer,
// then fill another buffer with padding and hash that.
TYPED_TEST(Md5Test, FinishWithinLengthWord) {
	std::string str = "123456789012345678901234567890"
	                  "123456789012345678901234567890";
	this->md->update((uint8_t*)str.data(), str.length());
	this->md->finish(digest);
	ASSERT_STREQ(
		"c5b549377c826cc3712418b064fc417e",
		Md5::digest_to_string(digest).c_str()
	);
}

TYPED_TEST(Md5Test, FinishOneLessThenBoundary) {
	std::string str = "123456789012345678901234567890"
	                  "123456789012345678901234567890"
	                  "123";
	this->md->update((uint8_t*)str.data(), str.length());
	this->md->finish(digest);
	ASSERT_STREQ(
		"c3eb67ece68488bb394241d4f6a54244",
		Md5::digest_to_string(digest).c_str()
	);
}

// Should add 0x80 byte and a full buffer (minus one) of padding
TYPED_TEST(Md5Test, FinishOnBoundary) {
	std::string str = "123456789012345678901234567890"
	                  "123456789012345678901234567890"
	                  "1234";
	this->md->update((uint8_t*)str.data(), str.length());
	this->md->finish(digest);
	ASSERT_STREQ(
		"eb6c4179c0a7c82cc2828c1e6338e165",
		Md5::digest_to_string(digest).c_str()
	);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

