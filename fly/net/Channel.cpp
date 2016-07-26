#include "Channel.h"

namespace fly{

void Channel::update(){
	poller_->updateChannel(this);
	addedEvents_ = true;
};

}