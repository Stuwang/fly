#ifndef FLY_LOGSTREAM_H__
#define FLY_LOGSTREAM_H__

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <assert.h>

#include <base/Types.h>
#include <base/StringView.h>


namespace fly {

class LogBuf : noncopyable {
public:
	LogBuf() {
		data_ = new char[initSize];
		cur_ = 0;
		size_ = initSize;
	};
	~LogBuf() {
		delete [] data_;
	};
	size_t avail() const {
		return size_ - cur_;
	};
	size_t append(const char*data, int len) {
		int w = 0;
		if (size_ - cur_ > static_cast<size_t>(len) ) {
			w = len;
		} else {
			w = size_ - cur_;
		}
		memcpy(data_ + cur_, data, w);
		add(w);
		return w;
	};
	void add(size_t len) {
		assert(size_ - cur_ > len);
		cur_ += len;
	};
	char* current() {
		return data_ + cur_;
	};
	const char* data() const {
		return data_;
	};
	size_t length() const {
		return cur_;
	}
private:
	char * data_;
	size_t cur_;
	size_t size_;

	static const size_t initSize = 64 * 1024;
};

class LogStream : noncopyable {
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

	friend LogStream& operator<<(LogStream& self,const StringView& v);


	BufferT& buffer() {
		return buffer_;
	};
	void append(const char*data, int len);
public:
	template<class T>
	void append(T value);
	BufferT buffer_;

	static const int MaxNumSize = 64;
};

class fmt{
	public:
		template<class T>
		fmt(const char *f,const T& data){
			size_ = snprintf(buf_,sizeof(buf_),f,data);
		};

		const char* data()const{
			return buf_;
		};

		int size() const {
			return size_;
		};
	private:
		char buf_[64];
		int size_;
};

inline LogStream& operator<< (LogStream& out,const fmt& f){
	out.append(f.data(),f.size());
	return out;
};

}


#endif
