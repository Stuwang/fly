#include "Condition.h"

namespace fly {

bool Condition::Wait(UniqueLock& m, double timeout) {
	struct timespec abstime;
	// FIXME: use CLOCK_MONOTONIC or CLOCK_MONOTONIC_RAW to prevent time rewind.
	clock_gettime(CLOCK_REALTIME, &abstime);

	const long long kNanoSecondsPerSecond = 1e9;
	 long long nanoseconds = static_cast< long long>(timeout * kNanoSecondsPerSecond);

	abstime.tv_sec += static_cast< long long>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
	abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);

	return ETIMEDOUT == pthread_cond_timedwait(&cond_, m.getMutex()->getRowMutex(), &abstime);
}

bool Condition::Wait(LockGuard& m, double timeout) {
	struct timespec abstime;
	// FIXME: use CLOCK_MONOTONIC or CLOCK_MONOTONIC_RAW to prevent time rewind.
	clock_gettime(CLOCK_REALTIME, &abstime);

	const long long kNanoSecondsPerSecond = 1e9;
	 long long nanoseconds = static_cast< long long>(timeout * kNanoSecondsPerSecond);

	abstime.tv_sec += static_cast< long long>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
	abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);

	return ETIMEDOUT == pthread_cond_timedwait(&cond_, m.getMutex()->getRowMutex(), &abstime);
};

}