#ifndef FLY_TIMERQUEUE_H__
#define FLY_TIMERQUEUE_H__

#include <net/Timer.h>
#include <net/EventLoop.h>

namespace fly {

class EventLoop;

// timer , not thread safe
class TimerQueue : noncopyable {
public:

	typedef std::function<void()> Functor;

	explicit TimerQueue(EventLoop* loop);

	~TimerQueue();

	void Add(Timer* p);

	void Delete(const uint64_t timerid);

	void DoFunctors();

	void handleRead();

private:
	void AddTimerInLoop(Timer* p);
	void setTime(const TimeDuration& time);
	void Close();


	EventLoop* loop_;
	int fd_;
	Channel * chann_;

	Time first_time_;

	typedef std::pair<Time, int64_t> Key;

	std::map<Key, Timer*> time_timers_;
	std::map<int64_t, Timer*> id_timers_;

	Mutex mutex_;
};

}

#endif