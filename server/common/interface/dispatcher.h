#ifndef DISPATCHER_T
#define DISPATCHER_T

#include <map>
#include <string>
#include <tr1/functional>
#include "google/protobuf/message.h"

class tcp_connection_t;

class dispatcher_t {
public:
	class cb_t {
	public:
		cb_t() {}

		virtual ~cb_t() {}

 		virtual void on_message(void *p, const google::protobuf::Message *msg) = 0;
	};

	template<typename T>
	class cbT_t : public cb_t {
	public:
		typedef std::tr1::function<void (void *, const T&)> msg_cb_t;

	public:
		cbT_t(const msg_cb_t& cb) { cb_ = cb; }

		virtual void on_message(void *p, const google::protobuf::Message *msg) {
			const T *tmsg = dynamic_cast<const T *> (msg);
			cb_(p, *tmsg);
		}

	public:
		msg_cb_t cb_;
	};

	typedef std::map<const google::protobuf::Descriptor *, cb_t *> msg_map_t;

public:
	dispatcher_t();

	~dispatcher_t();

	template<typename T>
	void register_message(const typename cbT_t<T>::msg_cb_t& cb) { msg_map_[T::descriptor()] = new cbT_t<T>(cb); }

	void on_message(void *p, const google::protobuf::Descriptor* des, const google::protobuf::Message *msg);

	virtual void register_callback() {}

private:
	msg_map_t msg_map_;
};

#endif
