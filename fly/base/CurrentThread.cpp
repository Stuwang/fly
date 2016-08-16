#include <base/CurrentThread.h>

namespace fly {

namespace  CurrentThread {

thread_local int tid_;

namespace detail {

int getTid() {
	return pthread_self();
};

class ThreadInit {
public:
	ThreadInit() {
		tid_ = getTid();
	};
};

static thread_local ThreadInit init_;

}; // detail

} // CurrentThread

} // fly
