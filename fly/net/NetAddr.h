#ifndef FLY_NETADDT_H__
#define FLY_NETADDT_H__

#include <stdint.h>

#include "base/StringView.h"
#include "net/SocketOps.h"


namespace fly {

class NetAddr {
public:
	explicit NetAddr(uint16_t port);
	NetAddr(StringView ip, uint16_t port);
	// NetAddr(StringView ip_port);
	explicit NetAddr(const struct sockaddr_in& addr)
		: addr_(addr)
	{};

	explicit NetAddr(const struct sockaddr& addr)
		: addr_(*socketops::sockaddr_in_cast(&addr))
	{};

	std::string Ip()const {
		char buf[64] = {0};
		socketops::toIp(buf, sizeof(buf),
		                socketops::sockaddr_cast(&addr_));
		return buf;
	};

	std::string IpPort()const {
		char buf[64] = {0};
		socketops::toIpPort(buf, sizeof(buf),
		                    socketops::sockaddr_cast(&addr_));
		return buf;
	};

	uint16_t Port()const {
		return ntohs(addr_.sin_port);
	};

	const struct sockaddr* getSockAddr()const {
		return socketops::sockaddr_cast(&addr_);
	};

private:
	struct sockaddr_in addr_;
};

}

#endif
