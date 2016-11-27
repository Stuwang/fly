#include <base/AsyncLogger.h>

#include <iostream>

namespace fly {

static size_t FileSize(StringView filename)
{
	unsigned long filesize = 0;
	struct stat statbuff;
	if (stat(filename.data(), &statbuff) < 0) {
		return filesize;
	} else {
		filesize = statbuff.st_size;
	}
	return filesize;
}

// AppendFile
AppendFile::AppendFile(StringView name)
	: fileName_(name.toString())
	, fp_(::fopen(fileName_.c_str(), "a"))
	, fileSize_(::fly::FileSize(fileName_))
{
	assert(fp_);
	::setbuffer(fp_, buf_, sizeof buf_);
};

AppendFile::~AppendFile() {
	::fclose(fp_);
}

void AppendFile::Append(const char* data, const size_t size) {
	size_t len = fwrite(data, 1, size, fp_);
	fileSize_ += len;
	std::cout << len << " " << size << std::endl;
	assert(size == len );
};

void AppendFile::Flush() {
	::fflush(fp_);
};


// AsyncLogger
// static

std::shared_ptr<AsyncLogger>& GetAsyncLogger() {
	static std::shared_ptr<AsyncLogger> logger_;
	return logger_;
};

void AsyncLoggerWrite(const char* data, int len) {
	GetAsyncLogger()->Write(data, len);
};

void AsyncLoggerFlush() {
	GetAsyncLogger()->Flush();
};

void AsyncLogger::Init(StringView name,
                       int level,
                       size_t bufsize,
                       size_t filesize)
{
	GetAsyncLogger().reset(new AsyncLogger(name, bufsize, filesize));
	Logger::SetLevel(level);
	Logger::setOutputFun(AsyncLoggerWrite);
	Logger::setFlushFun(AsyncLoggerFlush);
	std::cout << "ok init ok" << std::endl;
};

// member function
AsyncLogger::AsyncLogger(StringView filename,
                         size_t bufsize,
                         size_t filesize)
	: bufSize_(bufsize)
	, fileSize_ (filesize)
	, fileName_(filename.toString())
{
	buf_.reserve(bufSize_ + 5 * 1024);
	RollFile();
	thread_.reset(new Thread(
	                  std::bind(&AsyncLogger::Run, this)
	              ));
};

AsyncLogger::~AsyncLogger() {
	stop_ = true;
	thread_->join();
	thread_.reset();
	file_.reset();
};

void AsyncLogger::Write(const char* data, size_t size) {
	LockGuard lock(mutex_);
	buf_.append(data, size);
	if (buf_.size() >= bufSize_ ) {
		rws_.push(std::move(buf_));
		std::string().swap(buf_);
		buf_.reserve(bufSize_ + 1024 * 5);
		cond_.Notify();
	}
};

void AsyncLogger::Flush() {
	LockGuard lock(mutex_);
	rws_.push(std::move(buf_));
	std::string().swap(buf_);
	buf_.reserve(bufSize_ + 1024 * 5);
	auto & s_ = rws_;
	while (!s_.empty()) {
		auto & ws = s_.front();
		if (!ws.empty()) {
			WriteFile(ws.data(), ws.size(), true);
			s_.pop();
		}
	}
	std::cout << "ok " << std::endl;
};

std::string AsyncLogger::GetTimeSuffix() {
	static const int64_t kMicroSecondsPerSecond = 1000 * 1000;
	time_ = LocalClock::Now();
	int64_t msenonds = time_.Ms() ;
	char buf[32] = {0};
	time_t seconds = static_cast<time_t>(msenonds / kMicroSecondsPerSecond);
	struct tm tm_time;
	gmtime_r(&seconds, &tm_time);
	int microseconds = static_cast<int>(msenonds % kMicroSecondsPerSecond);
	snprintf(buf, sizeof(buf), "%4d%02d%02d-%02d-%02d-%02d-%06d",
	         tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
	         tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
	         microseconds);
	return std::string(buf);
	return buf;
};

void AsyncLogger::Run() {
	stop_ = false;
	while (!stop_) {
		std::queue<std::string> s_;
		{
			LockGuard lock(mutex_);
			if (NextFile()) {
				RollFile();
			}
			if (rws_.empty()) {
				cond_.TimeWait(lock, MilliSeconds(3000));
				continue;
			} else {
				std::swap(s_, rws_);
			}
		}
		while (!s_.empty()) {
			auto & ws = s_.front();
			if (!ws.empty()) {
				WriteFile(ws.data(), ws.size(), false);
				s_.pop();
			}
		}
	}
	Flush();
};

bool AsyncLogger::NextFile() {
	return time_.ToDate().day_of_year() != LocalClock::ToDay().day_of_year();
};

void AsyncLogger::WriteFile(const char* data, size_t size, bool flush) {
	int leftLen = fileSize_ - file_->FileSize();
	if ( leftLen < size ) {
		file_->Append(data, leftLen);
		RollFile();
		WriteFile(data + leftLen, size - leftLen, false);
	} else {
		file_->Append(data, size);
	}
	if (flush) {
		file_->Flush();
	}
};

void AsyncLogger::RollFile() {
	fullFileName_ = fileName_ + GetTimeSuffix() + ".log";
	file_.reset(new AppendFile(fullFileName_));
};

}
