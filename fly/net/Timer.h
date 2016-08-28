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

#endif