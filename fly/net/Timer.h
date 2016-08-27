#ifndef FLY_TIMER_H__
#define FLY_TIMER_H__

#include <stdint.h>
#include <functional>
#include <map>

#include <base/Atomic.h>
#include <base/Types.h>
#include <base/Time.h>

#include <net/Channel.h>
#include <net/SocketOps.h>

namespace fly {

class Timer : noncopyable {
public:
	typedef std::function<void()> Functor;
	Timer(const Functor& f, const TimeDuration& first)
		: id_(GetNewId())
		, first_time_(first)
		, repeat_(false)
		, f_(f)
	{};
	Timer(const Functor& f, const TimeDuration& first, const TimeDuration& repeat): id_(GetNewId())
		, first_time_(first)
		, repeat_(true)
		, repeat_time_(repeat)
		, f_(f)
	{};

	bool repeat() const {return repeat_;};
	TimeDuration FirstTime() const {return first_time_;};
	TimeDuration RepeatTime() const {return repeat_time_;};
	int64_t Id() const {return id_;};
	void Run() {
		if (f_) {
			f_();
		}
	};
private:
	int64_t id_;
	TimeDuration first_time_;
	bool repeat_;
	TimeDuration repeat_time_;
	Functor f_;

	static inline  int64_t GetNewId() {
		return static_id_.incrementAndGet();
	};
	static AtomicT<int64_t> static_id_;
};

}


// namespace fly {

// // timer , not thread safe
// class TimerQueue : noncopyable {
// public:

// 	typedef std::function<void()> Functor;

// 	TimerQueue(EventLoop* loop)
// 		: fd_(socketops::creatTimerFd())
// 		, chann_(new Channel(fd_, loop))
// 	{
// 		chann_->setReadCallBack(std::Pbind(TimerQueue::handleRead, this));
// 		chann_->enableRead();
// 		first_time_ = LocalClock::Now() + Hours(24);
// 	};

// 	void Queue(const TimeDuration& time, const Functor& f) {
// 		Time time_tmp = LocalClock::Now() + time;
// 		callbacks_[time_tmp] = f;
// 		if (time_tmp < first_time_) {
// 			setnext(time);
// 		};
// 	};

// 	void DoFunctors() {
// 		auto time = LocalClock::Now();
// 		auto i = callbacks_.begin();
// 		while (i != callbacks_.end() && i->first <= time ) {
// 			if (i->second) {
// 				i->second();
// 			}
// 			i++;
// 		}
// 		callbacks_.erase(callbacks_.begin(), i);
// 	}


// 	void handleRead() {
// 		uint64_t exp;
// 		int s = read(fd_, &exp, sizeof(uint64_t));
// 		if (s != sizeof(uint64_t)) {

// 		};

// 		DoFunctors();

// 		if (callbacks_.empty()) {
// 			setnext(Hours(24));
// 		} else {
// 			auto i = callbacks_.begin();
// 			auto t = i->first - LocalClock::Now();
// 			setnext(t);
// 		}
// 	}

// private:
// 	void setnext(const TimeDuration& time) {
// 		struct itimerspec data;
// 		data.it_value.tv_sec = time.Ms() / 1000000;
// 		data.it_value.tv_nsec = ( time.Ms() % 1000000) * 1000;

// 		data.it_interval.tv_sec = 0;
// 		data.it_interval.tv_nsec = 0;

// 		::timerfd_settime(fd_, 0, &data, NULL);
// 		first_time_ = LocalClock::Now() + time;
// 	};


// 	int fd_;
// 	Channel * chann_;

// 	Time first_time_;
// 	std::map<Time, Functor> callbacks_;
// };

// }

#endif