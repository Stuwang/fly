#ifndef _FLY_SOCKETOPS_H__
#define _FLY_SOCKETOPS_H__

#include <cstdlib>

namespace fly{

namespace socketops{

void setNoblockAndCloseOnExec(int socket);
void setNoDelay(int socket);
void setReuseAddr(int socket);
void setReusePort(int socket);
void setKeepAlive(int socket);
void setBufSize(int socket,size_t bufsize);

}

}

#endif