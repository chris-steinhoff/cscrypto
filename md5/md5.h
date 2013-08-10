
#ifndef  md5_INC
#define  md5_INC 1

#include <stdint.h>
#include <string>

#define DIGEST_SIZE 16
#define CHUNK_SIZE 64
#define WORD_SIZE 4

typedef uint8_t ui8;
typedef uint32_t ui32;
typedef uint64_t ui64;

extern "C" { void *memcpy(void *destination, const void *source, size_t num); }

class Md5 {
public:
	static const ui32 salts[64];
	static const ui32 shifts[64];
	static const char hex[17];
	static const ui8  padding[63];

	static void int_to_bytes(ui8* bytes, const ui32 intt);
	static ui32 bytes_to_int(const ui8 bytes[WORD_SIZE]);
 	static std::string digest_to_string(const ui8 digest[DIGEST_SIZE]);

	virtual void update(const ui8* const data, size_t len)=0;
	virtual void finish(ui8 digest[DIGEST_SIZE])=0;

};

#endif   /* ----- #ifndef md5_INC  ----- */

