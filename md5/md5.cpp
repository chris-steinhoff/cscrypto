
#include "md5.h"

extern "C" { void *memcpy(void *destination, const void *source, size_t num); }

Md5::Md5() :
		length(0),
		A(0x67452301),
		B(0xefcdab89),
		C(0x98badcfe),
		D(0x10325476),
		buffer_index(0) {
}

Md5::~Md5() {
}

std::string Md5::digest_to_string(const ui8 digest[DIGEST_SIZE]) {
	char hash[DIGEST_SIZE * 2];
	for(unsigned char i = 0, j = 0 ; i < DIGEST_SIZE ; ++i) {
		hash[j++] = hex[(digest[i] >> 4) & 0x0f];
		hash[j++] = hex[digest[i] & 0x0f];
	}

	return std::string(hash, DIGEST_SIZE * 2);
}

void Md5::update(const ui8* const data, size_t len) {
	// Update length
	length += len;

	// Calculate the remaining size of the buffer.
	size_t available = CHUNK_SIZE - buffer_index;
	// The loop needs to track of where in the data to start copying from.
	size_t start = 0;
	// Keep filling up the buffer until all data has been processed.
	while(len > 0) {
		if(len < available) {
			// Add the data to the buffer and return
			memcpy(&buffer[buffer_index], data + start, len);
			buffer_index += len;
			len = 0;
		} else {
			// Fill the buffer, hash it,
			// and continue the loop with the remaining data.
			memcpy(&buffer[buffer_index], data + start, available);
			hash_buffer();
			buffer_index = 0;
			len -= available;
			start += available;
			available = CHUNK_SIZE;
		}
	}
}

// 64 - 8 = 56
#define LENGTH_WORD_INDEX 56

void Md5::finish(ui8 digest[DIGEST_SIZE]) {
	// Append the 0x80 terminator
	buffer[buffer_index++] = 0x80;
	// If the terminator filled the buffer, hash it.
	if(buffer_index == CHUNK_SIZE) {
		hash_buffer();
		buffer_index = 0;
	}
	// If there isn't enough room for the length word,
	// fill the buffer, hash it, then hash another buffer with padding.
	else if(buffer_index > LENGTH_WORD_INDEX) {
		memcpy(&buffer[buffer_index], padding, CHUNK_SIZE - buffer_index);
		hash_buffer();
		buffer_index = 0;
	}

	// Append 0 padding upto the length word.
	memcpy(&buffer[buffer_index], padding, LENGTH_WORD_INDEX - buffer_index);

	// Append length
	// Set the last 64-bits to the legth, least significant 32-bit word first.
	// There are 16 32-bit words in the 512-bit buffer.
	((ui32*)buffer)[14] = (ui32)((length * 8) & 0x00000000ffffffff);
	((ui32*)buffer)[15] = (ui32)((length >> 29) & 0x00000000ffffffff);

	// Hash the buffer for the last time.
	hash_buffer();

	// Concatinate the state variables into the digest.
	int_to_bytes(&digest[ 0], A);
	int_to_bytes(&digest[ 4], B);
	int_to_bytes(&digest[ 8], C);
	int_to_bytes(&digest[12], D);
}

void Md5::hash_buffer() {
	ui32 a = A, b = B, c = C, d = D;
	ui32 f, g, z, temp;
	unsigned char i = 0;

	// Represent 512-bit buffer as 16 32-bit words
	const ui32 words[16] = {
		bytes_to_int(buffer), bytes_to_int(&buffer[4]),
		bytes_to_int(&buffer[8]), bytes_to_int(&buffer[12]),
		bytes_to_int(&buffer[16]), bytes_to_int(&buffer[20]),
		bytes_to_int(&buffer[24]), bytes_to_int(&buffer[28]),
		bytes_to_int(&buffer[32]), bytes_to_int(&buffer[36]),
		bytes_to_int(&buffer[40]), bytes_to_int(&buffer[44]),
		bytes_to_int(&buffer[48]), bytes_to_int(&buffer[52]),
		bytes_to_int(&buffer[56]), bytes_to_int(&buffer[60])
	};

#define ROTATION \
temp = d;\
d = c;\
c = b;\
z = (a + f + salts[i] + words[g]);\
b = b + ((z << shifts[i]) | (z >> (32-shifts[i])));\
a = temp;

	// First round
	for( ; i < 16 ; ++i) {
		f = (b & c) | ((~b) & d);
		g = i;
		ROTATION
	}

	// Second round
	for( ; i < 32 ; ++i) {
		f = (d & b) | ((~d) & c);
		g = (5 * i + 1) % 16;
		ROTATION
	}

	// Third round
	for( ; i < 48 ; ++i) {
		f = b ^ c ^ d;
		g = (3 * i + 5) % 16;
		ROTATION
	}

	// Forth round
	for( ; i < 64 ; ++i) {
		f = c ^ (b | (~d));
		g = (7 * i) % 16;
		ROTATION
	}

	// Update hash state
	A += a;
	B += b;
	C += c;
	D += d;
}

void Md5::int_to_bytes(ui8 bytes[WORD_SIZE], const ui32 intt) {
	bytes[0] = (ui8)intt;
    bytes[1] = (ui8)(intt >> 8);
    bytes[2] = (ui8)(intt >> 16);
    bytes[3] = (ui8)(intt >> 24);
}

ui32 Md5::bytes_to_int(const ui8 bytes[4]) {
	return (ui32) bytes[0]
        | ((ui32) bytes[1] << 8)
        | ((ui32) bytes[2] << 16)
        | ((ui32) bytes[3] << 24);
}

const ui32 Md5::salts[64] = {
0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

const ui32 Md5::shifts[64] = {
7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

const char Md5::hex[17] = {
'0', '1', '2', '3', '4', '5', '6', '7',
'8', '9', 'a', 'b', 'c', 'd', 'e', 'f', NULL
};

const ui8 Md5::padding[63] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

