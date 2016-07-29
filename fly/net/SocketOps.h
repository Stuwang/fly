#ifndef _FLY_SOCKETOPS_H__
#define _FLY_SOCKETOPS_H__

#include <cstdlib>

#include <arpa/inet.h>
#include <string.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>  // snprintf
#include <strings.h>  // bzero
#include <sys/socket.h>
#include <unistd.h>
#include <sys/eventfd.h>

namespace fly {

namespace socketops {

int creatEventFd();

void setNoblockAndCloseOnExec(int socketfd);
void setNoDelay(int socketfd, bool on);
void setReuseAddr(int socketfd, bool on);
void setReusePort(int socketfd, bool on);
void setKeepAlive(int socketfd, bool on);
// void setBufSize(int socketfd, size_t bufsize);

int creatNoBlockOrDie();
int connect(int socketfd, const sockaddr* addr);
void bindOrDie(int socketfd, const sockaddr* addr);
void listenOrDie(int socketfd);
int accept(int socketfd, struct sockaddr_in *addr);

ssize_t read(int sockfd, void *buf, size_t cout);
ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);
ssize_t write(int sockfd, void *buf, size_t cout);

void close(int sockfd);
void shutdownWrite(int sockfd);

int getSocketError(int sockfd);

struct sockaddr_in getLocalAddr(int sockfd);

struct sockaddr_in getPeerAddr(int sockfd) ;

void toIpPort(char* buf, size_t size, const sockaddr *addr) ;

void toIp(char *buf, size_t size, const struct sockaddr*addr) ;

void fromIpPort(const char* ip, uint16_t port, struct sockaddr_in * addr);

const struct sockaddr* sockaddr_cast(const struct sockaddr_in* addr);
struct sockaddr* sockaddr_cast(struct sockaddr_in* addr);
const struct sockaddr_in* sockaddr_in_cast(const struct sockaddr* addr);

}

}

#endif