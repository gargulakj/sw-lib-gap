#ifndef GAP_SOCKET_H
#define GAP_SOCKET_H

#include <functional>
#include <span>
#include "event_consumer.h"
#include "utils/endpoint.h"

namespace Gap {



class Socket : public EventConsumer {
   
  public:

    using SocketDataCallback = std::function<void()>;
    using SocketErrorCallback = std::function<void(int)>;

    Socket(int socketFd = -1);
    virtual ~Socket();

    void Listen(IPv4Endpoint endpoint);
    int Accept(IPv4Endpoint& remoteEndpoint);

    void Connect(IPv4Endpoint endpoint);

    void Close();

    // If the amount of data read equals the buffer size, it indicates that more
    // data is available, so the read operation should be repeated immediately.
    std::size_t ReadData(std::span<std::byte> buffer);
    
    // If not all data from the buffer was sent, retry the call after some delay.
    std::size_t SendData(std::span<std::byte> buffer);

    void SetOnReadyRead(SocketDataCallback callback);
    void SetOnReadyWrite(SocketDataCallback callback);
    void SetOnError(SocketErrorCallback callback);

  private:
    void Open();
    void onEvent(EventFlags events) override;

    void SetOption(int option, int value);
    int GetOption(int option);

    SocketDataCallback m_readyReadCallback;
    SocketDataCallback m_readyWriteCallback;
    SocketErrorCallback m_errorCallback;
    
};

}

#endif