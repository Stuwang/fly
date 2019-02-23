#include <net/Connecter.h>

namespace fly {

Connecter::Connecter(EventLoop* loop)
	: loop_(loop)
	, seted_addr_(false)
	, connecting_(false)
	, fd_(0)
{

};

Connecter::Connecter(EventLoop* loop, const sockaddr& addr)
	: loop_(loop)
	, addr_(addr)
	, seted_addr_(true)
	, connecting_(false)
	, fd_(0)
{

};

void Connecter::SetAddr(const NetAddr& addr) {
	assert(!IsConnecting());
	addr_ = addr;
};

void Connecter::Reset() {
	assert(!IsConnecting());
	addr_.reset();
	seted_addr_ = false;
	connecting_ = false;
	fd_ = 0;
	chan_.reset();
};

void Connecter::SetConnectedCallBack(const Functor& f) {
	cb_ = f;
};

void Connecter::Start() {
	assert(!IsConnecting());
	assert(seted_addr_);
	assert(!chan_);
	connecting_ = true;
	fd_ = socketops::creatNoBlockOrDie();
	socketops::connect(fd_, addr_);
	chan_.reset(new Channel(loop_->getPoller(),fd_));
	chan_->setWriteCallBack(std::bind(&Connecter::HandleWrite,
	                                  shared_from_this()));
	chan_->setErrorCallBack(std::bind(&Connecter::HandleError,
	                                  std::shared_from_this()));
	chan_->enableWrite();
};

void Connecter::Stop() {
	Reset();
};

void Connecter::HandleWrite() {
	chan_->disableWrite();
	if (cb_) cb_(fd_);
	Stop();
};

void Connecter::HandleError() {

};

}
