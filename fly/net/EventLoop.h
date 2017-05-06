#ifndef FLY_EVENTLOOP_H__
#define FLY_EVENTLOOP_H__

#include <list>
#include <memory>

#include <base/Mutex.h>
#include <base/Logger.h>
#include <base/Condition.h>
#include <base/CurrentThread.h>
#include <base/Types.h>

#include <net/TimerQueue.h>
#include <net/Channel.h>
#include <net/Poller.h>
#include <net/SocketOps.h>

namespace fly {

typedef std::function<void()> Functor;

class TimerQueue;

class EventLoop : noncopyable {
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

	int64_t RunAt(const Time& t, const Functor& f);
	int64_t RunAfter(const TimeDuration& t, const Functor& f);
	int64_t RunEvery(const TimeDuration& t, const Functor& f);
	void cancleTimer(int64_t id);
public:
	Poller *poller_;
	std::unique_ptr<Channel> weakup_chan_;
	ChannelList chans_;

	TimerQueue* timerqueue_;

	bool shouldSendEvent_;
	bool quit_;

	pthread_t pid_;

	std::list<Functor> funcs_;
	Mutex mutex_;
};

};

#endif
