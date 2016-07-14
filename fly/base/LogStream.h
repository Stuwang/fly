#ifndef FLY_LOGSTREAM_H__
#define FLY_LOGSTREAM_H__

#include <cstdlib>
#include <cstring>
#include <string>
#include <algorithm>

namespace fly {

class LogBuf{
public:
	size_t avail();
	size_t append(const char*data,int len);
	void add(size_t len);
	char* current();
};

class LogStream {
public:
	typedef LogBuf BufferT;

	friend LogStream& operator<<(LogStream& self, char);
	friend LogStream& operator<<(LogStream& self, unsigned char);

	friend LogStream& operator<<(LogStream& self, short);
	friend LogStream& operator<<(LogStream& self, unsigned short);
	friend LogStream& operator<<(LogStream& self, int);
	friend LogStream& operator<<(LogStream& self, unsigned int);
	friend LogStream& operator<<(LogStream& self, long);
	friend LogStream& operator<<(LogStream& self, unsigned long);
	friend LogStream& operator<<(LogStream& self, long long);
	friend LogStream& operator<<(LogStream& self, unsigned long long);

	friend LogStream& operator<<(LogStream& self, const char* str);
	friend LogStream& operator<<(LogStream& self, const unsigned char* str);

	friend LogStream& operator<<(LogStream& self, const std::string& str);

	friend LogStream& operator<<(LogStream& self, const void*);

private:
	template<class T>
	void append(T value);
	void append(const char*data, int len);
	BufferT buffer_;

	static const int MaxNumSize = 64;
};

}


#endif