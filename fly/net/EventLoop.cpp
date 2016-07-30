#include <net/EventLoop.h>

namespace fly {

EventLoop::EventLoop()
	: poller_(GetPoller())
	, weakup_chan_(socketops::creatEventFd(), poller_ )
	, looping_ (false)
	, quit_ (true)
{
	weakup_chan_.setReadCallBack(std::bind(&EventLoop::HandleRead,this));
	weakup_chan_.enableRead();
};

EventLoop::~EventLoop() {};

void EventLoop::Loop() {
	looping_ = true;
	while (!quit_) {
		int num = poller_->poll(1000, &chans_);

		if (num == 0) {
			return;
		}

		for (auto &i : chans_) {
			i->handleEvents();
		}
	}
	looping_ = false;
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

};