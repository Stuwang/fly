#include <net/help.h>

namespace fly{

// signal_init<> _signal_init_;


void Signal::signal(int sig,const SignalHandle& handle){
	handles[sig] = handle;
	::signal(sig,&handle_helper);
}; 

std::map<int,std::function<void()>> Signal::handles;

void Signal::handle_helper(int sig){
	auto i = handles.find(sig);
	if(i!= handles.end()){
		i->second();
	}
};

}
