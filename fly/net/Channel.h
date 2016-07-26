#ifndef FLY_CHANNEL_H__
#define FLY_CHANNEL_H__

#include "Poller.h"

namespace fly {

class Poller;

class Channel {
public:
	inline int getfd(){
		return sockfd_;
	};
	inline void setRevents(int events){
		r_events_ = events;
	};
	inline int events(){
		return events_;
	};

	inline bool HasAdded(){
		return addedEvents_;
	}

	void update();
private:
	int events_;
	int r_events_;
	Poller * poller_;
	int sockfd_;

	bool addedEvents_;
};

}

#endif