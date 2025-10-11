#include "server.h"
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace Gap {

TcpServer::~TcpServer()
{
  if(m_serverSocket != -1)
  {
    close(m_serverSocket);
  }
}

int TcpServer::Init()
{
  m_serverSocket = socket(AF_INET, SOCK_STREAM , 0);

  if(m_serverSocket == -1)
  {
    throw std::system_error({errno, std::generic_category()}, std::string("Cannot create server socket!"));  
  }

  return m_serverSocket;
}

void TcpServer::Open(Endpoint endpoint)
{
  // Specifying the address
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(endpoint.Port());
  serverAddress.sin_addr.s_addr = endpoint.IP().Get();
  
  // Binding socket.
  if(bind(m_serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
  {
    throw std::system_error({errno, std::generic_category()}, std::string("Cannot bind address ") + endpoint.ToString());    
  }
  else  
  {
    // Set socket listening
    if(listen(m_serverSocket, 64) == -1)
    {
      throw std::system_error({errno, std::generic_category()}, std::string("Cannot set socket listening ") + endpoint.ToString());
    }
    else
    {
      std::cout << "Server is listening on " << endpoint.ToString() << "\n";
    }
  }
}

void TcpServer::onConnected()
{
  std::cout << "connected \n";
}

}