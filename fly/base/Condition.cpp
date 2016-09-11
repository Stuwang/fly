#include <base/Condition.h>

namespace fly {

bool Condition::TimeWait(UniqueLock& m, const TimeDuration& waittime){
	struct timespec abstime;
	clock_gettime(CLOCK_REALTIME, &abstime);

	long long nanoseconds = static_cast< long long>(waittime.Ms() * 1000);

	abstime.tv_sec += static_cast< long long>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
	abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);

	return ETIMEDOUT == pthread_cond_timedwait(&cond_, m.getMutex()->getRowMutex(), &abstime);

};

bool Condition::TimeWait(LockGuard& m, const TimeDuration& waittime){
	struct timespec abstime;
	clock_gettime(CLOCK_REALTIME, &abstime);

	long long nanoseconds = static_cast< long long>(waittime.Ms() * 1000);

	abstime.tv_sec += static_cast< long long>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
	abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);

	return ETIMEDOUT == pthread_cond_timedwait(&cond_, m.getMutex()->getRowMutex(), &abstime);
};

}
