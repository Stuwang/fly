#ifndef FLY_CURRENTTHREAD_H___
#define FLY_CURRENTTHREAD_H___

#include <pthread.h>

namespace fly{

namespace CurrentThread{

extern thread_local int tid_;

inline int tid(){
	return tid_;
};

namespace detail {

int getTid();

};

}

}

#endif