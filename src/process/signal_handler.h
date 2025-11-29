#ifndef GAP_SIGNAL_HANDLER_H
#define GAP_SIGNAL_HANDLER_H

#include <vector>
#include "event_consumer.h"

namespace Gap 
{

using SignalCallback = std::function<void(uint32_t)>;

class SignalHandler : public EventConsumer
{

  public:
    SignalHandler(std::vector<int> signalSet);
    virtual ~SignalHandler();

    void SetOnSignal(SignalCallback callback);
    int GetFd() const;

  private:
    void onEvent(EventFlags events) override;

    std::vector<int> m_signalSet;
    SignalCallback m_onSignal;
};
}
#endif