#ifndef SOCKET_H
#define SOCKET_H

#include <functional>
#include "event_consumer.h"
#include "utils/endpoint.h"

namespace Gap {

using SocketCallback = std::function<void()>;

class Socket : public EventConsumer {
   
  public:
    Socket();
    virtual ~Socket();

    void Listen(Endpoint endpoint);
    void Accept();
    void Connect(Endpoint endpoint);
    void Close();

    void SetOnReadyRead(SocketCallback callback);
    void SetOnReadyWrite(SocketCallback callback);
    void SetOnError(SocketCallback callback);

  private:
    void Open();
    void onEvent(EventFlags events) override;

    SocketCallback m_readyReadCallback;
    SocketCallback m_readyWriteCallback;
    SocketCallback m_errorCallback;
    
};

}

#endif