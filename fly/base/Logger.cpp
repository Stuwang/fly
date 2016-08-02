#include <stdlib.h>
#include <sys/time.h>

#include <base/Logger.h>
#include <base/Atomic.h>

namespace fly {

void defaultOutput(const char* msg, int len)
{
	size_t n = fwrite(msg, 1, len, stdout);
	//FIXME check n
	(void)n;
}

void defaultFlush()
{
	fflush(stdout);
}

OutputFunc Logger::g_output = defaultOutput;
FlushFunc Logger::g_flush = defaultFlush;
int Logger::defaultLevel = 0;

Logger::Logger(const char* file, int line, int level)
	: level_(level)
	, stream_()
{
	FmtTime();
	stream_ << ":" << file << ":" << line << ":" << LogLevelName(level);
};

Logger::~Logger() {
	if (level_ < defaultLevel) {
		return;
	};

	// const LogStream::BufferT buf = buffer();

	g_output(buffer().data(), buffer().length());

	if (level_ == FATAL) {
		g_flush();
		abort();
	}
};

void Logger::FmtTime() {

	struct timeval tv;
	gettimeofday(&tv, NULL);
	int64_t msenonds = (  8 * 60 * 60 + tv.tv_sec ) * kMicroSecondsPerSecond + tv.tv_usec ;

	char buf[32] = {0};
	time_t seconds = static_cast<time_t>(msenonds / kMicroSecondsPerSecond);
	struct tm tm_time;
	gmtime_r(&seconds, &tm_time);

	int microseconds = static_cast<int>(msenonds % kMicroSecondsPerSecond);
	snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
	         tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
	         tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
	         microseconds);

	stream_.append(buf, 24);
};

}