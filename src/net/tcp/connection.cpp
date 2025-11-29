#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "connection.h"
#include "log.h"

namespace Gap {

TcpConnection::TcpConnection(EventProvider& eventProvider):
  m_eventProvider(eventProvider)
{
  m_socket.SetOnError([this](int errCode){onError(errCode);});
  m_socket.SetOnReadyRead([this](){onReadyRead();});
  m_socket.SetOnReadyWrite([this](){onReadyWrite();});

}

TcpConnection::~TcpConnection()
{
  try
  {
    Close();
  }
  catch(const std::exception& e)
  {
  } 
}

void TcpConnection::Connect(IPv4Endpoint endpoint)
{
  m_socket.Connect(endpoint);
  m_eventProvider.AddConsumer(&m_socket);
}

void TcpConnection::Close()
{
  m_eventProvider.RemoveConsumer(&m_socket);
  m_socket.Close();
}

std::size_t TcpConnection::Read(Buffer& buffer)
{
  return m_socket.ReadData(buffer.GetWritable());
}

std::size_t TcpConnection::Send(Buffer& buffer)
{
  return m_socket.SendData(buffer.GetData());
}

// void TcpClient::onError(int errCode)
// {  
//   std::error_code err {errCode, std::generic_category()};  
//   log.Info("TCP client socket error! " + err.message());  

// }

// void TcpClient::onReadyRead()
// {
//   log.Info("TcpClient::onSocketReadyRead");
//   std::vector<std::byte> v;
//   v.resize(10);

//   std::size_t readed = m_socket.ReadData(v);
//   printf("READED %u\n", readed);
  
//    for(const std::byte& i : v) 
//     printf("0x%2x ",(uint8_t)i);
//   printf("\n");
// }
  

// void TcpClient::onReadyWrite()
// {
//   log.Info("TcpClient::onSocketReadyWrite");
//   std::vector<std::byte> v = { std::byte{0xFF}, std::byte{0xAA}, std::byte{0xCB}, std::byte{0x12}, std::byte{0x13}, std::byte{0x22}/*, static_cast<std::byte>('\n') */};
//   std::size_t sended = m_socket.SendData(v);

//   printf("SENDED %u\n", sended);
//   for(const std::byte& i : v) 
//     printf("0x%2x ",(uint8_t)i);
//   printf("\n");
    
// }

}