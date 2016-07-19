#include "Poller.h"

namespace fly {

Poller::Poller()
	: epollfd_(epoll_create(EPOLL_CLOEXEC))
	, events_(EventsInitSize)
{
	if (epollfd_ < 0) {

	}
};

Poller::~Poller() {
	::close(epollfd_);
};

int Poller::poll(int timeout, ChannelList *chans) {
	int Num = ::epoll_wait(&*channels_.begin(), static_cast<int>(channels_.size()), timeout );
	if ( Num > 0 ) {
		fillChanS(Num, chans);
	}
};

void Poller::fillChanS(int num, ChannelList *chs) const {
	for (int i = 0; i < num; ++i) {
		Channel * ptr = static_cast<Channel*>(events_[i].data.ptr);
		// maybe add debug
		ptr->setRevents(events_[i].events);
		chs->push_back(ptr);
	}
};

void Poller::UpdateChannel(Channel* chan) {

};

void Poller::removeChannel(Channel* chan) {

};

bool Poller::hasChannel(Channel* chan) {
	ChannelMap::const_iterator it = channels_.find(chan->fd());
	return it != channels_.end() && it->second == chan;
};

}