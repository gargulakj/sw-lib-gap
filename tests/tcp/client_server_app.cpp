#include <gtest/gtest.h>
#include <thread>
#include "client_server_app.h"
#include "log.h"

std::unique_ptr<ServerApp> serverApp;
std::unique_ptr<ClientApp> clientApp;

TcpClient::TcpClient(Gap::EventProvider& eventProvider):
  TcpConnection(eventProvider)
{

}
void TcpClient::onError(int errCode) 
{

}
void TcpClient::onReadyRead()
{

}
void TcpClient::onReadyWrite() 
{

}

void ServerApp::Init()
{  
  Gap::log.Info("Server init");  
  
  SetEventProvider(&m_eventProvider);
  m_tcpServer.Open(Gap::IPv4Endpoint{{"127.0.0.1"}, 10004});
  
}
void ClientApp::Init()
{  
  Gap::log.Info("Client init");
  SetEventProvider(&m_eventProvider);
  
  m_tcpClient.Connect(Gap::IPv4Endpoint{{"127.0.0.1"}, 10004});
  //m_tcpClient.Connect(Gap::Endpoint{{"45.79.112.203"}, 4242});
  
  
}

static void ServerLoop()
{  
  serverApp = std::make_unique<ServerApp>();
  
  serverApp->Init();
  serverApp->Run();
}


static void ClientLoop()
{  
  clientApp = std::make_unique<ClientApp>();
  
  clientApp->Init();
  clientApp->Run();
}

static void Terminate()
{  
  sleep(100);
  clientApp->Terminate();
  serverApp->Terminate();
}



// Test connection client to server.
TEST(Process, ClientServer) {  
  std::thread srvThread(ServerLoop);  
  sleep(1);
  std::thread clientThread(ClientLoop);
  sleep(1);
  std::thread termThread(Terminate);

  clientThread.join();
  srvThread.join();
  termThread.join();
  
}
