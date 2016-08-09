#ifndef FLY_EVENTLOOP_H__
#define FLY_EVENTLOOP_H__

#include <list>

#include <base/Mutex.h>
#include <base/Condition.h>

#include <net/Channel.h>
#include <net/Poller.h>
#include <net/SocketOps.h>

namespace fly {

typedef std::function<void()> Functor;

class EventLoop {
public:
	EventLoop();
	~EventLoop();
	
	void Loop();
	void DoFuncs();
	
	void HandleRead();
	void WeakUp();
	void quit();
	Poller *getPoller() {return poller_;};

	bool IsInLoop() const ;

	void runInLoop(const Functor& fun);
	void queueInLoop(const Functor& fun);
public:
	Poller *poller_;
	Channel weakup_chan_;
	ChannelList chans_;

	bool looping_;
	bool IsCallFunctors_;
	bool quit_;

	pthread_t pid_;

	std::list<Functor> funcs_;
	Mutex mutex_;
};

};

#endif
