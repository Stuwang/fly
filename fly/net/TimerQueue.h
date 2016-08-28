#ifndef FLY_TIMERQUEUE_H__
#define FLY_TIMERQUEUE_H__

#include <net/Timer.h>
#include <net/EventLoop.h>

namespace fly {


// timer , not thread safe
class TimerQueue : noncopyable {
public:

	typedef std::function<void()> Functor;

	TimerQueue(EventLoop* loop);

	void Add(const Timer* p);

	void Delete(const uint64_t timerid);

	void DoFunctors();

	void handleRead();

private:
	void setTime(const TimeDuration& time);
	void Close();

	int fd_;
	Channel * chann_;

	Time first_time_;

	std::map<Time, Timer*> time_timers_;
	std::map<int64_t, Timer*> id_timers_;
};

}

#endif