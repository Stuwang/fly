#include "NetAddr.h"

namespace fly {

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
