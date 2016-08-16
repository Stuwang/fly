#include <net/TcpConnection.h>

namespace fly {

TcpConnection::TcpConnection(EventLoop* loop,
                             int fd, const std::string& name)
	: loop_(loop)
	, sockfd_(fd)
	, name_(name)
{
	poller_ = loop->getPoller();
	chan_.reset(new Channel(sockfd_, poller_) );
	localAddr_ = socketops::getLocalAddr(sockfd_);
	peerAddr_ = socketops::getPeerAddr(sockfd_);

	chan_->setReadCallBack(std::bind(&TcpConnection::handRead, this));
	chan_->setWriteCallBack(std::bind(&TcpConnection::handWrite, this));
	chan_->setCloseCallBack(std::bind(&TcpConnection::handClose, this));
	chan_->setErrorCallBack(std::bind(&TcpConnection::handError, this));

	socketops::setNoblockAndCloseOnExec(sockfd_);
	socketops::setNoDelay(sockfd_, true);
	socketops::setKeepAlive(sockfd_, true);

	startRead();

	if (newCb_) newCb_(shared_from_this());
};

TcpConnection::~TcpConnection() {

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
	Send(StringView(data, len));
};

void TcpConnection::Send(const StringView& data) {
	// if in current thread loop.then send
	if (loop_->IsInLoop()) {
		SendInLoop(data.data(), data.size());
	} else {
		loop_->queueInLoop(
		    std::bind(&TcpConnection::SendInLoop_helper,
		              shared_from_this()
		              , data.toString()));
	}
};

void TcpConnection::Send(Buffer& data) {
	Send(StringView(data.data(), data.ReadAbleBytes()));
};

void TcpConnection::startRead() {
	loop_->runInLoop(std::bind(&TcpConnection::SetReadInLoop, this, true) );
};

void TcpConnection::stopRead() {
	loop_->runInLoop(std::bind(&TcpConnection::SetReadInLoop, this, false));
};

bool TcpConnection::isReading()const {
	chan_ -> isReading();
	return true;
};

void  TcpConnection::SetReadInLoop(bool on) {
	if (on) {
		chan_->enableRead();
	} else {
		chan_ ->disableRead();
	}
};

void TcpConnection::startListenWrite() {
	loop_->runInLoop(std::bind(&TcpConnection::SetWriteInLoop, this, true));
};

void TcpConnection::endListenWrite() {
	loop_->runInLoop(std::bind(&TcpConnection::SetWriteInLoop, this, false));
};

bool TcpConnection::isWriting()const {
	return chan_->isWriting();
};

void TcpConnection::SetWriteInLoop(bool on) {
	if (on) {
		chan_->enableWrite();
	} else {
		chan_ ->disableWrite();
	}
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
		if (len - size >= HighWater_ ) {
			if (highCb_) {
				highCb_(shared_from_this());
			} else {

			}
		}
		int Error = errno;
		if ( Error == EAGAIN || Error == EWOULDBLOCK) {
			outputBuffer_.append(static_cast<const char*>(data) + size, len - size);
			startListenWrite();
		}
	}
};

void TcpConnection::SendInLoop(StringView data) {
	SendInLoop(data.data(), data.size());
};

void TcpConnection::SendInLoop_helper(StringView data) {
	SendInLoop(data.data(), data.size());
};

void TcpConnection::shutdown() {
	loop_->runInLoop(std::bind(
	                     &TcpConnection::shutdownInLoop,
	                     shared_from_this()));
};

void TcpConnection::shutdownInLoop() {
	assert(loop_->IsInLoop());
	socketops::shutdownWrite(chan_->getfd());
};

void TcpConnection::forceClose()
{

	loop_->queueInLoop(std::bind(&TcpConnection::forceCloseInLoop,
	                             shared_from_this()));

}


void TcpConnection::forceCloseInLoop()
{
	assert(loop_->IsInLoop());
	handClose();
}

void TcpConnection::handRead() {
	static const int extraBufSize = 6 * 1024;
	char buf[extraBufSize];
	struct iovec vec[2];
	const size_t writable = inputBuffer_.WriteAbleBytes();
	vec[0].iov_base = inputBuffer_.Peek();
	vec[0].iov_len = writable;
	vec[1].iov_base = buf;
	vec[1].iov_len = extraBufSize;
	const ssize_t n = socketops::readv(chan_->getfd(), vec, 2);
	if (n < 0) {
		if (readCb_) readCb_(shared_from_this());
	} else if (n <= writable) {
		inputBuffer_.retireWriteAble(n);
	} else {
		inputBuffer_.retireWriteAble(writable);
		inputBuffer_.append(buf, n - writable);
	}
};

void TcpConnection::handWrite() {
	size_t len = outputBuffer_.ReadAbleBytes();
	size_t size = socketops::write(chan_->getfd(), outputBuffer_.data(), len);
	outputBuffer_.retireRead(size);
	if (0 == outputBuffer_.ReadAbleBytes()) {
		chan_->disableWrite();
		if (writeCb_) writeCb_(shared_from_this());
	}
};

void TcpConnection::handClose() {
	assert(loop_->IsInLoop());
	if (closeCb_) closeCb_(shared_from_this());
};

void TcpConnection::handError() {
	int err = socketops::getSocketError(chan_->getfd());
	(void)err;
};

}
