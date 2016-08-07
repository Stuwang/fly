#ifndef FLY_BUGGER_H__
#define FLY_BUGGER_H__

#include <vector>

#include <base/Types.h>
#include <base/StringView.h>
#include <base/Endian.h>

namespace fly {

class Buffer : noncopyable {
public:

	static const size_t initbufSize = 1024;

	explicit Buffer(size_t size = initbufSize);
	void swap(Buffer& buf);
	size_t Read(char *, size_t len);
	size_t Read(void *, size_t len);
	size_t ReadAbleBytes() const ;
	StringView ToStringView();

	size_t WriteAbleBytes()const ;
	void append(const char* data, size_t len);
	void append(const void* data, size_t len);

	void retireRead(size_t );
	void retireWriteAble(size_t n);

	template<class T>
	void Append(T value) {
		T value_ = HostToNet(value);
		append(&value, sizeof(T));
	};

	template<class T>
	T Get() {
		T value = NetToHost<T>(*(data()));
		retireRead(sizeof(T) );
		return value;
	};
private:
	void retireWrite(size_t n);
	char* data();
	char* Peek();
	void clearReaded();

	std::vector<char> buf_;
	size_t ReadIndex_;
	size_t WriteIndex_;
};

}

#endif