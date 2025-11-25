#ifndef EVENT_PROVIDER_H
#define EVENT_PROVIDER_H

#include <functional>
#include <map>
#include "event_consumer.h"

namespace Gap {

class EventProvider
{

  friend class Process;
  
  public:
    void AddConsumer(EventConsumer* consumer);
    void RemoveConsumer(EventConsumer* consumer);

  private:
    std::map<int,EventConsumer*> m_eventConsumer;
    std::function<void(int)> onConsumerAdd;
    std::function<void(int)> onConsumerRemove;
    
};
    
}

#endif