#include <net/Poller.h>

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
	int num = ::epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(channels_.size()), timeout );
	if ( num > 0 ) {
		fillChanS(num, chans);
	}
	return num;
};

void Poller::fillChanS(int num, ChannelList *chs) const {
	for (int i = 0; i < num; ++i) {
		Channel * ptr = static_cast<Channel*>(events_[i].data.ptr);
		// maybe add debug
		ptr->setRevents(events_[i].events);
		chs->push_back(ptr);
	}
};

void Poller::updateChannel(Channel* chan) {
	int fd = chan->getfd();
	if (chan->HasAdded()) {
		channels_[fd] = chan;
		update(EPOLL_CTL_ADD, chan);
	} else {
		update(EPOLL_CTL_MOD, chan);
	}
};

void Poller::removeChannel(Channel* chan) {
	int sockfd = chan->getfd();
	ChannelMap::const_iterator it = channels_.find(sockfd);
	assert(hasChannel(chan));
	channels_.erase(it->first);
	update(EPOLL_CTL_DEL, chan);
};

void Poller::update(int operation, Channel *chan) {
	int fd = chan->getfd();
	struct epoll_event event;
	bzero(&event, sizeof(event));
	event.events = chan->events();
	event.data.ptr = static_cast<void*>(chan);
	if (::epoll_ctl(epollfd_, operation, fd, &event) < 0 ) {

	}
};

bool Poller::hasChannel(Channel* chan) {
	ChannelMap::const_iterator it = channels_.find(chan->getfd());
	return it != channels_.end() && it->second == chan;
};

Poller *GetPoller() {
	return new Poller();
};

}