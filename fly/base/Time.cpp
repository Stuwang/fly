#include <base/Time.h>


namespace fly {

std::string Time::ToString() const {
	int64_t msenonds = (  data_.tv_sec ) * kMicroSecondsPerSecond + data_.tv_usec ;
	char buf[32] = {0};
	time_t seconds = static_cast<time_t>(msenonds / kMicroSecondsPerSecond);
	struct tm tm_time;
	gmtime_r(&seconds, &tm_time);
	int microseconds = static_cast<int>(msenonds % kMicroSecondsPerSecond);
	snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
	         tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
	         tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
	         microseconds);
	return std::string(buf);
};

Date Time::ToDate() const {
	auto& tv = data_;
	int64_t msenonds = ( tv.tv_sec ) * kMicroSecondsPerSecond + tv.tv_usec ;
	time_t seconds = static_cast<time_t>(msenonds / kMicroSecondsPerSecond);
	struct tm tm_time;
	gmtime_r(&seconds, &tm_time);

	return Date(tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday);
};

Date Clock::ToDay() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int64_t msenonds = ( tv.tv_sec ) * kMicroSecondsPerSecond + tv.tv_usec ;
	time_t seconds = static_cast<time_t>(msenonds / kMicroSecondsPerSecond);
	struct tm tm_time;
	gmtime_r(&seconds, &tm_time);

	return Date(tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday);
};

Time Clock::Now() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	Time t(tv);
	return t;
};

}
