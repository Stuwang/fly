#include <net/TcpConnection.h>

namespace fly {

TcpConnection::TcpConnection(EventLoop* loop,
                             int fd, const std::string& name)
	: loop_(loop)
	, sockfd_(fd)
	, name_(name)
{
	poller_ = loop->getPoller();
	chan_ = new Channel(sockfd_, poller_);
	localAddr_ = socketops::getLocalAddr(sockfd_);
	peerAddr_ = socketops::getPeerAddr(sockfd_);
	socketops::setNoblockAndCloseOnExec(sockfd_);
	socketops::setNoDelay(sockfd_, true);
	socketops::setKeepAlive(sockfd_, true);
};

TcpConnection::~TcpConnection() {
	delete chan_;
};

EventLoop *TcpConnection::getEventLoop() {
	return loop_;
};

const std::string & TcpConnection::name() const {
	return name_;
};

const struct sockaddr_in& TcpConnection::localAddress() const {
	return localAddr_;
};

const struct sockaddr_in& TcpConnection::peerAddress() const {
	return peerAddr_;
};

bool TcpConnection::connected() const {

	return true;
};

void TcpConnection::Send(const void* data, size_t len) {
	// if in current thread loop.then send
	if (1) {
		SendInLoop(data, len);
	}
};

void TcpConnection::Send(const StringView& data) {

};

void TcpConnection::Send(const Buffer& data) {

};

void TcpConnection::startRead() {
	chan_->enableRead();
};

void TcpConnection::stopRead() {
	chan_->disableRead();
};

bool TcpConnection::isReading()const {
	chan_ -> isReading();
	return true;
};

void TcpConnection::startListenWrite() {
	chan_->enableWrite();
};

void TcpConnection::endListenWrite() {
	chan_->disableWrite();
};

bool TcpConnection::isWriting()const {
	return chan_->isWriting();
};

Buffer* TcpConnection::readBuffer() {
	return &inputBuffer_;
};

Buffer* TcpConnection::writeBuffer() {
	return &outputBuffer_;
};

void TcpConnection::SendInLoop(const void* data, size_t len) {
	if (0 != outputBuffer_.ReadAbleBytes()) {
		outputBuffer_.append(data , len);
		startListenWrite();
		return;
	}

	size_t size = socketops::write(sockfd_, data, len);
	if (size == len) {
		return;
	} else {
		int Error = errno;
		if ( Error == EAGAIN || Error == EWOULDBLOCK ) {
			outputBuffer_.append(static_cast<const char*>(data)+size, len - size);
			startListenWrite();
		}
	}
};

void TcpConnection::SendInLoop(StringView data) {
	SendInLoop(data.data(), data.size());
};

void TcpConnection::handRead() {
	
};

void TcpConnection::handWrite() {
	
};

void TcpConnection::handClose() {

};

void TcpConnection::handError() {

};

}