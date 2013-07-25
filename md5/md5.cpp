
#include <stdio.h>
#include <string.h>
#include "md5.h"

Md5::Md5() :
		len(0),
		A(0x67452301),
		B(0xefcdab89),
		C(0x98badcfe),
		D(0x10325476),
		buffer_index(0) {
}

Md5::~Md5() {
}

void Md5::update(ui8 data[], size_t start, size_t len) {
	// Calculate the remaining size of the buffer
	size_t remaining = CHUNK_SIZE - buffer_index;

	// If there isn't enough data to fill the buffer,
	// add the data and return
	if(len < remaining) {
		// Copy data into the buffer
		memcpy(&buffer[buffer_index], &data[start], len);
		// Update the buffer pointer
		buffer_index += len;
		return;
	}

	// Fill the buffer
	memcpy(&buffer[buffer_index], &data[start], remaining);
	// Reset the buffer pointer
	buffer_index = 0;

	/*
	 * Hash the buffer
	 */

	// Represent buffer as 16 32-bit words
	ui32 words[16] = {
		bytes_to_int(buffer), bytes_to_int(&buffer[4]),
		bytes_to_int(&buffer[8]), bytes_to_int(&buffer[12]),
		bytes_to_int(&buffer[16]), bytes_to_int(&buffer[20]),
		bytes_to_int(&buffer[24]), bytes_to_int(&buffer[28]),
		bytes_to_int(&buffer[32]), bytes_to_int(&buffer[36]),
		bytes_to_int(&buffer[40]), bytes_to_int(&buffer[44]),
		bytes_to_int(&buffer[48]), bytes_to_int(&buffer[52]),
		bytes_to_int(&buffer[56]), bytes_to_int(&buffer[60])
	};

	// Get the current state to manipulate
	ui32 a = A, b = B, c = C, d = D;
	size_t i;

	// First round
	for( ; i < 16 ; ++i) {
		ui32 f = (b & c) | ((!b) & d);
		ui32 g = i;
		// Rotate function
		ui32 z = (a + f + salts[i] + words[g]);
		// Shift state
		ui32 tmp = d;
		d = c;
		c = b;
		b = b + ((z << shifts[i]) | (z >> (32-shifts[i])));
		a = tmp;
	}

	// Add to the hash
	A += a;
	B += b;
	C += c;
	D += d;

	// If there is more data then what remained in the buffer,
	// add the left over to the buffer
	if(len > remaining) {
		// Copy left over data
		size_t left_over = len - remaining;
		memcpy(buffer, &data[start + remaining], left_over);
		// Update the buffer pointer
		buffer_index = left_over;
	}
}

std::string Md5::finish() {
	return finish(NULL, 0, 0);
}

std::string Md5::finish(ui8 data[], unsigned int start,
		unsigned int len) {
	if(data) {
		update(data, start, len);
	}

	ui8 d[CHUNK_SIZE] = {
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	update(d, 0, CHUNK_SIZE);

	char hash[32];
	ui32 h[4] = { A, B, C, D };
	for(int i = 0, k = 0 ; i < 4 ; ++i) {
		for(int j = 28 ; j >= 0 ; j -= 4) {
			printf("k = %d ;i = %d ; j = %d ; (h[i] >> j) & 0x0f = %d ; hex = %c\n", k, i, j, (h[i] >> j) & 0x0f, hex[(int)((h[i] >> j) & 0x0f)]);
			int x = (h[i] >> j) & 0x0f;
			hash[k++] = hex[x];
		}
	}
printf("%s\n", hex);

	return std::string(hash, 32);
}

void Md5::int_to_bytes(ui8* bytes, ui32 intt) {
	bytes[0] = (ui8)intt;
    bytes[1] = (ui8)(intt >> 8);
    bytes[2] = (ui8)(intt >> 16);
    bytes[3] = (ui8)(intt >> 24);
}

ui32 Md5::bytes_to_int(ui8* bytes) {
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

