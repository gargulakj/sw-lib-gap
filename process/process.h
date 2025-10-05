#ifndef GAP_PROCESS_H
#define GAP_PROCESS_H

#include <sys/epoll.h>
#include <map>
#include <vector>
#include <functional>

namespace Gap {

class Process
{
  public:
    using Callback = std::function<void(int)>;
    
    Process();
    void RegisterEvent(int fd, Callback cb);
    void DeregisterEvent(int fd);
    void Run();
    
  
  private:
    int m_epollFd {-1};
    std::vector<struct epoll_event> m_epollEvents;
    std::map<int,Callback> m_epollCallback;
    
};

}

#endif