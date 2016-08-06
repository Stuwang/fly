#ifndef FLY_ENDIAN_H__
#define FLY_ENDIAN_H__

#include <algorithm>

namespace fly {

template<typename T>
inline T NetToHost(T value) {
	int len = sizeof(T);
	char *p = reinterpret_cast<char*>(&value);
	std::reverse(p, p + len);
	return value;
};

template<typename T>
inline T HostToNet(T value) {
	int len = sizeof(T);
	char *p = reinterpret_cast<char*>(&value);
	std::reverse(p, p + len);
	return value;
};

}

#endif