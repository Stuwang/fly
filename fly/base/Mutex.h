#ifndef FLY_MUTEX_H__
#define FLY_MUTEX_H__

#include "Config.h"

#include <base/Types.h>

#include <pthread.h>


namespace fly {

class Mutex: noncopyable
{
public:
	Mutex()
	{
		pthread_mutex_init(&mutex_, NULL);
	};

	~Mutex() {
		pthread_mutex_destroy(&mutex_);
	};

	bool isLocked() {
		bool locked ( 0 != pthread_mutex_trylock(&mutex_) );
		if (!locked) {
			unlock();
		}
		return locked;
	};

	void lock() {
		pthread_mutex_lock(&mutex_);
	};

	void unlock() {
		pthread_mutex_unlock(&mutex_);
	};

	Mutex* getMutex() {
		return this;
	};

	pthread_mutex_t* getRowMutex() {
		return &mutex_;
	}

private:
	pthread_mutex_t mutex_;
};

class LockGuard: noncopyable {
public:
	explicit LockGuard(Mutex& mutex)
		: mutex_(mutex)
	{
		assert(!mutex_.isLocked());
		mutex_.lock();
	};
	~LockGuard() {
		assert(mutex_.isLocked());
		mutex_.unlock();
	};
	Mutex* getMutex() {
		return &mutex_;
	}
private:
	Mutex& mutex_;
};

struct HasLocked_t {};
struct DonnotLock_t {};

class UniqueLock: noncopyable  {
public:
	explicit UniqueLock(Mutex& mutex)
		: mutex_(&mutex)
	{
		assert(!mutex_->isLocked());
		mutex_->lock();
	};

	UniqueLock(Mutex& mutex, HasLocked_t)
		: mutex_(&mutex)
	{
	};

	UniqueLock(Mutex& mutex, DonnotLock_t)
		: mutex_(&mutex)
	{

	};

	~UniqueLock() {
		if (!mutex_->isLocked()) {
			mutex_->unlock();
		}
	};

	void lock() {
		assert(!mutex_->isLocked());
		mutex_->lock();
	};

	void unlock() {
		assert(mutex_->isLocked());
		mutex_->unlock();
	};

	bool isLocked() const {
		return mutex_->isLocked();
	};

	Mutex* getMutex() {
		return mutex_;
	};

private:
	Mutex* mutex_;
};

}

#endif