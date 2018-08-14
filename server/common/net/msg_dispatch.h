#ifndef MESSAGE_DISPATCH_T
#define MESSAGE_DISPATCH_T

#include <map>
#include <string>
#include <tr1/functional>
#include "google/protobuf/message.h"

class tcp_connection_t;
class tcp_network_t;

class msg_dispatch_t {
public:
	class cb_t {
	public:
		cb_t() {}

		virtual ~cb_t() {}

 		virtual void on_message(int sid, const google::protobuf::Message *msg) = 0;
	};

	template<typename T>
	class cbT_t : public cb_t {
	public:
		typedef std::tr1::function<void (int, const T&)> msg_cb_t;

	public:
		cbT_t(const msg_cb_t& cb) { cb_ = cb; }

		virtual void on_message(int sid, const google::protobuf::Message *msg) {
			const T *tmsg = dynamic_cast<const T *> (msg);
			cb_(sid, *tmsg);
		}

	public:
		msg_cb_t cb_;
	};

	class cb_net_t {
	public:
		cb_net_t() {}

		virtual ~cb_net_t() {}

		virtual void on_message(tcp_connection_t *conn, const google::protobuf::Message *msg) = 0;
	};

	template<typename T>
	class cb_netT_t : public cb_net_t {
	public:
		typedef std::tr1::function<void (tcp_connection_t *, const T&)> msg_cb_t;

	public:
		cb_netT_t(const msg_cb_t& cb) { cb_ = cb; }

		virtual void on_message(tcp_connection_t *conn, const google::protobuf::Message *msg) {
			const T *tmsg = dynamic_cast<const T *> (msg);
			cb_(conn, *tmsg);
		}

	public:
		msg_cb_t cb_;
	};

	typedef std::map<const google::protobuf::Descriptor *, cb_t *> msg_map_t;

	typedef std::map<const google::protobuf::Descriptor *, cb_net_t *> msg_net_map_t;

	typedef std::tr1::function<void (int, const google::protobuf::Descriptor *, const google::protobuf::Message *)> msg_default_cb_t;

	typedef std::tr1::function<bool (tcp_connection_t *, const char *)> on_script_func_t;

public:
	msg_dispatch_t(tcp_network_t *network);

	~msg_dispatch_t();

	template<typename T>
	void register_message(const typename cbT_t<T>::msg_cb_t& cb) { msg_map_[T::descriptor()] = new cbT_t<T>(cb); }

	template<typename T>
	void register_net_message(const typename cb_netT_t<T>::msg_cb_t& cb) { msg_net_map_[T::descriptor()] = new cb_netT_t<T>(cb); }

	void register_default_message(const msg_default_cb_t& func) { msg_default_cb_ = func; }

	void set_on_script_func(const on_script_func_t& func) { on_script_func_ = func; }

	void on_message(tcp_connection_t *conn, google::protobuf::Message *msg);

	bool on_script_func(tcp_connection_t *conn, const char *name);

private:
	tcp_network_t *network_;

	msg_map_t msg_map_;

	msg_net_map_t msg_net_map_;

	msg_default_cb_t msg_default_cb_;

	on_script_func_t on_script_func_;
};

#endif
