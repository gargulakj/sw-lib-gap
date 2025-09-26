#ifndef GAP_PROCESS_H
#define GAP_PROCESS_H

#include <sys/epoll.h>

namespace Gap {

class Process
{
  public:
    Process();
  
  private:
    int m_epollFd {-1};
};

}

#endif