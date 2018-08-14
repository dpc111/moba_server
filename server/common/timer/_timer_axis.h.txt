#ifndef TIMER_AXIS_H
#define TIMER_AXIS_H

#define TIMER_GRID 64
#define TIMER_AXIS_LENGTH 720000

class timer_axis_t {
public:
	struct callback_t;

	typedef std::list<callback_t *> callback_list_t;

	typedef std::vector<callback_list_t> axis_vector_t;

	typedef std::hash_map<uint32, callback_list_t> axis_map_t;

	struct function_t {
		virtual void *obj() = 0;

		virtual void *callback() = 0;

		virtual bool empty() const = 0;

		virtual bool call(timer_axis_t *axis, uint32 timer_id) = 0; 
	};

	template<typename T>
	struct function_tmp_t : public function_t {
		typedef bool (T::*timer_callback_t) (timer_axis_t *axis, uint32 timer_id);

		function_tmp_t(T *obj, const timer_callback_t& callback) : obj_(obj), callback_(callback) {}

		virtual void *obj() { return obj_; }

		virtual void *callback() { return *(void**)&callback_; }

		virtual bool empty () const { return obj_ == NULL || callback_ == NULL; }

		virtual bool call(timer_axis_t *axis, uint32 timer_id) { return (obj_->*callback_)(axis, timer_id); }

		T *obj_;

		timer_callback_t callback_;
	};

	struct callback_t {
		template<typename T>
		callback_t(uint32 timer_id, T* obj, const function_tmp_t<T>::timer_callback_t& callback, uint32 call_times, uint32 timeout, uint32 interval) :
			function_(new function_tmp_t<T>(obj, callback)),
			timer_id_(timer_id),
			call_times_(call_times),
			timeout_(timeout),
			interval_(interval) {}

		virtual ~callback_t() {
			delete function_;
		}

		bool setup(timer_axis_t *axis) {
			if (interval == 0) {
				interval = 1;
			}
			last_call_time_ = axis->get_last_check_time();
			grid_index_ = ((last_call_time_ + interval - axis->get_init_time()) / TIMER_GRID) % axis->get_axis_size();
			return true;
		}

		bool activate(uint32 now) {
			if (now - last_call_time_ >= interval_) {
				last_call_time_ = now;
				return true;
			}
			return false;
		}

		bool exchanged(timer_axis_t *axis) {
			uint32 index = ((last_call_time_ + interval - axis->get_init_time()) / TIMER_GRID) % axis->get_axis_size();
			if (index == grid_index_) {
				return false;
			}
			grid_index_ = index;
			return true;
		}

		void *obj() { return function_->obj(); }

		void *callback() { return function_->callback(); }

		bool empty() { return function_->empty(); }

		bool call(timer_axis_t *axis) { return function_->call(axis, timer_id_); }

		function_t *function_;

		uint32 timer_id_;

		uint32 call_times;

		uint32 timeout_;

		uint32 interval_;

		timestamp last_call_time_;

		uint32 grid_index_;

		callback_list_t::iterator it_;
	};

public:
	timer_axis_t();

	~timer_axis_t();

	uint32 get_axis_size() const { return axis_.size(); }

	timestamp get_init_time() const { return init_time_; }

	timestamp get_last_check_time() const { return last_check_time_; }

	callback_list_t& get_callback_list(void *ptr);

	void clear_callback_list(void *ptr);

	bool register(callback_t *callback);

	bool unregister(uint32 timer_id, void *obj, void *callback);

	void process(uint32 start_grid, uin32 current_grid, uint32 now);

public:
	bool register()

private:
	axis_vector_t axis_;

	axis_map_t axis_map_;

	timestamp init_time_;

	timestamp last_check_time_;
};

#endif