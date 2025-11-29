#ifndef GAP_TCP_CONNECTION_H
#define GAP_TCP_CONNECTION_H

#include "utils/endpoint.h"
#include "utils/buffer.h"
#include "socket.h"
#include "event_provider.h"

namespace Gap {

class TcpConnection
{
  public:
    TcpConnection(EventProvider& eventProvider);
    virtual ~TcpConnection();

    void Connect(IPv4Endpoint endpoint);
    void Close();

    std::size_t Read(Buffer& buffer);
    std::size_t Send(Buffer& buffer);
  
  protected:
    virtual void onError(int errCode) = 0;
    virtual void onReadyRead() = 0;
    virtual void onReadyWrite() = 0;
    
  private:
    EventProvider& m_eventProvider;
    Socket m_socket;
  
};

}

#endif