#ifndef FLY_HELP_H__
#define FLY_HELP_H__

#include <csignal>

namespace fly{

template <int Signal = SIGPIPE>
class signal_init
{
public:
  // Constructor.
  signal_init()
  {
    std::signal(Signal, SIG_IGN);
  }
};

}

#endif // FLY_HELP_H__
