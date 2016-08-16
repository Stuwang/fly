#ifndef FLY_CURRENTTHREAD_H___
#define FLY_CURRENTTHREAD_H___

#include <pthread.h>

namespace fly {

namespace CurrentThread {

namespace detail {

int getTid();

};

int &ThreadTid();

inline int tid() {
	return pthread_self();
};



}

}

#endif