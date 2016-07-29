#ifndef FLY_POLLER_H__
#define FLY_POLLER_H__

#include <vector>
#include <map>
#include <cstring>
#include <assert.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <net/Channel.h>

namespace fly {

class Channel;

typedef std::vector<Channel*> ChannelList;

class Poller {
public:
	Poller();
	~Poller();
	int poll(int timeout, ChannelList *chans);

	void updateChannel(Channel* chan);
	void removeChannel(Channel* chan);
	bool hasChannel(Channel* chan);
private:
	void update(int operation,Channel *chan);
	void fillChanS(int num, ChannelList*) const;

	typedef std::map<int, Channel*> ChannelMap;
	typedef std::vector<struct epoll_event> EvecnList;

	ChannelMap channels_;
	int epollfd_;
	EvecnList events_;

	static const int EventsInitSize = 64;
};

Poller *GetPoller();

}

#endif