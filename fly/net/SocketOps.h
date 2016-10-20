#ifndef _FLY_SOCKETOPS_H__
#define _FLY_SOCKETOPS_H__

#include <cstdlib>

#include <string>
#include <arpa/inet.h>
#include <string.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>  	// snprintf
#include <strings.h>  	// bzero
#include <sys/socket.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>

#include <base/Logger.h>

#include <net/NetAddr.h>

namespace fly {

class NetAddr;

namespace socketops {

int creatEventFd();
int creatTimerFd();

void setNoblockAndCloseOnExec(int socketfd);
void setNoDelay(int socketfd, bool on);
void setReuseAddr(int socketfd, bool on);
void setReusePort(int socketfd, bool on);
void setKeepAlive(int socketfd, bool on);
// void setBufSize(int socketfd, size_t bufsize);
int creatNoBlockOrDie();


int connect(int socketfd, const NetAddr& addr);
void bindOrDie(int socketfd, const NetAddr& addr);
void listenOrDie(int socketfd);
int accept(int socketfd, NetAddr* addr);

ssize_t read(int sockfd, void *buf, size_t cout);
ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);
ssize_t write(int sockfd, const void *buf, size_t cout);

void close(int sockfd);
void shutdownWrite(int sockfd);
int getSocketError(int sockfd);

NetAddr getLocalAddr(int sockfd);
NetAddr getPeerAddr(int sockfd) ;


}

}

#endif
