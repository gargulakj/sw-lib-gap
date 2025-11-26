#ifndef SOCKET_H
#define SOCKET_H

#include <functional>
#include <span>
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

    // If the amount of data read equals the buffer size, it indicates that more
    // data is available, so the read operation should be repeated immediately.
    std::size_t ReadData(std::span<std::byte> buffer);
    
    // If not all data from the buffer was sent, retry the call after some delay.
    std::size_t SendData(std::span<std::byte> buffer);

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