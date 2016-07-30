#ifndef FLY_THREAD_H__
#define FLY_THREAD_H__

#include <string>
#include <functional>
#include <pthread.h>

#include <base/Types.h>

namespace fly {

typedef std::function<void()> ThreadFunc;

class Thread : noncopyable {
public:
	Thread(const ThreadFunc& func, const std::string& name = std::string() );
	~Thread();
	int join();
	pid_t tid();
	const std::string& name();

	static void* thread_func(void *);
private:
	void setDefaultName();

	bool running_;
	bool joined_;
	pthread_t id_;
	ThreadFunc func_;
	std::string name_;

	static int thread_num_;
};

}

#endif