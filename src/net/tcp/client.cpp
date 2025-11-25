#include "client.h"
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "log.h"

namespace Gap {

TcpClient::TcpClient(EventProvider& eventProvider):
  m_eventProvider(eventProvider)
{
  m_socket.SetOnError([this](){onSocketError();});
  m_socket.SetOnReadyRead([this](){onSocketReadyRead();});
  m_socket.SetOnReadyWrite([this](){onSocketReadyWrite();});

}

TcpClient::~TcpClient()
{
  try
  {
    Close();
  }
  catch(const std::exception& e)
  {
  } 
}


void TcpClient::Connect(Endpoint endpoint)
{
  m_socket.Connect(endpoint);
  m_eventProvider.AddConsumer(&m_socket);
}

void TcpClient::Close()
{
  m_eventProvider.RemoveConsumer(&m_socket);
  m_socket.Close();
}

void TcpClient::onSocketError()
{
  log.Info("TcpClient socket error!");

}

void TcpClient::onSocketReadyRead()
{
  log.Info("TcpClient::onSocketReadyRead");
}

void TcpClient::onSocketReadyWrite()
{
  log.Info("TcpClient::onSocketReadyWrite");

}

}