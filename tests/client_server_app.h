#include "process.h"
#include "signal_handler.h"
#include "tcp/server.h"
#include "tcp/client.h"

class ServerApp : public Gap::Process
{
  public:
    void Init();
    
  private:
    Gap::EventProvider m_eventProvider;
    Gap::TcpServer m_tcpServer {m_eventProvider};

};

class ClientApp : public Gap::Process
{
  public:
    void Init();

  private:
    Gap::EventProvider m_eventProvider;
    Gap::TcpClient m_tcpClient {m_eventProvider};
    
};