#ifndef FLY_ACCEPTER_H__
#define FLY_ACCEPTER_H__

#include <base/Types.h>
#include <base/Logger.h>
#include <base/Types.h>

#include <net/Channel.h>
#include <net/SocketOps.h>
#include <net/EventLoop.h>

namespace fly {

typedef std::function<void(int sockfd, const NetAddr&)>
NewConnetionCallBack;

class Accepter : noncopyable {
public:
	Accepter(EventLoop* loop,
	         const NetAddr&, bool reuseport);

	~Accepter();

	bool isListening() const {
		return listening_;
	}

	void listen();

	void setNewConnectionCallBack(const NewConnetionCallBack& callback) {
		callback_ = callback;
	};
	
private:
	void handleRead();

	EventLoop *loop_;
	NetAddr addr_;
	Channel chan_;
	NewConnetionCallBack callback_;
	bool listening_;
	int idleFd_;
};

}

#endif
