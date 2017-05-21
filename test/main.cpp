#include <iostream>

#include <base/fly_base.h>
#include <net/fly_net.h>

using namespace std;
using namespace fly;
// using namespace fly::socketops;

template<int Sin>
class Singnal {
public:
	typedef std::function<void()> Functor;
	explicit Singnal(const Functor& f) {
		LOG_INFO << "signal";
		GetCallBack() = f;
		::signal(Sin, &callback);
	};

	static void callback(int) {
		LOG_INFO << "signal" << Sin;
		GetCallBack()();
	};

	static Functor& GetCallBack() {
		static Functor f;
		return f;
	};
};

std::vector<TcpConPtr> &Conns() {
	static std::vector<TcpConPtr> conns_;
	return conns_;
};

void test_net() {
	// Signal_ignore<SIGPIPE> __;
	Signal::signal(SIGPIPE, []() {
		LOG_INFO << "SIGPIPE OCCR";
	});
	ThreadInit _init;

	EventLoop service;

	assert(service.IsInLoop());


	LOG_INFO << "test time " << LocalClock::Now().ToString() ;
	LOG_INFO << "test time " << LocalClock::ToDay().ToString() ;

	NetAddr addr(8060);
	Accepter accepter(&service, addr, false);
	accepter.setNewConnectionCallBack([&service](int sockfd,
	const NetAddr & a) {
		LOG_INFO << "socket " << sockfd << " New Connection ,address "
		         << a.IpPort();
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

	Signal::signal(SIGINT, [&]() {
		auto & ts = Conns();
		for (auto &i : ts) {
			i->forceClose();
		}
		ts.clear();
		service.quit();
	});

	service.Loop();
	LOG_DEBUG << "out of loop";
};

void test_timer() {
	//signal_init<> _signal_init_;
	ThreadInit _init;

	EventLoop service;

	service.RunAt(LocalClock::Now() + Seconds(2), []() {
		std::cout << "..." << std::endl;
	});

	service.Loop();
}

void TestLooger() {
	AsyncLogger::Init("yuanlog-");
	LOG_INFO << "hahhahahha";
}

int main() {
	LOG_DEBUG << "NOW , we mod it   ";
	test_net() ;
	// TestLooger();
	// sleep(5);
	LOG_INFO << "fuck" ;
	//Logger::g_flush();
};
