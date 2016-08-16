#ifndef FLY_ACCEPTER_H__
#define FLY_ACCEPTER_H__

#include <base/Types.h>
#include <base/Logger.h>

#include <net/Channel.h>
#include <net/SocketOps.h>
#include <net/EventLoop.h>

namespace fly {

typedef std::function<void(int sockfd, const struct sockaddr_in&)> NewConnetionCallBack;

class Accepter : noncopyable {
public:
	Accepter(EventLoop* loop, const struct sockaddr_in&, bool reuseport);
	~Accepter();
	bool isListening() {
		return listening_;
	}
	void listen();

	void setNewConnectionCallBack(const NewConnetionCallBack& callback) {
		callback_ = callback;
	};
private:
	void handleRead();

	struct sockaddr addr;
	EventLoop *loop_;
	Channel chan_;
	NewConnetionCallBack callback_;
	bool listening_;
	int idleFd_;
};

}

#endif
