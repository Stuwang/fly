#include <net/TcpConnection.h>

namespace fly {

TcpConnection::TcpConnection(EventLoop* loop,
                             int fd, const std::string& name)
	: loop_(loop)
	, sockfd_(fd)
	, name_(name)
{
	poller_ = loop->getPoller();
	chan_ = new Channel(sockfd_, poller_);
	localAddr_ = socketops::getLocalAddr(sockfd_);
	peerAddr_ = socketops::getPeerAddr(sockfd_);
	socketops::setNoblockAndCloseOnExec(sockfd_);
	socketops::setNoDelay(sockfd_, true);
	socketops::setKeepAlive(sockfd_, true);
};

TcpConnection::~TcpConnection() {

};

EventLoop *TcpConnection::getEventLoop() {
	return loop_;
};

const std::string & TcpConnection::name() const {
	return name_;
};

const struct sockaddr_in& TcpConnection::localAddress() const {
	return localAddr_;
};

const struct sockaddr_in& TcpConnection::peerAddress() const {
	return peerAddr_;
};

bool TcpConnection::connected() const {
	return true;
};

void TcpConnection::Send(const void* data, size_t len) {

};

void TcpConnection::Send(const StringView& data) {

};

void TcpConnection::Send(const Buffer& data) {

};

void TcpConnection::startRead() {

};

void TcpConnection::stopRead() {

};

bool TcpConnection::isReading()const {
	return true;
};

void TcpConnection::startListenWrite() {

};

void TcpConnection::endListenWrite() {

};

Buffer* TcpConnection::readBuffer() {
	return &inputBuffer_;
};

Buffer* TcpConnection::writeBuffer() {
	return &outputBuffer_;
};

void TcpConnection::SendInLoop(const void* data, size_t len) {

};

void TcpConnection::SendInLoop(StringView data) {
	SendInLoop(data.data(), data.size());
};

void TcpConnection::handRead() {

};

void TcpConnection::handWrite() {

};

void TcpConnection::handClose() {

};

void TcpConnection::handError() {

};

}