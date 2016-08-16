#include <net/Accepter.h>

namespace fly {

Accepter::Accepter(EventLoop* loop, const struct sockaddr_in& addr, bool reuseport)
	: loop_(loop)
	, chan_(socketops::creatNoBlockOrDie(), loop_->getPoller	() )
	, listening_(false)
	, idleFd_(::open("/dev/null", O_RDONLY | O_CLOEXEC))
{
	int fd = chan_.getfd();
	socketops::setReuseAddr(fd, true);
	socketops::setReusePort(fd, reuseport);
	socketops::bindOrDie(fd, socketops::sockaddr_cast(&addr));
	chan_.setReadCallBack(std::bind(&Accepter::handleRead, this));

};

Accepter::~Accepter() {
	chan_.disableAll();
	chan_.remove();
	::close(idleFd_);
}

void Accepter::listen() {
	int fd = chan_.getfd();
	socketops::listenOrDie(fd);
	listening_ = true;
	chan_.enableRead();
	LOG_INFO << "listens start,local address:"
	         << toIpPort(sockaddr_cast(&addr));
};

void Accepter::handleRead() {
	struct sockaddr_in addr;
	int connfd = socketops::accept(chan_.getfd(), &addr);
	if (connfd > 0) {
		if (callback_) {
			callback_(connfd, addr);
		} else {
			socketops::close(connfd);
			LOG_INFO << "New Connection , But no callback , close ,addr is"
			         << toIpPort(sockaddr_cast(&addr));
		}
	} else {
		if (errno == EMFILE) {
			::close(idleFd_);
			idleFd_ = socketops::accept(chan_.getfd(), &addr);
			::close(idleFd_);
			::open("/dev/null", O_RDONLY | O_CLOEXEC);
			LOG_INFO << "New Connection , But no socket descriper can use"
			         << toIpPort(sockaddr_cast(&addr));
		}
	}
};

};
