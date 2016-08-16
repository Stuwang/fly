#include <net/EventLoop.h>

namespace fly {

EventLoop::EventLoop()
	: poller_(GetNewPoller())
	, weakup_chan_(socketops::creatEventFd(), poller_ )
	, looping_ (false)
	, quit_ (false)
{
	weakup_chan_.setReadCallBack(std::bind(&EventLoop::HandleRead, this));
	weakup_chan_.enableRead();
};

EventLoop::~EventLoop() {};

void EventLoop::Loop() {
	looping_ = true;
	while (!quit_) {
		int num = poller_->poll(1000, &chans_);
		IsCallFunctors_ =false;

		if (num != 0) {
			for (auto &i : chans_) {
				i->handleEvents();
			}
		}
		DoFuncs();
	}
	looping_ = false;
};

void EventLoop::DoFuncs(){
	std::list<Functor>	funcs;
	{
		LockGuard lock(mutex_);
		funcs.swap(funcs_);
		IsCallFunctors_ =true;
	}
	for(auto &i :funcs){
		i();
	}
};

void EventLoop::HandleRead() {
	int fd = weakup_chan_.getfd();
	uint64_t one = 1;
	size_t n = socketops::read(fd, &one, sizeof(uint64_t));
	if (n != sizeof(one)) {

	}
};

void EventLoop::WeakUp() {
	int fd = weakup_chan_.getfd();
	uint64_t one = 1;
	size_t n = socketops::write(fd, &one, sizeof(uint64_t));
	if (n != sizeof(one)) {

	}
};

void EventLoop::quit() {
	quit_ = true;
	WeakUp();
};

bool EventLoop::IsInLoop() const {
	return pid_ == CurrentThread::tid();
};

void EventLoop::runInLoop(const Functor& func){
	if(IsInLoop()){
		func();
	}else{
		queueInLoop(func);
	}
};

void EventLoop::queueInLoop(const Functor& func){
	{
		LockGuard lock(mutex_);
		funcs_.push_back(func);
	}
	if(!IsInLoop() || IsCallFunctors_){
		WeakUp();
	}
};

};
