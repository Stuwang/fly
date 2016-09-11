#ifndef FLY_CONDITION_H__
#define FLY_CONDITION_H__


#include "Config.h"

#include <base/Types.h>
#include <base/Mutex.h>
#include <base/Time.h>

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

	bool TimeWait(UniqueLock& m, const TimeDuration& waittime);
	bool TimeWait(LockGuard& m, const TimeDuration& waittime);

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
	
	static const long long kNanoSecondsPerSecond = 1e9;
};

}

#endif
