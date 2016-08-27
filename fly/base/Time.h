#ifndef FLY_TIME_H__
#define FLY_TIME_H__

#include <string>
#include <sys/time.h>

#include <base/Date.h>

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

class TimeDuration {
public:
	long Ms()const {
		return data_ ;
	};

	OPERATOR_HELPER(TimeDuration, data_);
#undef OPERATOR_HELPER
private:
	long data_;
};

inline TimeDuration Hours(int value) {
	return TimeDuration(value * 3600 * 1000 * 1000);
};

inline TimeDuration Minutes(int value) {
	return TimeDuration(value * 60 * 1000 * 1000);
};

inline TimeDuration Seconds(int value) {
	return TimeDuration(value * 1000 * 1000);
};

inline TimeDuration MilliSeconds(int value) {
	return TimeDuration(value * 1000);
};

inline TimeDuration MicroSeconds(int value) {
	return TimeDuration(value);
};



class Time {
	static const int64_t kMicroSecondsPerSecond = 1000 * 1000;
public:
	Time() {
		data_.tv_sec = 0;
		data_.tv_usec = 0;
	};
	explicit Time(const struct timeval& time) {
		data_ = time;
	};

	explicit Time(long time) {
		data_.tv_sec = time / kMicroSecondsPerSecond;
		data_.tv_usec = time % kMicroSecondsPerSecond;
	}

	inline long Ms() const {
		int64_t msenonds = (  data_.tv_sec ) * kMicroSecondsPerSecond + data_.tv_usec ;
		return msenonds;
	};

	std::string ToString()const;

	friend bool operator==(const Time& left, const Time& right) {
		return left.Ms() == right.Ms();
	};

	friend bool operator!=(const Time& left, const Time& right) {
		return left.Ms() != right.Ms();
	};

	friend bool operator>(const Time& left, const Time& right) {
		return left.Ms() > right.Ms();
	};

	friend bool operator>=(const Time& left, const Time& right) {
		return left.Ms() >= right.Ms();
	};

	friend bool operator<(const Time& left, const Time& right) {
		return left.Ms() < right.Ms();
	};

	friend bool operator<=(const Time& left, const Time& right) {
		return left.Ms() <= right.Ms();
	};

	friend inline TimeDuration operator-(const Time& left, const Time& right) {
		return TimeDuration(left.Ms() - right.Ms());
	};


	friend Time operator+(const Time& time, const TimeDuration& r) {
		return Time(time.Ms() + r.Ms());
	};
	Time& operator+=(const TimeDuration& r) {
		*this = Time(Ms() + r.Ms());
		return *this;
	};

	friend Time operator-(const Time& time, const TimeDuration& r) {
		return Time(time.Ms() - r.Ms());
	};

	Time& operator-=(const TimeDuration& r) {
		*this = Time(Ms() - r.Ms());
		return *this;
	}
private:

	struct timeval data_;

};



inline TimeDuration AbsTimeDuration(const Time& left, const Time& right) {
	return left > right ? (left - right ) : (right - left );
};


class Clock {
public:
	static const int64_t kMicroSecondsPerSecond = 1000 * 1000;
	static Date ToDay();
	static Time Now();
};

template<int local>
class CityClock {
public:
	static const int64_t kMicroSecondsPerSecond = 1000 * 1000;
	static Date ToDay() {
		struct timeval tv;
		gettimeofday(&tv, NULL);
		int64_t msenonds = (  local * 60 * 60 + tv.tv_sec ) * kMicroSecondsPerSecond + tv.tv_usec ;

		time_t seconds = static_cast<time_t>(msenonds / kMicroSecondsPerSecond);
		struct tm tm_time;
		gmtime_r(&seconds, &tm_time);

		return Date(tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday);
	};

	static Time Now() {
		struct timeval tv;
		gettimeofday(&tv, NULL);
		tv.tv_sec += local * 3600;
		return Time(tv);
	};
};

static const int BEIJING = +8;

typedef CityClock<BEIJING> LocalClock;

}

#endif