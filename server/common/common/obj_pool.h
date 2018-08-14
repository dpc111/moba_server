// #ifndef	OBJ_POOL_H 
// #define OBJ_POOL_H

// #include <stdlib.h>
// #include <queue>

// #define OBJ_POOL_MAX_SIZE 1024

// template <typename obj_t>
// class obj_pool_t
// {
// public:
// 	obj_pool_t() {
// 		size_ = 0;
// 	}

// 	~obj_pool_t() {
// 		while (pool_.empty()) {
// 			obj_t* obj = pool_.front();
// 			this->free(obj);
// 			pool_.pop();
// 			size_ -= 1;
// 		}
// 	}

// 	obj_t* alloc() {
// 		if (pool_.empty()) {
// 			alloc_obj();
// 		}
// 		obj_t* obj = pool_.front();
// 		pool_.pop();
// 		return obj;
// 	}

// 	void free(obj_t *obj) {
// 		if (!obj) {
// 			return;
// 		}
// 		pool_.push(obj);
// 		size_ += 1;
// 		while (size_ > OBJ_POOL_MAX_SIZE) {
// 			obj_t* obj = pool_.front();
// 			::free(obj);
// 			// delete obj;
// 			pool_.pop();
// 			size_ -= 1;
// 		}
// 	}

// private:
// 	void alloc_obj() {
// 		obj_t* obj = (obj_t*)malloc(sizeof(obj_t)); 
// 		// obj_t* obj = new obj_t; 
// 		pool_.push(obj);
// 		size_ += 1;
// 	} 

// private:
// 	std::queue<obj_t *> pool_;
// 	int size_;
// };

// #endif


#ifndef	OBJ_POOL_H 
#define OBJ_POOL_H

#include <stdlib.h>
#include <list>

#define OBJ_POOL_MAX_SIZE 1024

template <typename obj_t>
class obj_pool_t
{
public:
	obj_pool_t() {
		size_ = 0;
	}

	~obj_pool_t() {
		while (pool_.empty()) {
			obj_t* obj = pool_.front();
			assert(obj);
			::free(obj);
			pool_.pop_front();
			size_ -= 1;
		}
	}

	obj_t* alloc() {
		if (pool_.empty()) {
			alloc_obj();
		}
		obj_t* obj = pool_.front();
		pool_.pop_front();
		size_ -= 1;
		return obj;
	}

	void free(obj_t *obj) {
		if (!obj) {
			return;
		}
		pool_.push_back(obj);
		size_ += 1;
		while (size_ > OBJ_POOL_MAX_SIZE) {
			obj_t* obj = pool_.front();
			assert(obj);
			::free(obj);
			// delete obj;
			pool_.pop_front();
			size_ -= 1;
		}
	}

private:
	void alloc_obj() {
		obj_t* obj = (obj_t*)malloc(sizeof(obj_t)); 
		// obj_t* obj = new obj_t; 
		pool_.push_back(obj);
		size_ += 1;
	} 

private:
	std::list<obj_t *> pool_;
	int size_;
};

#endif