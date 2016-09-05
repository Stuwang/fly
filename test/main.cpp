#include <iostream>

#include <base/fly_base.h>
#include <net/fly_net.h>

using namespace std;
using namespace fly;
using namespace fly::socketops;

std::vector<TcpConPtr> &Conns() {
	static std::vector<TcpConPtr> conns_;
	return conns_;
};

void test_net() {
	signal_init<> _signal_init_;
	ThreadInit _init;

	EventLoop service;

	assert(service.IsInLoop());


	LOG_INFO << "test time " << LocalClock::Now().ToString() ;
	LOG_INFO << "test time " << LocalClock::ToDay().ToString() ;

	struct sockaddr_in addr;
	socketops::fromIpPort("127.0.0.1", 8060, &addr);
	Accepter accepter(&service, addr, false);
	accepter.setNewConnectionCallBack([&service](int sockfd,
	const struct sockaddr_in & a) {
		LOG_INFO << "socket " << sockfd << " New Connection ,address " << socketops::toIpPort(sockaddr_cast(&a)) ;
		TcpConPtr ptr( new TcpConnection(&service, sockfd));
		ptr->ReadCallBack([](const TcpConPtr & conn) {
			auto &buf = *( conn->readBuffer() );
			int len = buf.ReadAbleBytes();
			std::string str(buf.data(), len);
			buf.retireRead(len);
			LOG_INFO << " read " << len ;//<< " msg:" << str;
			conn->Send(str);
		});

		ptr->closeCallBack([](const TcpConPtr & conn) {
			if (conn.use_count() == 1) {
				LOG_INFO << "close And debug----";
			} else {
				LOG_INFO << "just erase";
			}
			auto &m = Conns();
			auto i =  std::find(m.begin(), m.end(), conn);
			if (i != m.end()) {
				m.erase(i);
			}
			LOG_INFO << "close TcpConnection from server";
		});

		ptr->Start();
		Conns().push_back(ptr);
	});
	LOG_INFO << fmt("%p", (void*)&service);
	accepter.listen();
	service.Loop();
};

void test_timer() {

}

int main() {
	signal_init<> _signal_init_;
	ThreadInit _init;

	EventLoop service;

	service.RunAt(LocalClock::Now()+Seconds(2),[](){
		std::cout << "..." << std::endl;
	});

	service.Loop();
};
