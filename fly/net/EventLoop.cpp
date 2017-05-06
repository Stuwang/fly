#include <net/EventLoop.h>

namespace fly {

EventLoop::EventLoop()
	: poller_(GetNewPoller())
	, weakup_chan_()
	, timerqueue_(new TimerQueue(this) )
	, shouldSendEvent_(false)
	, quit_ (false)
	, pid_(tid())
{
	weakup_chan_.reset(new Channel(poller_, socketops::creatEventFd()));
	LOG_DEBUG << "weaoup fd is " << weakup_chan_->getfd();
	weakup_chan_->setReadCallBack(std::bind(&EventLoop::HandleRead, this));
	weakup_chan_->enableRead();
};

EventLoop::~EventLoop() {
	LOG_DEBUG << " before delete timerqueue";
	delete timerqueue_;
	LOG_DEBUG << " after delete timerqueue";
	weakup_chan_.reset();
	LOG_DEBUG << "after event delete";
	delete poller_;
	LOG_DEBUG << " after delete polller";
};

void EventLoop::Loop() {
	while (!quit_) {
		LOG_DEBUG << "quit " << (quit_ ? "true " : "false");
		chans_.clear();
		int num = poller_->poll(-1, &chans_);
		shouldSendEvent_ = false;
		LOG_DEBUG << "poll size " << chans_.size() ;
		if (num != 0) {
			for (auto &i : chans_) {
				i->handleEvents();
			}
		}
		DoFuncs();
	}
	LOG_DEBUG << "loop end !";
};

void EventLoop::DoFuncs() {

	std::list<Functor>	funcs;
	{
		LockGuard lock(mutex_);
		funcs.swap(funcs_);
		shouldSendEvent_ = true;
	}
	for (auto &i : funcs) {
		i();
	}
	shouldSendEvent_ = false;

	//LOG_DEBUG << "Do Functor , thread id:" << pid_ ;
};

void EventLoop::HandleRead() {
	int fd = weakup_chan_->getfd();
	uint64_t one = 1;
	size_t n = socketops::read(fd, &one, sizeof(uint64_t));
	if (n != sizeof(one)) {
		// LOG_ERROR << "weak up read " << n << "bytes thread id:" << pid_ ;
	}
	LOG_INFO << "weak up read " << n << "bytes thread id:" << pid_ ;
};

void EventLoop::WeakUp() {
	int fd = weakup_chan_->getfd();
	uint64_t one = 1;
	size_t n = socketops::write(fd, &one, sizeof(uint64_t));
	if (n != sizeof(one)) {
		// LOG_ERROR << "weak up write " << n << "bytes thread id:" << pid_ ;
	}
	LOG_INFO << "weak up send " << n << "bytes thread id:" << pid_ ;
};

void EventLoop::quit() {
	LOG_DEBUG << "QUIT CALLED ";
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
	if (!IsInLoop() || shouldSendEvent_) {
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
