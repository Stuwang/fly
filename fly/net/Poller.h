#ifndef FLY_POLLER_H__
#define FLY_POLLER_H__

#include <vector>
#include <map>
#include <cstring>
#include <assert.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <base/Types.h>

#include <net/Channel.h>

namespace fly {

class Channel;

typedef std::vector<Channel*> ChannelList;

class Poller : noncopyable {
public:
	Poller();
	~Poller();
	int poll(int timeout, ChannelList *chans);

	void addChannel(Channel* chan);
	void updateChannel(Channel* chan);
	void removeChannel(Channel* chan);
	bool hasChannel(Channel* chan);
private:
	void update(int sockfd,int operation, int event,void *data);
	void fillChanS(int num, ChannelList*) const;

	typedef std::map<int, Channel*> ChannelMap;
	typedef std::vector<struct epoll_event> EvecnList;

	ChannelMap channels_;
	int epollfd_;
	EvecnList events_;

	static const int EventsInitSize = 64;
};

Poller *GetNewPoller();

}

#endif