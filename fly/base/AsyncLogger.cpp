#include <base/AsyncLogger.h>

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
	: fileName_(name.ToString())
	, fp_(::fopen(fileName_.c_str(),"a"))
	, fileSize_(FileSize(fileName_.c_str()))
{
	assert(fp_);
	::setbuffer(fp_,buf_,sizeof buf_);
};

AppendFile::~AppendFile(){
	::fclose(fp_);
}

void AppendFile::Append(const char* data,const size_t size){
	size_t len = fwrite(data,size,1,fp_);
	assert(size == len );
};

void AppendFile::Flush(){
	::fflush(fp_);
};


// AsyncLogger
// static
static void AsyncLogger::Init(StringView name,
                              size_t bufsize,
                              size_t filesize)
{

};

// member function
AsyncLogger::AsyncLogger(StringView filename,
		size_t bufsize,
		size_t filesize)
	: fileName_(filename)
	, bufSize_(bufsize)
	, fileSize_ (filesize)
{
	buf_.reserve(bufSize_ + 5* 1024);
	RollFile();
	thread_.reset(new Thread(
				std::bind(&AsyncLogger::Run,this)
			));
};

AsyncLogger::～AsyncLogger() {
	stop_ = true;
	thread_->join();
	Flush();
};

void AsyncLogger::Write(const char* data, size_t size) {
	LockGuard lock(mutex_);
	buf_.append(data, size);
	if (buf_.size() >= bufSize_ ) {
		rws_.push_back(std::move(rws_));
		std::string().swap(rws_);
		buf_.reserve(bufSize_ + 1024 * 5);
		cond_.Notify();
	}
};

void AsyncLogger::Flush() {
	LockGuard lock(mutex_);
	auto & s_ = rws_;
	while (!s_.empty()) {
		auto & ws = s_.front();
		if (!ws.empty()) {
			WriteFile(ws.data(),ws.size());
			s_.pop_front();
		}
	}

};

std::string AsyncLogger::GetTimeSuffix() const {
	return LocalClock::Now().ToString();
};

void AsyncLogger: Run() {
	stop_ = false;
	while (!stop_) {
		std::queue<std::string> s_;
		{
			LockGuard lock(mutex_);
			if (NextFile()) {
				RollFile();
			}
			if(rws_.empty()){
				cond_.TimeWait(lock,MilliSeconds(3000));
				continue;
			}else{
				std::swap(s_,rws_);	
			}
		}
		while (!s_.empty()) {
			auto & ws = s_.front();
			if (!ws.empty()) {
				WriteFile(ws.data(),ws.size());
				s_.pop_front();
			}
		}
	}
	Flush();
};

bool AsyncLogger::NextFile() {
	return time_.ToDate().day_of_year() != CityClock::ToDay().day_of_year();
};

void AsyncLogger::WriteFile(const char* data, size_t size, bool flush){
int leftLen = fileSize_ - file_->FileSize();
	if( leftLen < size ){
		file_->Append(data,leftLen);
		RollFile();
		WriteFile(data + leftLen,size - leftLen);
	}else{
		file_->Append(data,size);	
	}
};
void AppendFile::RollFile(){
	fullFileName_ = fileName_ + GetTimeSuffix() + ".log";
	file_.reset(new AppendFile(fullFileName_));
};

}
