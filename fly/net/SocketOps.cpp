#include <net/SocketOps.h>


namespace fly {

namespace socketops {

int creatEventFd() {
	int eventfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (eventfd < 0) {
		int err = socketops::getSocketError(errno);
		LOG_FATAL << "errno:"
		          << err
		          << " errmsg:" << strerror(err);
	}
	return eventfd;
};

int creatTimerFd() {
	int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	if (timerfd < 0) {
		int err = socketops::getSocketError(errno);
		LOG_FATAL << "get new timer fd error "
		          << err
		          << " rrmsg " << strerror(err);
	}
	return timerfd;
};

void setNoblockAndCloseOnExec(int socket) {
	int flag = ::fcntl(socket, F_GETFL, 0);
	flag |= O_NONBLOCK;
	int ret = ::fcntl(socket, F_SETFL, flag);

	flag = ::fcntl(socket, F_GETFD, 0);
	flag |= FD_CLOEXEC;
	ret = ::fcntl(socket, F_SETFD, flag);

	(void)ret;
};

void setNoDelay(int socketfd, bool on) {
	int flag = on ? 1 : 0;
	::setsockopt(socketfd, IPPROTO_TCP, TCP_NODELAY, &flag, static_cast<socklen_t>(sizeof(int)));
};

void setReuseAddr(int socketfd, bool on) {
	int flag = on ? 1 : 0;
	::setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &flag, static_cast<socklen_t>(sizeof(int)));
};

void setReusePort(int socketfd, bool on) {
	int flag = on ? 1 : 0;
	::setsockopt(socketfd, SOL_SOCKET, SO_REUSEPORT, &flag, static_cast<socklen_t>(sizeof(int)));
};

void setKeepAlive(int socketfd, bool on) {
	int flag = on ? 1 : 0;
	::setsockopt(socketfd, SOL_SOCKET, SO_KEEPALIVE, &flag, static_cast<socklen_t>(sizeof(int)));
};

// void setBufSize(int socketfd, size_t bufsize);

int creatNoBlockOrDie() {
	int socketfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
	if (socketfd < 0) {
		int err = getSocketError(errno);
		LOG_FATAL << "create socket error,die errno:"
		          << err
		          << " errmsg:" << strerror(err);
	}
	return socketfd;
};

int connect(int socketfd, const NetAddr& addr) {
	return ::connect(socketfd, addr.getSockAddr(), static_cast<socklen_t>(sizeof(struct sockaddr_in)));
};

void bindOrDie(int socketfd, const NetAddr& addr) {
	int ret = ::bind(socketfd, addr.getSockAddr(), sizeof(struct sockaddr_in));
	if (ret < 0 ) {
		int err = getSocketError(errno);
		LOG_FATAL << "bind error , addr is " << toIpPort(addr)
		          << " errno:" << err
		          << " errmsg:" << strerror(err);
	}
};


void listenOrDie(int socketfd) {
	int ret = ::listen(socketfd, SOMAXCONN);
	if (ret < 0) {
		int err = getSocketError(errno);
		NetAddr addr = getLocalAddr(socketfd);
		LOG_FATAL << "listen error , addr is "
		          << addr.IpPort()
		          << " errno:" << err
		          << " errmsg:" << strerror(err);
	}
};

int accept(int socketfd, NetAddr* addr) {
	socklen_t len = static_cast<socklen_t>(sizeof(struct sockaddr_in));
	int connfd = accept(socketfd, addr->getSockAddr(), &len);
	setNoblockAndCloseOnExec(connfd);

	if (connfd < 0) {
		int saveErrno = errno;
		switch (saveErrno) {
		case EAGAIN:
		case ECONNABORTED:
		case EINTR:
		case EPROTO:
		case EPERM:
		case EMFILE:
			errno = saveErrno;
			break;
		case EBADF:
		case EFAULT:
		case EINVAL:
		case ENFILE:
		case ENOBUFS:
		case ENOMEM:
		case ENOTSOCK:
		case EOPNOTSUPP:
			break;
		default:
			break;
		}
	}

	return connfd;
};

ssize_t read(int socketfd, void *buf, size_t cout) {
	return ::read(socketfd, buf, cout);
};

ssize_t readv(int socketfd, const struct iovec *iov, int iovcnt) {
	return ::readv(socketfd, iov, iovcnt);
};

ssize_t write(int socketfd, const void *buf, size_t cout) {
	return ::write(socketfd, buf, cout);
};

void close(int socketfd) {
	if (::close(socketfd) < 0) {
		int err = getSocketError(errno);
		LOG_FATAL << "close error "
		          << " errno:" << err
		          << " errmsg:" << strerror(err);
	}
};

void shutdownWrite(int socketfd) {
	if (::shutdown(socketfd, SHUT_WR) < 0) {
		int err = getSocketError(errno);
		LOG_FATAL << "shutdownWrite error"
		          << " errno:" << err
		          << " errmsg:" << strerror(err);

	}
};

int getSocketError(int socketfd) {
	int flag = 0;
	socklen_t len = static_cast<socklen_t>(sizeof(struct sockaddr_in));
	if (::getsockopt(socketfd, SOL_SOCKET, SO_ERROR, &flag, &len) < 0) {
		return errno;
	} else {
		return flag;
	}
};

NetAddr getLocalAddr(int socketfd)  {
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	socklen_t len = static_cast<socklen_t>(sizeof(struct sockaddr));
	if (::getsockname(socketfd, sockaddr_cast(&addr), &len) < 0) {
		int err = getSocketError(errno);
		LOG_FATAL << "getLocalAddr error , addr is " << toIpPort(sockaddr_cast(&addr))
		          << " errno:" << err
		          << " errmsg:" << strerror(err);
	}
	return NetAddr(addr);
};

NetAddr getPeerAddr(int socketfd) {
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	socklen_t len = static_cast<socklen_t>(sizeof(struct sockaddr));
	if (::getpeername(socketfd, sockaddr_cast(&addr), &len) < 0) {
		int err = getSocketError(errno);
		LOG_FATAL << "getPeerAddr error , addr is " << toIpPort(sockaddr_cast(&addr))
		          << " errno:" << err
		          << " errmsg:" << strerror(err);
	}
	return NetAddr(addr);
};

}

};
