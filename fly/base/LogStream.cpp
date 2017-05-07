#include <base/LogStream.h>

namespace fly {

const char *digits = "9876543210123456789" + 9;
// const char digitsHex[] = "0123456789ABCDEF";

template<typename T>
size_t convert(char buf[], T value) {
	T i = value;
	char *p = buf;
	do {
		int l = static_cast<int>(i % 10);
		i /= 10;
		*p++ = digits[l];
	} while (i != 0);

	if (value < 0) {
		*p++ = '-';
	}
	std::reverse(buf, p);

	return p - buf;
};

// template<typename T>
// size_t convertHex(char buf[], long value) {
// 	long i = value;
// 	char *p = buf;
// 	do {
// 		int l = static_cast<int>(i % 16);
// 		i /= 10;
// 		*p++ = digitsHex[l];
// 	} while (i != 0);

// 	*p++ = '\0';
// 	std::reverse(buf, p);

// 	return p - buf;
// };

template<class T>
void LogStream::append(T value) {
	if (buffer_.avail() > MaxNumSize) {
		size_t len = convert(buffer_.current(), value);
		buffer_.add(len);
	}
};

void LogStream::append(const char*data, int len) {
	buffer_.append(data, len);
};

#define LOGSTREAM_HELPER(type)	\
LogStream& operator<<(LogStream& self, type value){\
	self.append(value);\
	return self;\
};

LOGSTREAM_HELPER(short);
LOGSTREAM_HELPER(unsigned short);
LOGSTREAM_HELPER(int);
LOGSTREAM_HELPER(unsigned int);
LOGSTREAM_HELPER(long);
LOGSTREAM_HELPER(unsigned long);
LOGSTREAM_HELPER(long long);
LOGSTREAM_HELPER(unsigned long long);

#undef LOGSTREAM_HELPER

LogStream& operator<<(LogStream& self, char value) {
	self.append(&value, 1);
	return self;
};

LogStream& operator<<(LogStream& self, unsigned char value) {
	self.append(reinterpret_cast<char*>(&value), 1);
	return self;
};

LogStream& operator<<(LogStream& self,
                      const char* str) {
	self.append(str, ::strlen(str));
	return self;
};
LogStream& operator<<(LogStream& self,
                      const unsigned char* str) {
	const char* p = reinterpret_cast<const char*>(str);
	self.append(p, ::strlen(p));
	return self;
};

LogStream& operator<<(LogStream& self, const std::string& str) {
	self.append(str.data(), str.length());
	return self;
};

LogStream& operator<<(LogStream& self, const StringView& v) {
	self.append(v.data(), v.size());
	return self;
};

// 
namespace inner{
}

LogStream& operator << (LogStream& out, const inner::InnerMinStr& str) {
	out.append(str.data(), str.size());
	if (str.needSpace()) {
		int len = str.spaceLen();
		char space[len];
		memset(space, ' ', len);
		out.append(space, len);
	}
	return out;
};

}