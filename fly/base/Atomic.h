#ifndef FLY_ATOMIC_H__
#define FLY_ATOMIC_H__

#include <base/Types.h>


namespace fly {

template<class Type>
class AtomicT : noncopyable {
public:
	typedef Type T;
	AtomicT()
		: value_(0)
	{};

	T get()
	{
		// in gcc >= 4.7: __atomic_load_n(&value_, __ATOMIC_SEQ_CST)
		return __sync_val_compare_and_swap(&value_, 0, 0);
	}

	T getAndAdd(T x)
	{
		// in gcc >= 4.7: __atomic_fetch_add(&value_, x, __ATOMIC_SEQ_CST)
		return __sync_fetch_and_add(&value_, x);
	}

	T addAndGet(T x)
	{
		return getAndAdd(x) + x;
	}

	T incrementAndGet()
	{
		return addAndGet(1);
	}

	T decrementAndGet()
	{
		return addAndGet(-1);
	}

	void add(T x)
	{
		getAndAdd(x);
	}

	void increment()
	{
		incrementAndGet();
	}

	void decrement()
	{
		decrementAndGet();
	}

	T getAndSet(T newValue)
	{
		// in gcc >= 4.7: __atomic_store_n(&value, newValue, __ATOMIC_SEQ_CST)
		return __sync_lock_test_and_set(&value_, newValue);
	}


private:
	Type value_;
};

template class AtomicT<int32_t>;
template class AtomicT<int64_t>;

}
#endif