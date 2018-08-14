#include <cassert>
#include "thread.h"

//////////////////////////////////////////////////////////////////////////
lock_t::lock_t() {
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutex_init(&lock_, &attr);
	file_name_ = NULL;
	line_ = 0;
}

lock_t::~lock_t() {
	pthread_mutex_destroy(&lock_);
}

void lock_t::lock(const char *filename, int line) {
	pthread_mutex_lock(&lock_);
	file_name_ = filename;
	line_ = line;
}

void lock_t::unlock() {
	pthread_mutex_unlock(&lock_);
}

//////////////////////////////////////////////////////////////////////////
guard_t::guard_t(lock_t *lock, const char *filename, int line) {
	lock_ = lock;
	lock_->lock(filename, line);
}

guard_t::~guard_t() {
	lock_->unlock();
}

//////////////////////////////////////////////////////////////////////////
tls_t::tls_t() {
	int r = pthread_key_create(&tls_, NULL);
	if (r) {
		(false);
	}
}

tls_t::~tls_t() {
	int r = pthread_key_delete(tls_);
	if (r) {
		assert(false);
	}
}

void *tls_t::get() {
	return pthread_getspecific(tls_);
}

void tls_t::set(void *ptr) {
	int r = pthread_setspecific(tls_, ptr);
	if (r) {
		assert(false);
	}
}

//////////////////////////////////////////////////////////////////////////
thread_t::thread_t() {

}

thread_t::~thread_t() {

}

bool thread_t::create(thread_proc proc, void *param) {
	if (!proc) {
		return false;
	}
	proc_ = proc;
	param_ = param;
	int code = pthread_create(&thread_, NULL, reinterpret_cast<void*(*)(void *)>(&runner), this);
	if (code != 0) {
		return false;
	}
	return true;
}

int thread_t::runner(void *ptr) {
	thread_t *thread = (thread_t *)ptr;
	return thread->proc_(thread->param_);
}

void thread_t::exit(int exitcode) {
	pthread_exit(&exitcode);
}

int thread_t::terminate(int exitcode) {
	int retcode = pthread_cancel(thread_);
	return !retcode;
}

int thread_t::join() {
	int retcode = pthread_join(thread_, NULL);
	return !retcode;
}

void thread_t::detach() {
	pthread_detach(thread_);
}