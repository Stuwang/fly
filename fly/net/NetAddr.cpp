#include "NetAddr.h"

namespace fly {

namespace socketops {

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

struct sockaddr_in fromPort(uint16_t port) {
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
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

NetAddr::NetAddr(uint16_t port) {
	bzero(&addr_, sizeof(addr_));
	addr_.sin_family = AF_INET;
	addr_.sin_port = htons(port);
};

NetAddr::NetAddr(StringView ip, uint16_t port) {
	addr_.sin_family = AF_INET;
	addr_.sin_port = htons(port);
	if (::inet_pton(AF_INET, ip.data(), &addr_.sin_addr) <= 0 ) {
		LOG_FATAL << "error ip and port " << ip << " " << port ;
	}
};

// NetAddr::NetAddr(StringView ip_port) {

// };

}
