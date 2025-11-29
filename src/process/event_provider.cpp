#include <stdexcept>
#include <system_error>
#include "event_provider.h"

namespace Gap {

void EventProvider::AddConsumer(EventConsumer* consumer)
{
  if(consumer != nullptr)
  {    
    auto exist = m_eventConsumer.find(consumer->m_fd);
    if(exist == m_eventConsumer.end())
    {      
      m_eventConsumer[consumer->m_fd] = consumer;
      onConsumerAdd(consumer->m_fd);
    }
    else
    {
      throw std::runtime_error("EventProvider::Event consumer is already registered!");
    }
  }
}

void EventProvider::RemoveConsumer(EventConsumer* consumer)
{
  if(consumer != nullptr)
  {
    auto removeItem = m_eventConsumer.find(consumer->m_fd);
    if(removeItem != m_eventConsumer.end())
    {
      onConsumerRemove(consumer->m_fd);
      m_eventConsumer.erase(removeItem);
    }
    else
    {
      throw std::runtime_error("EventProvider::Event consumer is already removed!");
    }
  }
}

}