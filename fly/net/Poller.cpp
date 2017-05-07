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
	int num = ::epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(EventsInitSize), timeout );
	if ( num > 0 ) {
		fillChanS(num, chans);
		// return num;
	} else if (num == -1) {
		if (errno != EINTR) {
			LOG_ERROR << "epoll wait error " << errno;
		}
		// return 0;
	}
	LOG_DEBUG << "here " << num
	          <<  "chan size " << chans->size();
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

void Poller::addChannel(Channel* chan) {
	assert(!hasChannel(chan));
	int sockfd = chan->getfd();
	channels_[sockfd] = chan;
	update(sockfd, EPOLL_CTL_ADD, chan->events(), chan);
};

void Poller::updateChannel(Channel* chan) {
	assert(hasChannel(chan));
	int sockfd = chan->getfd();
	update(sockfd, EPOLL_CTL_MOD, chan->events(), chan);
};

void Poller::removeChannel(Channel* chan) {
	int sockfd = chan->getfd();
	ChannelMap::const_iterator it = channels_.find(sockfd);
	assert(hasChannel(chan));
	channels_.erase(sockfd);
	update(sockfd, EPOLL_CTL_DEL, it->second->events(), chan);
};

// void Poller::update(int operation, Channel *chan) {
// 	int fd = chan->getfd();
// 	struct epoll_event event;
// 	bzero(&event, sizeof(event));
// 	event.events = chan->events();
// 	event.data.ptr = static_cast<void*>(chan);
// 	if (::epoll_ctl(epollfd_, operation, fd, &event) < 0 ) {
// 		LOG_DEBUG << "operation error : "
// 		          << operation;
// 	}
// };

void Poller::update(int sockfd, int operation, int events, void *data) {
	struct epoll_event event;
	bzero(&event, sizeof(event));
	event.events = events;
	event.data.ptr = data;
	if (::epoll_ctl(epollfd_, operation, sockfd, &event) < 0 ) {
		LOG_DEBUG << "operation error : "
		          << operation;
	}
};

bool Poller::hasChannel(Channel* chan) {
	ChannelMap::const_iterator it = channels_.find(chan->getfd());
	return it != channels_.end() && it->second == chan;
};

Poller *GetNewPoller() {
	return new Poller();
};

}
