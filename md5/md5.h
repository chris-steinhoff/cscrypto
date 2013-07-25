
#ifndef  md5_INC
#define  md5_INC

#include <stdint.h>
#include <string>

#define CHUNK_SIZE 64

typedef uint8_t ui8;
typedef uint32_t ui32;
typedef uint64_t ui64;

class Md5 {
private:
	const static ui32 salts[64];
	const static ui32 shifts[64];
	const static char hex[17];

	ui64 len;
	ui32 A, B, C, D;
	ui8 buffer[CHUNK_SIZE];
	size_t buffer_index;

	void int_to_bytes(ui8* bytes, ui32 intt);
	ui32 bytes_to_int(ui8* bytes);

public:
	Md5();
	virtual ~Md5();

	void update(ui8 data[], size_t start, size_t len);
	std::string finish();
	std::string finish(ui8 data[], unsigned int start, unsigned int len);
};

#endif   /* ----- #ifndef md5_INC  ----- */

