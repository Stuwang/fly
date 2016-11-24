#ifndef FLY_CONNECTER_H__
#define FLY_CONNECTER_H__

#include <base/Types.h>
#include <net/TcpConnection.h>

namespace fly {

class Connecter : noncopyable,
	public std::enable_shared_from_this<Connecter> {
public:
	typedef std::function<void(int fd)> Functor;

	explicit Connecter(EventLoop* loop);
	Connecter(EventLoop* loop, const sockaddr& addr);
	void SetAddr(const NetAddr& addr);
	void Reset();
	bool IsConnecting()const {return connecting_;};
	void SetConnectedCallBack(const Functor& f);
	void Start();
	void Stop();

private:
	void HandleWrite();
	void HandleError();

	EventLoop *loop_;
	NetAddr addr_;
	bool seted_addr_;
	bool connecting_;
	int fd_;
	std::unique_ptr<Channel> chan_;
	Functor cb_;
};

}

#endif