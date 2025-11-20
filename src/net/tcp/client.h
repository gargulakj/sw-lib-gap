#ifndef GAP_TCP_CLIENT_H
#define GAP_TCP_CLIENT_H

#include "utils/endpoint.h"

namespace Gap {

class TcpClient
{
  public:
    ~TcpClient();
    int Init();
    void Connect(Endpoint endpoint);
    void Close();
    void onConnected();
    
  private:
    int m_clientSocket {-1};
};

}

#endif