#include "process.h"
#include "tcp/server.h"
#include "tcp/client.h"

class ServerApp : public Gap::Process
{
  public:
    void Init();
  private:
    void onSocketEvent(int fd, Event evn);
    Gap::TcpServer m_tcpServer;
};

class ClientApp : public Gap::Process
{
  public:
    void Init();
    virtual void onTerminate() override;
  private:
    void onSocketEvent(int fd, Event evn);
    Gap::TcpClient m_tcpClient;
};