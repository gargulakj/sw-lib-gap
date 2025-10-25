#include <gtest/gtest.h>
#include <thread>
#include "client_server_app.h"

static ServerApp serverApp;

void ServerApp::Init()
{
  m_tcpServer.Init();
  //RegisterEvent()
}
static void ServerLoop()
{  
  serverApp.Run();
}


// Demonstrate some basic assertions.
TEST(ServerTesting, BasicAssertions) {
  
  std::thread srvThread(ServerLoop);
  
}