#include "server.h"
#include "log.h"



#include <unistd.h>

namespace Gap {

TcpServer::TcpServer(EventProvider& eventProvider):
  m_eventProvider(eventProvider)
{
  m_serverSocket.SetOnError([this](){onServerSocketError();});
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

void TcpServer::Open(Endpoint endpoint)
{
  m_serverSocket.Listen(endpoint);
  m_eventProvider.AddConsumer(&m_serverSocket);
}

void TcpServer::onServerSocketError()
{
  log.Info("Server socket error!");

}

void TcpServer::onServerSocketReadyRead()
{
  log.Info("TcpServer::onServerSocketReadyRead");
  m_serverSocket.Accept();
  
}

void TcpServer::onServerSocketReadyWrite()
{
  log.Info("TcpServer::onServerSocketReadyWrite");

}

}