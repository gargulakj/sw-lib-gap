#ifndef GAP_PROCESS_H
#define GAP_PROCESS_H

#include <sys/epoll.h>
#include <vector>

namespace Gap {

class Process
{
  public:
    Process();
    void RegisterEvent(int fd);
    void DeregisterEvent(int fd);
    void Run();
  
  private:
    int m_epollFd {-1};
    std::vector<struct epoll_event> m_epollEvents;
};

}

#endif