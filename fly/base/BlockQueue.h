#ifndef FLY_BLOCKQUEUE_H__
#define FLY_BLOCKQUEUE_H__

#include <deque>

#include <base/Types.h>
#include <base/Mutex.h>
#include <base/Condition.h>

namespace  fly{

template<class T>
class BlockQueue:  noncopyable{
public:
	void put(const T& t){
		LockGuard lock(mutex_);
		data_.push_back(t);
		cond_.Notify();
	};

	T get(){
		LockGuard lock(mutex_);
		while(data_.empty()){
			cond_.wait(mutex_);
		}
		T ret(data_.front());
		data_.pop_front();
		return ret;
	};

	size_t size(){
		return data_.size();
	};
private:
	std::queue<T> data_;
	Condition cond_;
	Mutex mutex_;
};

}

#endif
