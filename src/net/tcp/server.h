#ifndef GAP_TCP_SERVER_H
#define GAP_TCP_SERVER_H

#include "utils/endpoint.h"

namespace Gap {

class TcpServer
{
  public:
    ~TcpServer();
    int Init();
    void Close();
    void Open(Endpoint endpoint);
    void onConnected();

  private:
    int m_serverSocket {-1};
};

}

#endif