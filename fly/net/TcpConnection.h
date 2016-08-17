#ifndef FLY_TCPCONNECTION_H__
#define FLY_TCPCONNECTION_H__

#include  <functional>
#include  <memory>

#include <base/Buffer.h>
#include <base/Types.h>
#include <base/StringView.h>
#include <net/EventLoop.h>


namespace fly {

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConPtr;

typedef std::function<void(const TcpConPtr&)> CallBack;

class TcpConnection :
	noncopyable ,
	public std::enable_shared_from_this<TcpConnection> {
public:
	TcpConnection(EventLoop* loop, int fd, const std::string& name = "");
	~TcpConnection();

	void Start();

	EventLoop *getEventLoop();
	const std::string & name() const;
	const struct sockaddr_in& localAddress() const ;
	const struct sockaddr_in& peerAddress() const ;

	void newCallBack(const CallBack& cb) {newCb_ = cb;};
	void ReadCallBack(const CallBack& cb) {readCb_ = cb;};
	void writeCallBack(const CallBack& cb) {writeCb_ = cb;};
	void closeCallBack(const CallBack& cb) {closeCb_ = cb;};
	void highCallBack(const CallBack& cb, size_t size = DefaultHighWater_) {
		highCb_ = cb;
		HighWater_ = size;
	};

	bool connected() const;

	void Send(const void* data, size_t len);
	void Send(const StringView& data);
	void Send(Buffer& data);
	void startRead();
	void stopRead();
	void SetReadInLoop(bool on);
	bool isReading()const;
	void startListenWrite();
	void endListenWrite();
	void SetWriteInLoop(bool on);
	bool isWriting()const;

	void shutdown();
	void forceClose();

	Buffer* readBuffer();
	Buffer* writeBuffer();

protected:
	void SendInLoop(const void* data, size_t len);
	void SendInLoop(StringView data);
	void SendInLoop_helper(StringView data);
	void shutdownInLoop();
	void forceCloseInLoop();

	void handRead();
	void handWrite();
	void handClose();
	void handError();

private:
	EventLoop *loop_;
	Poller *poller_;
	int sockfd_;
	std::unique_ptr<Channel> chan_;
	std::string name_;

	Buffer inputBuffer_;
	Buffer outputBuffer_;
	struct sockaddr_in localAddr_;
	struct sockaddr_in peerAddr_;

	static const size_t DefaultHighWater_ = 64 * 1024;
	size_t HighWater_;
	CallBack newCb_;
	CallBack readCb_;
	CallBack writeCb_;
	CallBack closeCb_;
	CallBack highCb_;

};



}

#endif
