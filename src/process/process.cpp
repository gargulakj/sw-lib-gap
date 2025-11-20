#include <stdexcept>
#include <system_error>
#include <errno.h>
#include <iostream>
#include <sys/signalfd.h>
#include <sys/epoll.h>
#include "process.h"
#include "log.h"

namespace Gap {

Process::Process()
{
  // Store thread ID
  m_threadId = pthread_self();

  // Fill structure with signals which are handle by FD.
  sigset_t signals;
  sigemptyset(&signals);
  for(int sig : m_exitSignals)
  {
    sigaddset(&signals, sig);
  }

  // Block signals which is handle by FD
  if (sigprocmask(SIG_BLOCK, &signals, NULL) == -1)
  {
    throw std::system_error({errno, std::generic_category()}, "Cannot block signals handled by FD!");
  }

  // Create FD for signal handling.
  m_exitSignalFd = signalfd(-1, &signals, SFD_NONBLOCK);
  if (m_exitSignalFd == -1)
  {
    throw std::system_error({errno, std::generic_category()}, "Cannot create FD for signal handling!");
  }
 
  // Epoll create
  m_epollFd = epoll_create1(0);
  if (m_epollFd == -1)
  {
    throw std::system_error({errno, std::generic_category()}, "Cannot create epoll instance!");
  }

  // Register exit signal
  RegisterEvent(m_exitSignalFd, [this](int fd, Event evn){ onTerminate(fd, evn); });
}

Process::~Process()
{
  if(m_epollFd != -1)
  {
    close(m_epollFd);
  }
  if(m_exitSignalFd != -1)
  {
    close(m_exitSignalFd);
  }
}

void Process::onTerminate(int fd, Event event)
{
  m_run = false;
  onTerminate();
}

void Process::RegisterEvent(int fd, Callback cb)
{
  if(!m_epollCallback.contains(fd))
  {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
    ev.data.fd = fd;

    // Register epol event
    if (epoll_ctl(m_epollFd, EPOLL_CTL_ADD, fd, &ev) == -1)
    {
      throw std::system_error({errno, std::generic_category()}, "Failed to register file descriptor!");
    }
    // Emplace next structure for getting event.
    m_epollEvents.emplace_back(epoll_event{});
    // Store callback function
    m_epollCallback[fd] = cb;

    log.Info("Process::RegisterEvent for fd: " + std::to_string(fd));
  }
  else
  {
    throw std::runtime_error("File descriptor is already registered!");
  }
}

void Process::DeregisterEvent(int fd)
{
  // Deregister epol event.
  if (epoll_ctl(m_epollFd, EPOLL_CTL_DEL, fd, nullptr) == -1)
  {
    throw std::system_error({errno, std::generic_category()}, "Failed to deregister file descriptor!");
  }
  // Remove callback funtion
  m_epollCallback.erase(fd);
  // Remove structure for getting event.
  m_epollEvents.pop_back();  
}

void Process::Run()
{
  log.Info("Application started");
  int eventCount = 0;

  while(m_run)
  {
    // Wait for event. Event are stored to vector m_epollEvents.
    int eventCount = epoll_wait(m_epollFd, &m_epollEvents[0], m_epollEvents.size(), -1);
    
    log.Info("EPOLL events count: " + std::to_string(eventCount));

    // Check all events.
    for(int eventIt = 0; eventIt < eventCount; eventIt++)
    {
      struct epoll_event& event = m_epollEvents[eventIt];

      // Get stored data
      int fd = event.data.fd;

      log.Info("EPOLL event fd: " + std::to_string(fd));
      // Callback is there?
      if(m_epollCallback.contains(fd))
      {
        // Error
        if((event.events & (EPOLLHUP | EPOLLERR)) > 0)
        {
          m_epollCallback[fd](fd, Event::Error);
        }        
        // Ready to read
        if((event.events & EPOLLIN) == EPOLLIN)
        {
          m_epollCallback[fd](fd, Event::ReadyRead);
        }
        // Ready to write
        if((event.events & EPOLLOUT) == EPOLLOUT)
        {
          m_epollCallback[fd](fd, Event::ReadyWrite);
        }
      }      
    }
    
    // Epoll error
    if(eventCount == -1)
    {
      throw std::system_error({errno, std::generic_category()}, "epoll_wait error");
    }
  }
  
  log.Info("Application finished");
}

void Process::Terminate()
{
  log.Info("Application terminated");
  pthread_kill(m_threadId, SIGINT);
}

}