#ifndef FLY_TIME_H__
#define FLY_TIME_H__

namespace fly {

// min is ms

#define OPERATOR_HELPER(type,data)						\
type():data_(0){};										\
type(long value):data_(value){}							\
type operator+(const type& right)const {				\
		return type(data_ + right.data_);				\
};														\
type &operator+=(const type& right) {					\
	*this = type(data_ + right.data_);					\
	return *this;										\
};														\
type operator-(const type& right)const {				\
	return type(data_ + right.data_);					\
};														\
type &operator-=(const type& right) {					\
	*this = type(data_ - right.data_);					\
	return *this;										\
};														\
type operator*(int right) const{						\
	return type(data_ * right);							\
};														\
type &operator*=(int right) {							\
	*this = type(data_ * right);						\
	return *this;										\
};														\
type operator/(int right) const{						\
	return type(data_ / right);							\
};														\
type &operator/=(int right) {							\
	*this = type(data_ / right);						\
	return *this;										\
};

class Hours {
public:
	OPERATOR_HELPER(Hours, data_)
	long Ms()const {
		return data_ * 3600 * 1000 * 1000;
	};
private:
	long data_;
};

class Minutes {
public:
	OPERATOR_HELPER(Minutes, data_);
	long Ms()const {
		return data_ * 60 * 1000 * 1000;
	};
private:
	long data_;
};

class Seconds {
public:
	OPERATOR_HELPER(Seconds, data_);
	long Ms()const {
		return data_ * 1000 * 1000;
	};
private:
	long data_;
};

class MilliSeconds {
public:
	OPERATOR_HELPER(MilliSeconds, data_);
	long Ms()const {
		return data_ * 1000;
	};
private:
	long data_;
};

class MicroSeconds {
public:
	OPERATOR_HELPER(MicroSeconds, data_);
	long Ms()const {
		return data_ ;
	};
private:
	long data_;
};



class TimeDuration {
public:
	long Ms()const {
		return data_ ;
	};
	TimeDuration(const Hours& h): data_(h.Ms()) {};
	TimeDuration(const Minutes& h): data_(h.Ms()) {};
	TimeDuration(const Seconds& h): data_(h.Ms()) {};
	TimeDuration(const MilliSeconds& h): data_(h.Ms()) {};
	TimeDuration(const MicroSeconds& h): data_(h.Ms()) {};

	OPERATOR_HELPER(TimeDuration, data_);
private:
	long data_;
};

#undef OPERATOR_HELPER

class Time {
public:

private:

};

}

#endif