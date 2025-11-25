#ifndef GAP_PROCESS_H
#define GAP_PROCESS_H

#include <map>
#include <vector>
#include <functional>
#include <signal.h>
#include "event_provider.h"
#include "signal_handler.h"

namespace Gap {

class Process
{
  public:
    
    Process();
    virtual ~Process();

    void Run();
    void Terminate();
    void SetEventProvider(EventProvider* provider);
  
  private:
    void UnsetEventProvider();
    void RegisterEvent(int fd);
    void DeregisterEvent(int fd);

    EventProvider* m_eventProvider;
    SignalHandler m_exitSignal {{SIGQUIT}};

    pthread_t m_threadId {0};
    bool m_run {true};
    int m_epollFd {-1};
    std::vector<struct epoll_event> m_epollEvents;
    
};

}

#endif