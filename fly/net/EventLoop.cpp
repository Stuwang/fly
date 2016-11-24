#include <net/EventLoop.h>

namespace fly {

EventLoop::EventLoop()
	: poller_(GetNewPoller())
	, weakup_chan_(socketops::creatEventFd(), poller_ )
	, timerqueue_(new TimerQueue(this) )
	, looping_ (false)
	, quit_ (false)
	, pid_(tid())
{
	weakup_chan_.setReadCallBack(std::bind(&EventLoop::HandleRead, this));
	weakup_chan_.enableRead();
};

EventLoop::~EventLoop() {
	delete timerqueue_;
	delete poller_;
};

void EventLoop::Loop() {
	looping_ = true;
	while (!quit_) {
		chans_.clear();
		int num = poller_->poll(-1, &chans_);
		IsCallFunctors_ = false;
		LOG_DEBUG << "poll size " << chans_.size() ;
		if (num != 0) {
			for (auto &i : chans_) {
				i->handleEvents();
			}
		}
		DoFuncs();
	}
	looping_ = false;
};

void EventLoop::DoFuncs() {
	std::list<Functor>	funcs;
	{
		LockGuard lock(mutex_);
		funcs.swap(funcs_);
		IsCallFunctors_ = true;
	}
	for (auto &i : funcs) {
		i();
	}
	//LOG_DEBUG << "Do Functor , thread id:" << pid_ ;
};

void EventLoop::HandleRead() {
	int fd = weakup_chan_.getfd();
	uint64_t one = 1;
	size_t n = socketops::read(fd, &one, sizeof(uint64_t));
	if (n != sizeof(one)) {
		// LOG_ERROR << "weak up read " << n << "bytes thread id:" << pid_ ;
	}
	LOG_INFO << "weak up read " << n << "bytes thread id:" << pid_ ;
};

void EventLoop::WeakUp() {
	int fd = weakup_chan_.getfd();
	uint64_t one = 1;
	size_t n = socketops::write(fd, &one, sizeof(uint64_t));
	if (n != sizeof(one)) {
		// LOG_ERROR << "weak up write " << n << "bytes thread id:" << pid_ ;
	}
	LOG_INFO << "weak up send " << n << "bytes thread id:" << pid_ ;
};

void EventLoop::quit() {
	quit_ = true;
	WeakUp();
};

bool EventLoop::IsInLoop() const {
	return pid_ == tid();
};

void EventLoop::runInLoop(const Functor& func) {
	if (IsInLoop()) {
		func();
	} else {
		queueInLoop(func);
	}
};

void EventLoop::queueInLoop(const Functor& func) {
	{
		LockGuard lock(mutex_);
		funcs_.push_back(func);
	}
	if (!IsInLoop() || IsCallFunctors_) {
		WeakUp();
	}
};


int64_t EventLoop::RunAt(const Time& t, const Functor& f) {
	Timer *p = new Timer(f, t - LocalClock::Now());
	timerqueue_->Add(p);
	return p->Id();
};

int64_t EventLoop::RunAfter(const TimeDuration& t, const Functor& f) {
	Timer *p = new Timer(f, t );
	timerqueue_->Add(p);
	return p->Id();
};

int64_t EventLoop::RunEvery(const TimeDuration& t, const Functor& f) {
	Timer *p = new Timer(f, t, t);
	timerqueue_->Add(p);
	return p->Id();
};

void EventLoop::cancleTimer(int64_t id) {
	timerqueue_->Delete(id);
};

};
