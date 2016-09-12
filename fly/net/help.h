#ifndef FLY_HELP_H__
#define FLY_HELP_H__

#include <csignal>
#include <functional>
#include <map>

namespace fly{

template <int Signal = SIGPIPE>
class Signal_ignore
{
public:
  // Constructor.
  Signal_ignore()
  {
    ::signal(Signal, SIG_IGN);
  }
};

class Signal{
public:
	typedef std::function<void()> SignalHandle;
	
	static void signal(int sig,const SignalHandle& handle); 

	static std::map<int,SignalHandle> handles;

	static void handle_helper(int sig);
};



}

#endif // FLY_HELP_H__
