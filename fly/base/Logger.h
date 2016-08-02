#ifndef FLY_LOGGER_H__
#define FLY_LOGGER_H__

#include <base/Types.h>
#include <base/LogStream.h>

namespace fly {

typedef void (*OutputFunc)(const char* msg, int len);
typedef void (*FlushFunc)();

class Logger : noncopyable {
public:
	static const int TRACE = 1;
	static const int DEBUG = 2;
	static const int INFO = 3;
	static const int WARNING = 4;
	static const int ERROR = 5;
	static const int FATAL = 6;

	static int defaultLevel;

	static void SetLevel(int level) {
		defaultLevel = level;
	};

	static OutputFunc g_output;
	static FlushFunc g_flush;

	static void setOutputFun(OutputFunc func) {
		g_output = func;
	};

	static void setFlushFun(FlushFunc func) {
		g_flush = func;
	};

	static const char* LogLevelName(int level) {
		switch (level) {
		case TRACE:
			return "TRACE   ";
		case DEBUG:
			return "DEBUG   ";
		case INFO:
			return "INFO    ";
		case WARNING:
			return "WARNING ";
		case ERROR:
			return "ERROR   ";
		case FATAL:
			return "FATAL   ";
		default:
			assert(0);
		}
		return "UNKNOW";
	};

	Logger(const char* file, int line, int level);
	~Logger();
	void FmtTime();

	LogStream& Stream() {
		return stream_;
	};
	// LogStream::BufferT & buffer() {
	// 	return stream_.buffer_;
	// };
private:
	int level_;
	LogStream stream_;

	static const int64_t kMicroSecondsPerSecond = 1000 * 1000;
};

#define LOG_TRACE fly::Logger(__FILE__,__LINE__,fly::Logger::TRACE).Stream()
#define LOG_DEBUG fly::Logger(__FILE__,__LINE__,fly::Logger::DEBUG).Stream()
#define LOG_INFO fly::Logger(__FILE__,__LINE__,fly::Logger::INFO).Stream()
#define LOG_ERROR fly::Logger(__FILE__,__LINE__,fly::Logger::ERROR).Stream()
#define LOG_FATAL fly::Logger(__FILE__,__LINE__,fly::Logger::FATAL).Stream()

}

#endif