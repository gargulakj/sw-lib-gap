#include <system_error>
#include "server.h"
#include "log.h"

namespace Gap {

TcpServer::TcpServer(EventProvider& eventProvider):
  m_eventProvider(eventProvider)
{
  m_serverSocket.SetOnError([this](int errCode){onServerSocketError(errCode);});
  m_serverSocket.SetOnReadyRead([this](){onServerSocketReadyRead();});
  m_serverSocket.SetOnReadyWrite([this](){onServerSocketReadyWrite();});

}

TcpServer::~TcpServer()
{
  try
  {
    Close();
  }
  catch(const std::exception& e)
  {
  }  
}

void TcpServer::Close()
{
  m_eventProvider.RemoveConsumer(&m_serverSocket);
  m_serverSocket.Close();
}

void TcpServer::Open(IPv4Endpoint endpoint)
{
  m_serverSocket.Listen(endpoint);
  m_eventProvider.AddConsumer(&m_serverSocket);
}

void TcpServer::onServerSocketError(int errCode)
{
  std::error_code err {errCode, std::generic_category()};  
  log.Info("Server socket error! " + err.message());  

}

void TcpServer::onServerSocketReadyRead()
{
  log.Info("TcpServer::onServerSocketReadyRead");
  IPv4Endpoint endpoint;
  m_serverSocket.Accept(endpoint);
  log.Info("Incoming connection from " + endpoint.ToString());
  
}

void TcpServer::onServerSocketReadyWrite()
{
  log.Info("TcpServer::onServerSocketReadyWrite");

}

}