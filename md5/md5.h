
#ifndef  md5_INC
#define  md5_INC

#include <stdint.h>
#include <string>

#define DIGEST_SIZE 16
#define CHUNK_SIZE 64
#define WORD_SIZE 4

typedef uint8_t ui8;
typedef uint32_t ui32;
typedef uint64_t ui64;

class Md5 {
private:
	const static ui32 salts[64];
	const static ui32 shifts[64];
	const static char hex[17];
	const static ui8  padding[63];

	ui64 length;
	ui32 A, B, C, D;
	ui8 buffer[CHUNK_SIZE];
	size_t buffer_index;

	static void int_to_bytes(ui8* bytes, const ui32 intt);
	static ui32 bytes_to_int(const ui8 bytes[WORD_SIZE]);

	void hash_buffer();

public:
	Md5();
	virtual ~Md5();

 	static std::string digest_to_string(const ui8 digest[DIGEST_SIZE]);

	void update(const ui8* const data, size_t len);
	void finish(ui8 digest[DIGEST_SIZE]);

};

#endif   /* ----- #ifndef md5_INC  ----- */

