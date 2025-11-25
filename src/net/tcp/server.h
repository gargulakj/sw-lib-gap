#ifndef GAP_TCP_SERVER_H
#define GAP_TCP_SERVER_H

#include "utils/endpoint.h"
#include "event_provider.h"
#include "socket.h"

namespace Gap {

class TcpServer
{
  public:
    TcpServer(EventProvider& eventProvider);
    ~TcpServer();
    
    void Close();
    void Open(Endpoint endpoint);

  private:
    void onServerSocketError();
    void onServerSocketReadyRead();
    void onServerSocketReadyWrite();

    EventProvider& m_eventProvider;
    Socket m_serverSocket;

};

}

#endif