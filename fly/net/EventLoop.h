#ifndef FLY_EVENTLOOP_H__
#define FLY_EVENTLOOP_H__

#include <base/Mutex.h>
#include <base/Condition.h>

#include <net/Channel.h>
#include <net/Poller.h>
#include <net/SocketOps.h>

namespace fly {

class EventLoop {
public:
	EventLoop();
	~EventLoop();
	void Loop();
public:
	void HandleRead();
	void WeakUp();
	void quit();
	Poller *getPoller() {return poller_;};
public:
	Poller *poller_;
	Channel weakup_chan_;
	ChannelList chans_;

	bool looping_;
	bool quit_;
};

}

#endif