#include "process.h"
#include "signal_handler.h"
#include "tcp/server.h"
#include "tcp/connection.h"
#include "event_provider.h"

class ServerApp : public Gap::Process
{
  public:
    void Init();
    
  private:
    Gap::EventProvider m_eventProvider;
    Gap::TcpServer m_tcpServer {m_eventProvider};

};

class TcpClient : public Gap::TcpConnection
{
  public:
    TcpClient(Gap::EventProvider& eventProvider);

  private:
    void onError(int errCode) override;
    void onReadyRead() override;
    void onReadyWrite() override;
};

class ClientApp : public Gap::Process
{
  public:
    void Init();

  private:
    Gap::EventProvider m_eventProvider;
    TcpClient m_tcpClient {m_eventProvider};
    
};