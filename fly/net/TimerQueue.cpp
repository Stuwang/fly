#include <net/TimerQueue.h>

namespace fly {

TimerQueue::TimerQueue(EventLoop* loop)
	: loop_(loop)
	, fd_(socketops::creatTimerFd())
	, chann_(new Channel(fd_, loop->getPoller()))

{
	chann_->setReadCallBack(std::bind(&TimerQueue::handleRead, this));
	chann_->enableRead();
	first_time_ = LocalClock::Now() + Hours(24);
};

TimerQueue::~TimerQueue() {
	chann_->disableAll();
	for (auto i : time_timers_) {
		delete i.second;
	}
	time_timers_.clear();
	id_timers_.clear();
	delete chann_;

};

void TimerQueue::Add(Timer* p) {
	if (loop_->IsInLoop()) {
		AddTimerInLoop(p);
	} else {
		loop_->queueInLoop(std::bind(&TimerQueue::AddTimerInLoop, this, p));
	}
};

void TimerQueue::AddTimerInLoop(Timer* p) {
	time_timers_[Key(p->GetNextTime(), p->Id())] = p;
	id_timers_[p->Id()] = p;
	first_time_ = time_timers_.begin()->first.first;
	setTime(first_time_ - LocalClock::Now());
}

void TimerQueue::Delete(const uint64_t timerid) {
	LockGuard lock(mutex_);
	auto i = id_timers_.find(timerid);
	if (i == id_timers_.end()) {
		return;
	} else {
		Timer *p = i->second;
		time_timers_.erase(Key(p->GetNextTime(), p->Id()));
		id_timers_.erase(i);
	}
};

void TimerQueue::DoFunctors() {
	auto now = LocalClock::Now();
	auto i = time_timers_.begin();
	while (i != time_timers_.end() && i->first.first <= now ) {
		i->second->Run();
		if (i->second->repeat()) {
			LOG_INFO << "repeat";
			auto time = now + i->second->RepeatTime();
			i->second->SetNextTime(time);
			time_timers_[Key(time, i->second->Id())] = i->second;
		} else {
			auto id = i->second->Id();
			id_timers_.erase(id);
			delete i->second;
		}
		time_timers_.erase(i);
		i = time_timers_.begin();
	}
};

void TimerQueue::handleRead() {
	uint64_t exp;
	int s = read(fd_, &exp, sizeof(uint64_t));
	if (s != sizeof(uint64_t)) {

	};
	{
		LockGuard lock(mutex_);
		DoFunctors();
		LOG_INFO << time_timers_.size();
		if (time_timers_.empty()) {
			Close();
		} else {
			LOG_INFO << LocalClock::Now().ToString();
			if (first_time_ < time_timers_.begin()->first.first) {
				first_time_ = time_timers_.begin()->first.first;
				auto time = first_time_ - LocalClock::Now();
				setTime(time);
				LOG_INFO << LocalClock::Now().ToString();
			}
		}
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