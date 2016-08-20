#include <base/Date.h>

namespace fly {

typedef unsigned char inner_type;

Date::Date()
	: data_()
{};

Date::Date(int year, inner_type month, inner_type day)
	: data_(year, month, day)
{};

int Date::year() const {
	return data_.year;
};

inner_type Date::month() const {
	return data_.month;
};

inner_type Date::day() const {
	return data_.year;
};

DayOfWeek Date::day_of_week()const {
	return DayOfWeek(1);
};

int Date::day_of_year() const {
	int sum = 0;
	if (IsLeap(data_.year)) {
		for (int i = 0; i < data_.month; i++) {
			sum += LeapMonthDays[i];
		}
	} else {
		for (int i = 0; i < data_.month; i++) {
			sum += MonthDays[i];
		}
	};
	sum = sum + data_.day + 1 ;
	return sum;
};

inner_type Date::day_of_month() const {
	return data_.day + 1;
};

Date Date::end_of_month()const {
	inner_type day;
	if (IsLeap(data_.year)) {
		day = LeapMonthDays[data_.month];
	} else {
		day = MonthDays[data_.month];
	}
	return Date(data_.year, data_.month, day - 1);
};


std::string Date::ToString()const {
	char buf[64];
	snprintf(buf, sizeof(buf), "%4d-%2d-%2d", data_.year, data_.month, data_.day);
	return std::string(buf);
};

bool Date::IsLeap(int year) {
	if ( (0 == year % 4 && year % 100 != 0 ) ||
	        (year % 400 == 0  )) {
		return true;
	}
	return false;
};

// DateDuration operator-(const Date& left, const Date& right) {

// };

// Date operator-(const Date& left, const DateDuration& right) {

// };

// Date operator+(const Date& left, const DateDuration& right) {

// };

};