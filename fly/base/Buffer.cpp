#include <base/Buffer.h>

namespace fly {

Buffer::Buffer(size_t size )
	: buf_(size)
	, ReadIndex_(0)
	, WriteIndex_(0) {

};

void Buffer::swap(Buffer& buf) {
	buf_.swap(buf.buf_);
	std::swap(ReadIndex_, buf.ReadIndex_);
	std::swap(WriteIndex_, buf.WriteIndex_);
};

size_t Buffer::Read(char *dest, size_t len) {
	size_t s = len > (WriteIndex_ - ReadIndex_) ? (WriteIndex_ - ReadIndex_) : len;
	memcpy(dest, data(), s);
	ReadIndex_ += s;
	return s;
};

size_t Buffer::Read(void *dest, size_t len) {
	return Read(static_cast<char*>(dest), len);
};

size_t Buffer::ReadAbleBytes() const {
	return WriteIndex_ - ReadIndex_;
};

StringView Buffer::ToStringView() {
	return StringView(data(), ReadAbleBytes());
};


size_t Buffer::WriteAbleBytes()const {
	return buf_.size() - WriteIndex_;
};

void Buffer::append(const char* data, size_t len) {
	append(static_cast<const void*>(data), len);
};

void Buffer::append(const void* data, size_t len) {
	retireWriteAble(len);
	memcpy(Peek(), data, len);
	retireWrite(len);
};

char* Buffer::data() {
	return buf_.data() + ReadIndex_;
};

char* Buffer::Peek() {
	return buf_.data() + WriteIndex_;
};

void Buffer::retireRead(size_t n) {
	ReadIndex_ += n;
};

void Buffer::retireWrite(size_t n) {
	WriteIndex_ += n;
};

void Buffer::retireWriteAble(size_t n) {
	if (n <= WriteAbleBytes()) {
		return;
	} else {
		clearReaded();
		buf_.resize(WriteIndex_ + n);
	}
};

void Buffer::clearReaded() {
	if (ReadIndex_ >= buf_.size() / 2) {
		std::vector<char> v(buf_.size());
		memcpy(&*v.begin(), &*data(), WriteIndex_ - ReadIndex_);
		WriteIndex_ = WriteIndex_ - ReadIndex_;
		ReadIndex_ = 0;
		buf_.swap(v);
	}
};

}