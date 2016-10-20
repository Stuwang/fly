#ifndef FLY_CHANNEL_H__
#define FLY_CHANNEL_H__

#include <functional>

#include <net/Poller.h>
#include <net/SocketOps.h>
#include <base/Types.h>
#include <base/Logger.h>

namespace fly {

class Poller;

typedef std::function<void()> EventCallBack;

class Channel : noncopyable {
public:

	Channel(Poller* poller, int sockfd);

	~Channel();

	void setReadCallBack(const EventCallBack& b) {
		readCallBack_ = b;
	};

	void setWriteCallBack(const EventCallBack& b) {
		writeCallBack_ = b;
	};

	void setCloseCallBack(const EventCallBack& b) {
		closeCallBack_ = b;
	};

	void setErrorCallBack(const EventCallBack& b) {
		errorCallBack_ = b;
	}

	void enableRead() {
		events_ |= EPOLLIN;
		update();
	};
	void disableRead() {
		events_ &= ~EPOLLIN;
		update();
	};

	bool isReading() {
		return events_ & EPOLLIN;
	};

	void enableWrite() {
		events_ |= EPOLLOUT;
		update();
	};
	void disableWrite() {
		events_ &= ~EPOLLOUT;
		update();
	};

	bool isWriting() {
		return events_ & EPOLLOUT;
	};

	void disableAll() {
		events_ = 0;
		update();
	};

	inline int getfd() {
		return sockfd_;
	};
	inline void setRevents(int events) {
		r_events_ = events;
	};
	inline int events() {
		return events_;
	};
	inline bool HasAdded() {
		return addedEvents_;
	}

	void update();
	void remove();

	void handleEvents();
	std::string EventToString(int event);

private:
	Poller * poller_;
	int sockfd_;
	bool addedEvents_;
	int events_;
	int r_events_;

	EventCallBack readCallBack_;
	EventCallBack writeCallBack_;
	EventCallBack closeCallBack_;
	EventCallBack errorCallBack_;
};

}

#endif