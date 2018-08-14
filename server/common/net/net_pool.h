#ifndef NET_POOL_H
#define NET_POOL_H

#include <assert.h>
#include "obj_pool.h"
#include "thread.h"

#define NUM_READ_IOVEC 16

/////////////////////////////////////////////////////////////////////////
typedef obj_pool_t<input_chunk_t> input_pool_t;

static tls_t tls_input_pool;

static input_chunk_t *input_chunk_alloc() {
	input_pool_t *pool = (input_pool_t *)tls_input_pool.get();
	if (pool == NULL) {
		pool = new input_pool_t;
		tls_input_pool.set(pool);
	}
	input_chunk_t *chunk = pool->alloc();
	assert(chunk);
	return new (chunk) input_chunk_t;
}

static void input_chunk_free(input_chunk_t *chunk) {
	input_pool_t *pool = (input_pool_t *)tls_input_pool.get();
	assert(pool);
	chunk->~chunk_t();
	pool->free(chunk);
}

/////////////////////////////////////////////////////////////////////////
typedef obj_pool_t<output_chunk_t> output_pool_t;

static tls_t tls_output_pool;

static output_chunk_t *output_chunk_alloc() {
	output_pool_t *pool = (output_pool_t *)tls_output_pool.get();
	if (pool == NULL) {
		pool = new output_pool_t;
		tls_output_pool.set(pool);
	}
	output_chunk_t *chunk = pool->alloc();
	assert(chunk);
	return new (chunk) output_chunk_t;
}

static void output_chunk_free(output_chunk_t *chunk) {
	output_pool_t *pool = (output_pool_t *)tls_output_pool.get();
	assert(pool);
	chunk->~chunk_t();
	pool->free(chunk);
}

/////////////////////////////////////////////////////////////////////////
static tls_t tls_read_fd_buff;

static input_queue_t& assign_read_fd_buff() {
	input_queue_t *read_fd_buff = (input_queue_t *)tls_read_fd_buff.get();
	if (read_fd_buff == NULL) {
		read_fd_buff = new input_queue_t;
		tls_read_fd_buff.set(read_fd_buff);
	}
	while (read_fd_buff->size() < NUM_READ_IOVEC) {
		input_chunk_t *chunk = input_chunk_alloc();
		read_fd_buff->push_back(chunk);
	}
	return *read_fd_buff;
}

/////////////////////////////////////////////////////////////////////////
typedef obj_pool_t<tcp_connection_t> connection_pool_t;

static tls_t tls_connection_pool;

static tcp_connection_t *connection_alloc() {
	connection_pool_t *pool = (connection_pool_t *)tls_connection_pool.get();
	if (pool == NULL) {
		pool = new connection_pool_t;
		tls_connection_pool.set(pool);
	}
	tcp_connection_t *conn = pool->alloc();
	assert(conn);
	return new (conn) tcp_connection_t;
}

static void connection_free(tcp_connection_t *conn) {
	connection_pool_t *pool = (connection_pool_t *)tls_connection_pool.get();
	assert(pool);
	conn->~tcp_connection_t();
	pool->free(conn);
}

#endif
