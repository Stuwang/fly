#ifndef FLY_STRINGVIEW_H__
#define FLY_STRINGVIEW_H__

#include <cstring>
#include <string>
#include <ostream>

namespace fly {

class StringView {
public:
	StringView(): data_(NULL), len_(0) {};
	StringView(const char*str)
		: data_(str), len_(strlen(str) ) {};
	StringView(const char*str, size_t size)
		: data_(str), len_(size) {};
	StringView(const unsigned char* str)
		: data_(reinterpret_cast<const char*>(str))
		, len_(static_cast <int>(::strlen(data_)) ) {};
	StringView(const std::string & str )
		: data_(str.data()), len_(str.length()) {};

	const char* data()const {return data_;};
	int size()const {return len_;};
	bool empty()const {return 0 == len_;};
	const char* begin()const {return data_;};
	const char* end()const {return data_ + len_;};
	void clear() {data_ = NULL, len_ = 0;};
	void set(const char* buf, int len) {
		data_ = buf;
		len_ = len;
	};
	void set(const char* str) {
		data_ = str;
		len_ = static_cast<int>(strlen(str));
	}
	void set(const void* buffer, int len) {
		data_ = reinterpret_cast<const char*>(buffer);
		len_ = len;
	}

	char operator[](int n)const {return data_[n];};
	void remove_front(int n) {data_ += n; len_ -= n;};
	void remove_back(int n) {len_ -= n;};

	bool operator ==(const StringView& r) const {
		return len_ == r.len_ && memcmp(data_, r.data_, len_);
	};
	bool operator <(const StringView& r) const {return cmp(r) < 0;};

	bool operator !=(const StringView& r) const {return !(*this == r);};
	bool operator <=(const StringView& r) const {return cmp(r) <= 0;};
	bool operator >=(const StringView& r) const {return cmp(r) >= 0;;};
	bool operator >(const StringView& r) const {return cmp(r) > 0;};

	int cmp(const StringView& r)const {
		int ret = memcmp(data_, r.data_, len_ > r.len_ ? r.len_ : len_);
		if (ret == 0 ) {
			if (r.len_ == len_) {
				return 0;
			} else {
				return len_ > r.len_ ? 1 : -1;
			}
		}
		return ret;
	};
	std::string toString()const {return std::string(data_, len_);};
	void copyToString(std::string *str) const
	{str->append(data_, static_cast<size_t>(len_));};
	bool startWith(const StringView& v)const {
		return len_ >= v.len_ && memcmp(data_, v.data_, v.len_);
	};

	friend std::ostream& operator << (std::ostream& out, const StringView& v) {
		out.write(v.data_, v.len_);
		return out;
	};

private:
	const char *data_;
	int len_;
};



}

#endif