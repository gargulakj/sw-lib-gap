#include <stdexcept>
#include <system_error>
#include <errno.h>
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include "process.h"
#include "flags.h"
#include "log.h"

namespace Gap {

Process::Process()
{
  // Store thread ID
  m_threadId = pthread_self();
 
  // Epoll create
  m_epollFd = epoll_create1(0);
  if (m_epollFd == -1)
  {
    throw std::system_error({errno, std::generic_category()}, "Cannot create epoll instance!");
  }
  
  RegisterEvent(m_exitSignal.GetFd());

}

Process::~Process()
{
  if(m_epollFd != -1)
  {
    close(m_epollFd);
  }
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
      
      if(fd != m_exitSignal.GetFd())
      {        
        try
        {
          if(m_eventProvider != nullptr)
          {
            // Consumer is there?
            EventConsumer* consumer = m_eventProvider->m_eventConsumer.at(fd);
            if(consumer != nullptr)
            {
              EventFlags events;

              // Error
              if((event.events & (EPOLLHUP | EPOLLERR)) > 0)
              {
                events.Set(EventFlag::Error);
              }        
              // Ready to read
              if((event.events & EPOLLIN) == EPOLLIN)
              {
                events.Set(EventFlag::ReadyRead);
              }
              // Ready to write
              if((event.events & EPOLLOUT) == EPOLLOUT)
              {
                events.Set(EventFlag::ReadyWrite);
              }
              
              consumer->onEvent(events);      
            }
            else 
            {
              throw std::runtime_error("Cannot find epoll consumer");
            }
          }
          else 
          {
            throw std::runtime_error("Event provider is not set!");
          }
        }
        catch(const std::exception& e)
        {
          throw std::runtime_error("Cannot find epoll consumer");
        }        
      }
    }
    
    // Epoll error
    if(eventCount == -1)
    {
      if(m_epollEvents.size() == 0)
      {
        throw std::runtime_error("No epoll consumers!");
      }
      else
      {      
        throw std::system_error({errno, std::generic_category()}, "epoll_wait error");
      }
    }
  }
  
  log.Info("Application finished");
}

void Process::Terminate()
{
  log.Info("Application terminate requested");
  m_run = false;
  
  if(m_threadId != 0)
  {
    pthread_kill(m_threadId, SIGQUIT);
    m_threadId = 0;
  }
}

void Process::SetEventProvider(EventProvider* provider)
{
  UnsetEventProvider();

  if(provider != nullptr)
  {
    m_eventProvider = provider;
    m_eventProvider->onConsumerAdd = [this](int fd){RegisterEvent(fd);};
    m_eventProvider->onConsumerRemove = [this](int fd){DeregisterEvent(fd);};
  }
}

void Process::UnsetEventProvider()
{
  if(m_eventProvider != nullptr)
  {
    for(auto const& item : m_eventProvider->m_eventConsumer)
    {
      DeregisterEvent(item.first);
    }
  }
}

void Process::RegisterEvent(int fd)
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
  
  log.Info("Process::RegisterEvent for fd: " + std::to_string(fd));

}

void Process::DeregisterEvent(int fd)
{
  // Deregister epol event.
  if (epoll_ctl(m_epollFd, EPOLL_CTL_DEL, fd, nullptr) == -1)
  {
    throw std::system_error({errno, std::generic_category()}, "Failed to deregister file descriptor!");
  }

  // Remove structure for getting event.
  m_epollEvents.pop_back();  
}

}