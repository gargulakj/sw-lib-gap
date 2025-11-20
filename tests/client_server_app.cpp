#include <gtest/gtest.h>
#include <thread>
#include "client_server_app.h"
#include "log.h"

std::unique_ptr<ServerApp> serverApp;
std::unique_ptr<ClientApp> clientApp;

void ServerApp::Init()
{  
  Gap::log.Info("Server init");  
  
  int fd = m_tcpServer.Init();
  RegisterEvent(fd, [this](int fd, Event evn){ onSocketEvent(fd, evn); });
  m_tcpServer.Open(Gap::Endpoint{{"127.0.0.1"}, 10000});
  
}
void ServerApp::onSocketEvent(int fd, Event evn)
{
  Gap::log.Info("ServerApp::onSocketEvent " + std::to_string(evn));  

}

void ClientApp::Init()
{  
  Gap::log.Info("Client init");  
  
  int fd = m_tcpClient.Init();
  RegisterEvent(fd, [this](int fd, Event evn){ onSocketEvent(fd, evn); });
  m_tcpClient.Connect(Gap::Endpoint{{"127.0.0.1"}, 10000});
  
}

void ClientApp::onTerminate()
{
  m_tcpClient.Close();
}

void ClientApp::onSocketEvent(int fd, Event evn)
{
  Gap::log.Info("ClientApp::onSocketEvent " + std::to_string(evn));  

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
  sleep(5);
  clientApp->Terminate();
  serverApp->Terminate();
}



// Test connection client to server.
TEST(ClientServer, Connection) {  
  std::thread srvThread(ServerLoop);
  sleep(1);
  std::thread clientThread(ClientLoop);
  sleep(1);
  std::thread termThread(Terminate);
  clientThread.join();
  srvThread.join();
  termThread.join();
  
}