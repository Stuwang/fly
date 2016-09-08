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

int creatTimerFd(){
	int timerfd = ::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK | TFD_CLOEXEC);
	if(timerfd < 0){
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

int connect(int socketfd, const sockaddr* addr) {
	return ::connect(socketfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in)));
};

void bindOrDie(int socketfd, const sockaddr* addr) {
	int ret = ::bind(socketfd, addr, sizeof(struct sockaddr_in));
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
		struct sockaddr_in addr = getLocalAddr(socketfd);
		LOG_FATAL << "listen error , addr is "
		          << toIpPort(sockaddr_cast(&addr) )
		          << " errno:" << err
		          << " errmsg:" << strerror(err);
	}
};

int accept(int socketfd, struct sockaddr_in *addr) {
	socklen_t len = static_cast<socklen_t>(sizeof(struct sockaddr_in));
	int connfd = accept(socketfd, sockaddr_cast(addr), &len);
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

struct sockaddr_in getLocalAddr(int socketfd)  {
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	socklen_t len = static_cast<socklen_t>(sizeof(struct sockaddr));
	if (::getsockname(socketfd, sockaddr_cast(&addr), &len) < 0) {
		int err = getSocketError(errno);
		LOG_FATAL << "getLocalAddr error , addr is " << toIpPort(sockaddr_cast(&addr))
		          << " errno:" << err
		          << " errmsg:" << strerror(err);
	}
	return addr;
};
struct sockaddr_in getPeerAddr(int socketfd) {
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	socklen_t len = static_cast<socklen_t>(sizeof(struct sockaddr));
	if (::getpeername(socketfd, sockaddr_cast(&addr), &len) < 0) {
		int err = getSocketError(errno);
		LOG_FATAL << "getPeerAddr error , addr is " << toIpPort(sockaddr_cast(&addr))
		          << " errno:" << err
		          << " errmsg:" << strerror(err);
	}
	return addr;
};

void toIpPort(char* buf, size_t size, const sockaddr *addr) {
	toIp(buf, size, addr);
	size_t end = strlen(buf);
	uint16_t port = ntohs(sockaddr_in_cast(addr)->sin_port);
	snprintf(buf + end, size - end, ":%u", port);
};

std::string toIpPort(const sockaddr *addr) {
	char buf[64];
	const int size = 64;
	toIp(buf, size, addr);
	size_t end = strlen(buf);
	uint16_t port = ntohs(sockaddr_in_cast(addr)->sin_port);
	snprintf(buf + end, size - end, ":%u", port);
	return std::string(buf);
};

void toIp(char *buf, size_t size, const struct sockaddr*addr) {
	if (addr->sa_family == AF_INET) {
		::inet_ntop(AF_INET, &sockaddr_in_cast(addr)->sin_addr, buf, static_cast<socklen_t>(size));
	}
};

void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in * addr) {
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0 ) {
		LOG_FATAL << "error ip and port " << ip << " " << port ;
	}
};

struct sockaddr_in fromPort(uint16_t port){
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	return addr;	
};

struct sockaddr_in fromIpPort(const char* ip, uint16_t port) {
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if (::inet_pton(AF_INET, ip, &addr.sin_addr) <= 0 ) {
		LOG_FATAL << "error ip and port " << ip << " " << port ;
	}
	return addr;
};

const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr) {
	return static_cast<const struct sockaddr*>(static_cast<const void*>(addr));
};

struct sockaddr* sockaddr_cast(struct sockaddr_in* addr) {
	return static_cast<struct sockaddr*>(static_cast<void*>(addr));
};

const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr) {
	return static_cast<const struct sockaddr_in*>(static_cast<const void*>(addr));
};


}

};
