
#ifndef  sse_md5_INC
#define  sse_md5_INC 1

#include "md5.h"

class SseMd5 : public Md5 {
private:
	ui64 length;
	ui32 A, B, C, D;
	ui8 buffer[CHUNK_SIZE];
	size_t buffer_index;

	void hash_buffer();

public:
	SseMd5();
	virtual ~SseMd5();

	void update(const ui8* const data, size_t len);
	void finish(ui8 digest[DIGEST_SIZE]);

};

#endif    /* ----- #ifndef sse_md5_INC ----- */

