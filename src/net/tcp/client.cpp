#include "client.h"
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "log.h"

namespace Gap {

TcpClient::~TcpClient()
{
  if(m_clientSocket != -1)
  {
    close(m_clientSocket);
  }
}

int TcpClient::Init()
{
  m_clientSocket = socket(AF_INET, SOCK_STREAM , 0);

  if(m_clientSocket == -1)
  {
    throw std::system_error({errno, std::generic_category()}, std::string("Cannot create client socket!"));  
  }

  return m_clientSocket;
}

void TcpClient::Connect(Endpoint endpoint)
{
  // Specifying the address
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(endpoint.Port());
  serverAddress.sin_addr.s_addr = endpoint.IP().Get();

  if(connect(m_clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
  {
    throw std::system_error({errno, std::generic_category()}, std::string("Cannot connect to server ") + endpoint.ToString());
  }
  else
  {
    log.Info("Connected to server " + endpoint.ToString());
  }
}

void TcpClient::Close()
{
  if(shutdown(m_clientSocket, SHUT_RDWR) == -1)
  {
    throw std::system_error({errno, std::generic_category()}, std::string("Cannot close socket "));
  }
  else
  {
    log.Info("Disconnected from server");
  }
  
}

void TcpClient::onConnected()
{
  std::cout << "connected \n";
}

}