#include <net/Channel.h>

namespace fly {

Channel::Channel(int sockfd, Poller* poller)
	: events_(0)
	, r_events_(0)
	, poller_(poller)
	, sockfd_(sockfd)
	, addedEvents_(false)
{

};

Channel::~Channel() {
	disableAll();
	remove();
	socketops::close(getfd());
	LOG_INFO << "Channel close";
};

void Channel::update() {
	poller_->updateChannel(this);
	addedEvents_ = true;
};

void Channel::remove() {
	poller_->removeChannel(this);
	addedEvents_ = false;
};

void Channel::handleEvents() {
	LOG_INFO << "handle event " << sockfd_ << " events " << EventToString(r_events_);

	if ( (r_events_ & EPOLLRDHUP ) && (r_events_ & EPOLLIN) ) {
		LOG_INFO << "handle close" ;
		if (closeCallBack_) {
			closeCallBack_();
			return;
		}
	}

	if ( r_events_ & EPOLLIN  ) {
		LOG_INFO << "handle read" ;
		if (readCallBack_) {
			readCallBack_();
			return;
		}
	}

	if (r_events_ & EPOLLOUT ) {
		LOG_INFO << "handle write" ;
		if (writeCallBack_) writeCallBack_();
	}

	if (r_events_ & EPOLLERR) {
		LOG_INFO << "handle error" ;
		if (errorCallBack_) errorCallBack_();
	}
};

std::string Channel::EventToString(int event) {
	std::string msg;

#define TEST(e)\
	if (event & e) {\
		msg += #e;\
		msg+= " ";\
	}

	TEST(EPOLLHUP);
	TEST(EPOLLIN);
	TEST(EPOLLOUT);
	TEST(EPOLLERR);

#undef TEST
	return msg;
};

}