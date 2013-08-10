
#include "sse_md5.h"

SseMd5::SseMd5() :
		length(0),
		A(0x67452301),
		B(0xefcdab89),
		C(0x98badcfe),
		D(0x10325476),
		buffer_index(0) {
}

SseMd5::~SseMd5() {
}

void SseMd5::update(const ui8* const data, size_t len) {
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

void SseMd5::finish(ui8 digest[DIGEST_SIZE]) {
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
	Md5::int_to_bytes(&digest[ 0], A);
	Md5::int_to_bytes(&digest[ 4], B);
	Md5::int_to_bytes(&digest[ 8], C);
	Md5::int_to_bytes(&digest[12], D);
}

void SseMd5::hash_buffer() {
	ui32 a = A, b = B, c = C, d = D;
	ui32 f, g, z, temp;
	unsigned char i = 0;

	// Represent 512-bit buffer as 16 32-bit words
	const ui32 words[16] = {
		Md5::bytes_to_int(buffer),      Md5::bytes_to_int(&buffer[4]),
		Md5::bytes_to_int(&buffer[8]),  Md5::bytes_to_int(&buffer[12]),
		Md5::bytes_to_int(&buffer[16]), Md5::bytes_to_int(&buffer[20]),
		Md5::bytes_to_int(&buffer[24]), Md5::bytes_to_int(&buffer[28]),
		Md5::bytes_to_int(&buffer[32]), Md5::bytes_to_int(&buffer[36]),
		Md5::bytes_to_int(&buffer[40]), Md5::bytes_to_int(&buffer[44]),
		Md5::bytes_to_int(&buffer[48]), Md5::bytes_to_int(&buffer[52]),
		Md5::bytes_to_int(&buffer[56]), Md5::bytes_to_int(&buffer[60])
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

