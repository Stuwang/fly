#include <TimerQueue.h>

namespace fly {

TimerQueue::TimerQueue(EventLoop* loop)
	: fd_(socketops::creatTimerFd())
	, chann_(new Channel(fd_, loop->getPoller()))
{
	chann_->setReadCallBack(std::bind(&TimerQueue::handleRead, this));
	chann_->enableRead();
	first_time_ = LocalClock::Now() + Hours(24);
};

void TimerQueue::Add(const Timer* p) {

};

void TimerQueue::Delete(const uint64_t timerid) {

};

void TimerQueue::DoFunctors() {
	auto now = LocalClock::Now();
	auto i = time_timers_.begin();
	std::map<Time, Timer*> repeats_;
	while (i != time_timers_.end() && i->first <= now ) {
		i->second->Run();
		if (i->second->repeat()) {
			auto time = now + i->second->RepeatTime();
			time_timers_[time] = i->second;
		}
		i++;
	}
	time_timers_.erase(time_timers_.begin(), i);

};

void TimerQueue::handleRead() {
	uint64_t exp;
	int s = read(fd_, &exp, sizeof(uint64_t));
	if (s != sizeof(uint64_t)) {

	};

	DoFunctors();

	if (time_timers_.empty()) {
		Close();
	} else {
		auto time = time_timers_.begin()->first - LocalClock::Now();
		setTime(time);
	}
}

void TimerQueue::setTime(const TimeDuration& time) {
	struct itimerspec data;
	data.it_value.tv_sec = time.Ms() / 1000000;
	data.it_value.tv_nsec = ( time.Ms() % 1000000) * 1000;

	data.it_interval.tv_sec = 0;
	data.it_interval.tv_nsec = 0;

	::timerfd_settime(fd_, 0, &data, NULL);
	first_time_ = LocalClock::Now() + time;
};

void TimerQueue::Close() {
	struct itimerspec data;
	data.it_value.tv_sec = 0;
	data.it_value.tv_nsec = 0;

	data.it_interval.tv_sec = 0;
	data.it_interval.tv_nsec = 0;

	::timerfd_settime(fd_, 0, &data, NULL);
};


}