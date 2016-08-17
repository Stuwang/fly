#ifndef FLY_CURRENTTHREAD_H___
#define FLY_CURRENTTHREAD_H___

#include <pthread.h>

namespace fly {

pthread_t tid();

class ThreadInit {
public:
	ThreadInit();
};

}

#endif