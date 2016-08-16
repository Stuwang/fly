#include <iostream>

#include <base/fly_base.h>
#include <net/fly_net.h>

using namespace std;
using namespace fly;

int main() {

	EventLoop service;

	struct sockaddr_in addr;
	socketops::fromIpPort("127.0.0.1", 8060, &addr);
	Accepter accepter(&service, addr, false);
	accepter.setNewConnectionCallBack([&service](int sockfd, const struct sockaddr_in & a) {
		char buf[64];
		socketops::toIpPort(buf, 64, socketops::sockaddr_cast(&a) );
		std::cout << "GetConnection from " << buf << endl;
		socketops::close(sockfd);
	});
	accepter.listen();
	service.Loop();
};
