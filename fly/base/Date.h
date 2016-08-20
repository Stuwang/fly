#ifndef FLY_DATE_TH__
#define FLY_DATE_TH__

#include <string>
#include <cstdio>

#include <base/Logger.h>

namespace fly {

class DayOfWeek {
public:
	DayOfWeek(int value)
		: data_(value) {
		if (value <= 0 || value > 7) {
			LOG_ERROR << "week day error " ;
		}
	};
	std::string ToString()const {
		switch (data_) {
		case 1:
			return "Monday";
		case 2:
			return "Tuesday";
		case 3:
			return "Wednesday";
		case 4:
			return "Thursday";
		case 5:
			return "Friday";
		case 6:
			return "Saturday";
		case 7:
			return "Sunday";
		default:
			LOG_ERROR << "week day error ";
		}
		return "";
	};
private:
	int data_;
};

#define OPERATOR_HELPER(type,data)						\
type(long value):data_(value){}							\
type operator+(const type& right) {						\
		return type(data_ + right.data_);				\
};														\
type &operator+=(const type& right) {					\
	*this = type(data_ + right.data_);					\
	return *this;										\
};														\
type operator-(const type& right) {						\
	return type(data_ + right.data_);					\
};														\
type &operator-=(const type& right) {					\
	*this = type(data_ - right.data_);					\
	return *this;										\
};														\
type operator*(int right) {								\
	return type(data_ * right);							\
};														\
type &operator*=(int right) {							\
	*this = type(data_ * right);						\
	return *this;										\
};														\
type operator/(int right) {								\
	return type(data_ / right);							\
};														\
type &operator/=(int right) {							\
	*this = type(data_ / right);						\
	return *this;										\
};


class Days {
public:
	Days(): data_(0) {};
	OPERATOR_HELPER(Days, data_)

	long days()const {
		return data_;
	};
private:
	int data_;
};

class Weeks {
public:
	Weeks(): data_(0) {};
	long days()const {
		return 7 * data_;
	};
	OPERATOR_HELPER(Weeks, data_);
private:
	int data_;
};

class Months {
public:
	Months(): data_(0) {};
	OPERATOR_HELPER(Months, data_)
	long days()const {
		return 30 * data_;
	};
private:
	int data_;
};

class DateDuration {
public:
	DateDuration(): data_(0) {};
	explicit DateDuration(int value): data_(value) {};
	DateDuration(Months m): data_(m.days()) {};
	DateDuration(Days d): data_(d.days()) {};
	DateDuration(Weeks w): data_(w.days()) {};

	long days()const {
		return data_ > 0 ? data_ : -data_;
	};

	OPERATOR_HELPER(DateDuration, data_);

private:
	long data_;
};

#undef OPERATOR_HELPER


class Date {
	typedef unsigned char inner_type;
public:
	Date();
	Date(int year, inner_type month, inner_type day);

	int year() const;
	inner_type month() const;
	inner_type day() const;
	DayOfWeek day_of_week()const;
	int day_of_year() const;
	inner_type day_of_month() const;
	Date end_of_month()const;

	std::string ToString()const;

	bool operator== (const Date& right)const {
		return data_.year == right.data_.year &&
		       data_.month == right.data_.month &&
		       data_.day == right.data_.day;
	};

	bool operator!= (const Date& right) const {
		return !(*this == right);
	};

#define DATE_COMPARE(op)\
return data_.year < right.data_.year ||\
data_.year == right.data_.year && data_.month < right.data_.month ||\
data_.year == right.data_.year && data_.month == right.data_.month \
&& data_.day < right.data_.day;

	bool operator> (const Date& right)const {
		DATE_COMPARE( < );
	};

	bool operator>= (const Date& right)const {
		DATE_COMPARE( <= );
	};

	bool operator< (const Date& right)const {
		DATE_COMPARE( > );
	};

	bool operator<= (const Date& right) const {
		DATE_COMPARE( >= );
	};
#undef DATE_COMPARE

private:

	static bool IsLeap(int year);

	struct DataImpl {
		int year;
		// start with 0
		inner_type month;
		// start with 0
		inner_type day;
		DataImpl(int y, inner_type m, inner_type d)
			: year(y)
			, month(m)
			, day(d)
		{};

		DataImpl()
			: year(0)
			, month(0)
			, day(0)
		{};
	};

	static constexpr inner_type MonthDays[] =
	{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	static constexpr inner_type LeapMonthDays[] =
	{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	DataImpl data_;
};

DateDuration operator-(const Date& left, const Date& right);

inline DateDuration AbsDateDuration(const Date& left, const Date& right) {
	if (left > right) {
		return right - left;
	} else {
		return left - right;
	}
};


Date operator-(const Date& left, const DateDuration& right);
Date operator+(const Date& left, const DateDuration& right);


}

#endif