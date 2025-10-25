#include "process.h"
#include "tcp/server.h"

class ServerApp : public Gap::Process
{
  public:
  private:
    void Init();
    Gap::TcpServer m_tcpServer;
};