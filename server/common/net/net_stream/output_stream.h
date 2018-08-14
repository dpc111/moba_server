#ifndef OUTPUT_STREAM_H
#define OUTPUT_STREAM_H

#include "chunk.h"

class output_stream_t
{
public:
	output_stream_t();

	~output_stream_t();

	int size() const { return size_; }

	int write(const void *buff, int size);
	
	bool next(void **data, int *size);
	
	void backup(int size);

	void append(output_chunk_t *chunk);

	void reset();

private:
	output_queue_t buff_;

	int size_;
};

#endif
