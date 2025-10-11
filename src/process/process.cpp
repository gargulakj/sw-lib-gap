#include "process.h"
#include <stdexcept>
#include <system_error>
#include <errno.h>
#include <iostream>

namespace Gap {

Process::Process()
{
  m_epollFd = epoll_create1(0);
  if (m_epollFd == -1)
  {
    throw std::runtime_error("Cannot create epoll instance!");
  }
}

void Process::RegisterEvent(int fd, Callback cb)
{
  if(!m_epollCallback.contains(fd))
  {
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
    ev.data.fd = fd;
    
    if (epoll_ctl(m_epollFd, EPOLL_CTL_ADD, fd, &ev) == -1)
    {
      throw std::system_error({errno, std::generic_category()}, "Failed to register file descriptor!");
    }  
    m_epollEvents.emplace_back(epoll_event{});
    m_epollCallback[fd] = cb;

    std::cout << "Process::RegisterEvent - done FD: " << fd << "\n";
  }
  else
  {
    throw std::runtime_error("File descriptor is already registered!");
  }
}

void Process::DeregisterEvent(int fd)
{
  if (epoll_ctl(m_epollFd, EPOLL_CTL_DEL, fd, nullptr) == -1)
  {
    throw std::system_error({errno, std::generic_category()}, "Failed to deregister file descriptor!");
  }
  m_epollEvents.pop_back();
}

void Process::Run()
{
  std::cout << "APP run" << "\n";
  int eventCount = 0;
  while(m_epollEvents.size() > 0)
  {
    int eventCount = epoll_wait(m_epollFd, &m_epollEvents[0], m_epollEvents.size(), -1);
    
    for(int eventIt = 0; eventIt < eventCount; eventIt++)
    {
      struct epoll_event& event = m_epollEvents[eventIt];
      // Ready to read
      if((event.events & EPOLLIN) == EPOLLIN)
      {
      }
      // Ready to write
      if((event.events & EPOLLOUT) == EPOLLOUT)
      {
      }
      // Error
      else
      {
      }
      int fd = event.data.fd;
      m_epollCallback[fd](fd);
    }
    
    if(eventCount == -1)
    {
      throw std::system_error({errno, std::generic_category()}, "epoll_wait error");
    }

    std::cout << "epoll_wait end" << "\n";

    for(int eventIt = 0; eventIt < eventCount; eventIt++)
    {
      std::cout << "FD Event " << m_epollEvents[eventIt].data.fd << "\n";
    }
  }
  std::cout << "APP finished" << "\n";
}
}