#ifndef FLY_TYPES_H__
#define FLY_TYPES_H__

#include "Config.h"

#if FLY_DEBUG
#  include <assert.h>
#endif

#include <cstdio>
#include <stdint.h>


namespace fly {

struct noncopyable {
	noncopyable() {};
	~noncopyable() {};
	noncopyable(const noncopyable&) ;
	noncopyable& operator=(const noncopyable&) ;
};

// template<class To, class From>
// inline down_cast(From *f) {
// #if FLY_DEBUG
// 	assert( f != 0 && dynamic_cast<To>(f) != NULL );
// #endif
// 	return static_cast<To>(f);
// };

}

#endif