#ifndef GAP_PROCESS_H
#define GAP_PROCESS_H

#include <map>
#include <vector>
#include <functional>
#include <signal.h>

namespace Gap {

class Process
{
  public:
    enum Event {
      ReadyRead,
      ReadyWrite,
      Error
    };

    using Callback = std::function<void(int, Event)>;
    
    Process();
    virtual ~Process();
    void RegisterEvent(int fd, Callback cb);
    void DeregisterEvent(int fd);
    void Run();
    void Terminate();
    virtual void onTerminate(){}
  
  private:
    bool m_run {true};
    int m_epollFd {-1};
    std::vector<struct epoll_event> m_epollEvents;
    std::map<int,Callback> m_epollCallback;
    std::vector<int> m_exitSignals {SIGINT, SIGQUIT};
    int m_exitSignalFd {-1};
    pthread_t m_threadId {0};

    void onTerminate(int fd, Event event);

    
};

}

#endif