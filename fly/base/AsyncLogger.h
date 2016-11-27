#ifndef FLY_ASYNCLOGGER_H__
#define FLY_ASYNCLOGGER_H__

#include <queue>
#include <string>
#include <memory>
#include <cstdio>
#include <atomic>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include <base/fly_base.h>

namespace fly
{

class AppendFile {
public:
	AppendFile(StringView name);
	~AppendFile();
	AppendFile(const AppendFile&) = delete;
	AppendFile& operator=(const AppendFile&) = delete;

	void Append(const char* data, const size_t size);
	void Flush();
	size_t FileSize() const { return fileSize_;};
private:
	std::string fileName_;
	FILE* fp_;
	size_t fileSize_;
	static const size_t bufSize = 64 * 1024;
	char buf_[bufSize];

};

class AsyncLogger {
public:
	AsyncLogger(StringView filename,
	            size_t bufsize,
	            size_t filesize);
	~AsyncLogger();

	static void Init(StringView name,
	                 int level = Logger::FLY_DEBUG,
	                 size_t bufsize = defaultBufSize,
	                 size_t filesize = defaultFileSize);

	AsyncLogger(const AsyncLogger&) = delete;
	AsyncLogger& operator=(const AsyncLogger&) = delete;

	void Write(const char*, size_t size);
	void Flush();
private:

	std::string GetTimeSuffix() ;
	void Run();
	bool NextFile();
	void RollFile();
	void WriteFile(const char* data, size_t size, bool flush) ;

	std::string buf_;
	std::queue<std::string> rws_;
	std::unique_ptr<Thread> thread_;
	std::unique_ptr<AppendFile> file_;
	Mutex mutex_;
	std::atomic<bool> stop_;
	Condition cond_;

	size_t bufSize_;
	size_t fileSize_;

	Time time_;
	const std::string fileName_;
	std::string fullFileName_;

	static const size_t defaultBufSize = 64 * 1024;
	static const size_t defaultFileSize = 16 * 1024 * 1024;

};
}

#endif //FLY_ASYNCLOGGER_H__
