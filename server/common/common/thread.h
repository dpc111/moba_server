#ifndef THREAD_H
#define THREAD_H

#include <unistd.h>
#include <pthread.h>

class lock_t
{
public:
	lock_t();

	~lock_t();

	void lock(const char *filename = NULL, int line = 0);

	void unlock();

private:
	pthread_mutex_t	lock_;
	const char *file_name_;
	int line_;
};

class guard_t 
{
public:
	guard_t(lock_t *lock, const char *filename = NULL, int line = 0);

	~guard_t();

private:
	lock_t *lock_;
};

class tls_t 
{
public:
	tls_t();

	~tls_t();

	void *get();

	void set(void *ptr);

private:
	pthread_key_t tls_;
};

#define GUARD(lock) guard_t guard(lock, __FILE__, __LINE__)

class thread_t
{
public:
	typedef int (*thread_proc) (void *);

	thread_t();
	
	~thread_t();

	bool create(thread_proc proc, void *param);

	void exit(int exitcode);

	int terminate(int exitcode);

	int join();

	void detach();

	static int runner(void *ptr);

	pthread_t thread_;
	thread_proc proc_;
	void *param_;
};
#endif
