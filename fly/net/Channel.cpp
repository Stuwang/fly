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
	if (r_events_ & (EPOLLIN | EPOLLPRI) ) {
		if (readCallBack_) readCallBack_();
	}

	if (r_events_ & EPOLLOUT ) {
		if (writeCallBack_) writeCallBack_();
	}

	if (r_events_ & EPOLLERR) {
		if (errorCallBack_) errorCallBack_();
	}

	if ( (r_events_ & EPOLLHUP ) && !(r_events_ & EPOLLIN) ) {
		if (closeCallBack_) closeCallBack_();
	}
};

}