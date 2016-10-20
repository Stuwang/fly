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

};

void TcpConnection::Start() {
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

const NetAddr  TcpConnection::localAddress() const {
	return localAddr_;
};

const NetAddr  TcpConnection::peerAddress() const {
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

	loop_->WeakUp();
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
	if (loop_->IsInLoop()) {
		SetReadInLoop(true);
	} else {
		loop_->queueInLoop(std::bind(&TcpConnection::SetReadInLoop, this, true));
	}
};

void TcpConnection::stopRead() {
	if (loop_->IsInLoop()) {
		SetReadInLoop(false);
	} else {
		loop_->queueInLoop(std::bind(&TcpConnection::SetReadInLoop, this, false));
	}
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
	if (loop_->IsInLoop()) {
		SetWriteInLoop(true);
	} else {
		loop_->queueInLoop(std::bind(&TcpConnection::SetWriteInLoop, this, true));
	}
};

void TcpConnection::endListenWrite() {
	if (loop_->IsInLoop()) {
		SetWriteInLoop(false);
	} else {
		loop_->queueInLoop(std::bind(&TcpConnection::SetWriteInLoop, this, false));
	}
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
	loop_->queueInLoop(std::bind(
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

	static const int extraBufSize = 64 * 1024;
	char buf[extraBufSize];
	const int n = socketops::read(chan_->getfd(), buf, extraBufSize);
	if (n < 0) {
		LOG_DEBUG << "read " << n << " error " << errno << " msg " << strerror(errno)  ;
		switch (errno) {
		case EAGAIN:
		case EINTR:
			break;
		case EBADF:
		case EFAULT:
		case EINVAL:
		case EIO:
		case EISDIR:
			handClose();
			break;
		default:
			break;
		}
	} else if (n == 0) {
		LOG_DEBUG << "read return 0 and close socket is" << chan_->getfd();
		handClose();
	} else {
		inputBuffer_.append(buf, n);
		if (readCb_) {
			readCb_(shared_from_this());
		}
	}


};

void TcpConnection::handWrite() {
	size_t len = outputBuffer_.ReadAbleBytes();
	const int size = socketops::write(chan_->getfd(), outputBuffer_.data(), len);
	if (size > 0 ) {
		outputBuffer_.retireRead(size);
		if (0 == outputBuffer_.ReadAbleBytes()) {
			chan_->disableWrite();
			if (writeCb_) {
				writeCb_(shared_from_this());
			}
		}
	} else if (size == 0) {
		handClose();
	} else if (size == -1) {
		switch (errno) {
		case EAGAIN:
		case EINTR:
		case ENOSPC:
			break;
		case EBADF:
		case EDESTADDRREQ:
		case EDQUOT:
		case EFAULT:
		case EFBIG:
		case EINVAL:
		case EIO:
		case EPIPE:
			handClose();
			break;
		default:
			break;

		}
	}
};

void TcpConnection::handClose() {
	assert(loop_->IsInLoop());
	if (closeCb_) {
		LOG_DEBUG << "close socket";
		closeCb_(shared_from_this());
	}
};

void TcpConnection::handError() {
	int err = socketops::getSocketError(chan_->getfd());
	(void)err;
};

}
