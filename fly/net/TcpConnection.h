#ifndef FLY_TCPCONNECTION_H__
#define FLY_TCPCONNECTION_H__

#include  <functional>

#include <base/Buffer.h>
#include <base/Types.h>
#include <base/StringView.h>
#include <net/EventLoop.h>


namespace fly {

class TcpConnection : noncopyable {
public:
	TcpConnection(EventLoop* loop, int fd, const std::string& name = "");
	~TcpConnection();
	EventLoop *getEventLoop();
	const std::string & name() const;
	const struct sockaddr_in& localAddress() const ;
	const struct sockaddr_in& peerAddress() const ;

	bool connected() const;

	void Send(const void* data, size_t len);
	void Send(const StringView& data);
	void Send(Buffer& data);
	void startRead();
	void stopRead();
	bool isReading()const;
	void startListenWrite();
	void endListenWrite();
	bool isWriting()const;

	Buffer* readBuffer();
	Buffer* writeBuffer();
protected:
	void SendInLoop(const void* data, size_t len);
	void SendInLoop(StringView data);
	void SendInLoop_helper(StringView data);
	
	void handRead();
	void handWrite();
	void handClose();
	void handError();

private:
	EventLoop *loop_;
	Poller *poller_;
	int sockfd_;
	Channel *chan_;
	std::string name_;

	Buffer inputBuffer_;
	Buffer outputBuffer_;
	struct sockaddr_in localAddr_;
	struct sockaddr_in peerAddr_;
};

}

#endif
