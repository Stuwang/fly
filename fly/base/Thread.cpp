#include <base/Thread.h>

namespace fly {

Thread::Thread(const ThreadFunc& func, const std::string& name )
	: running_(false)
	, joined_(false)
	, id_(0)
	, func_(func)
	, name_(name)
{
	if (::pthread_create(&id_, NULL, &Thread::thread_func, this) ) {
		running_ = false;
	}
};

Thread::~Thread() {
	if (running_ && !joined_) {
		pthread_detach(id_);
	}
};

int Thread::join() {
	joined_ = true;
	return pthread_join(id_, NULL);
};

pid_t Thread::tid() {
	return id_;
};

const std::string& Thread::name() {
	return name_;
};

void Thread::setDefaultName() {
	name_ = "default";
};

void* Thread::thread_func(void * data) {
	assert(data);
	Thread *p = static_cast<Thread*>(data);
	p->id_ = CurrentThread::detail::getTid();
	p->running_ = true;
	try {
		p->func_();
	} catch (const std::exception & e) {

	} catch (...) {
		throw;
	}
	return NULL;
};

}