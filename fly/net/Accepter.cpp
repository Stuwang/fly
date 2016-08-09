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
};

void Accepter::handleRead() {
	struct sockaddr_in addr;
	int connfd = socketops::accept(chan_.getfd(), &addr);
	if (connfd > 0) {
		if (callback_) callback_(connfd, addr);
	} else {
		if (errno == EMFILE) {
			::close(idleFd_);
			idleFd_ = socketops::accept(chan_.getfd(), &addr);
			::close(idleFd_);
			::open("/dev/null", O_RDONLY | O_CLOEXEC);
		}
	}
};

};
