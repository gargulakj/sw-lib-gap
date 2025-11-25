#ifndef GAP_TCP_CLIENT_H
#define GAP_TCP_CLIENT_H

#include "utils/endpoint.h"
#include "socket.h"
#include "event_provider.h"

namespace Gap {

class TcpClient
{
  public:
    TcpClient(EventProvider& eventProvider);
    ~TcpClient();

    void Connect(Endpoint endpoint);
    void Close();
    
  private:
    EventProvider& m_eventProvider;
    Socket m_socket;

    void onSocketError();
    void onSocketReadyRead();
    void onSocketReadyWrite();
  
};

}

#endif