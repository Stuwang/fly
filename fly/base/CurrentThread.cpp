#include <base/CurrentThread.h>

namespace fly {

namespace  CurrentThread {

int& ThreadTid() {
	static thread_local int tid_;
	return tid_;
}


namespace detail {

int getTid() {
	return pthread_self();
};

class ThreadInit {
public:
	ThreadInit() {
		ThreadTid() = getTid();
	};
};

static thread_local ThreadInit init_;

}; // detail

} // CurrentThread

} // fly
