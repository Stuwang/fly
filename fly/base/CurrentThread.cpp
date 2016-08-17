#include <base/CurrentThread.h>

namespace fly {

pthread_t& ThreadTid() {
	static thread_local pthread_t tid_;
	return tid_;
};

pthread_t tid() {
	return ThreadTid();
};

ThreadInit::ThreadInit() {
	ThreadTid() = pthread_self();
};

} // fly
