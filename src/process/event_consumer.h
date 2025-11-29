#ifndef GAP_EVENT_CONSUMER_H
#define GAP_EVENT_CONSUMER_H

#include <functional>
#include "flags.h"

namespace Gap 
{
enum class EventFlag : uint32_t {
  Error = 1,
  ReadyRead = 2,
  ReadyWrite = 4,  
};

using EventFlags = Flags<EventFlag>;
using EventCallback = std::function<void(EventFlags)>;

class EventConsumer
{
  friend class EventProvider;

  public:
    virtual void onEvent(EventFlags events) = 0;
    
  protected:
    int m_fd {-1};

};

}
#endif