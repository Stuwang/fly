#ifndef FLY_CHANNEL_H__
#define FLY_CHANNEL_H__

#include "Poller.h"

namespace fly {

class Poller;

typedef std::function<void()> EventCallBack;

class Channel {
public:

	void setReadCallBack(const EventCallBack& b){
		readCallBack_ = b;
	};

	void setWriteCallBack(const EventCallBack& b){
		writeCallBack_ = b;
	};

	void setCloseCallBack(const EventCallBack& b){
		closeCallBack_ = b;
	};

	void setErrorCallBack(const EventCallBack& b){
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

	void enableWrite() {
		events_ |= EPOLLOUT;
		update();
	};
	void disableWrite() {
		events_ &= ~EPOLLOUT;
		update();
	};

	void disableAll(){
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

	inline void SetAdded(bool added) {
		addedEvents_ = added;
	}

	void update();
	void remove();

	void handleEvents();
private:
	int events_;
	int r_events_;
	Poller * poller_;
	int sockfd_;

	bool addedEvents_;

	EventCallBack readCallBack_;
	EventCallBack writeCallBack_;
	EventCallBack closeCallBack_;
	EventCallBack errorCallBack_;
};

}

#endif