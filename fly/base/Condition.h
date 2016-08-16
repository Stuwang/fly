#ifndef FLY_CONDITION_H__
#define FLY_CONDITION_H__


#include "Config.h"

#include <base/Types.h>
#include <base/Mutex.h>

#include <errno.h>
#include <pthread.h>


namespace fly {

class Condition: noncopyable{
public:
	Condition() {
		pthread_cond_init(&cond_, NULL);
	};
	~Condition() {
		pthread_cond_destroy(&cond_);
	};

	void Wait(LockGuard& m) {
		pthread_cond_wait(&cond_, m.getMutex()->getRowMutex());
	};

	void Wait(UniqueLock& m) {
		pthread_cond_wait(&cond_, m.getMutex()->getRowMutex());
	};

	bool Wait(UniqueLock& m, double waittime);
	bool Wait(LockGuard& m, double waittime);

	void Notify() {
		pthread_cond_signal(&cond_);
	};

	void NotifyAll() {
		pthread_cond_broadcast(&cond_);
	};
private:
	pthread_cond_t cond_;

	friend class Mutex;
	friend class UniqueLock;
	friend class LockGuard;
};

}

#endif
