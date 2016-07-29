#include "Channel.h"

namespace fly{

void Channel::update(){
	poller_->updateChannel(this);
	addedEvents_ = true;
};

void Channel::remove(){
	poller_->removeChannel(this);
	addedEvents_ = false;
};

void Channel::handleEvents(){
	if(r_events_ & EPOLLIN){
		
	}
};

}