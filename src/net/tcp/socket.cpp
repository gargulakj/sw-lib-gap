#include <sys/socket.h>
#include <netinet/in.h>
#include <system_error>
#include <unistd.h>
#include <cerrno>
#include "socket.h"
#include "log.h"

namespace Gap {

Socket::Socket()
{
  
}

Socket::~Socket()
{
  Close();
}

void Socket::Open()
{
  if(m_fd == -1)
  {
    m_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    if(m_fd == -1)
    {
      throw std::system_error({errno, std::generic_category()}, std::string("Socket::Cannot create socket!"));  
    }
  }
  else
  {
    throw std::runtime_error("Socket::Socket is already opened!");
  }
}

void Socket::onEvent(EventFlags events)
{
  if(events.Has(EventFlag::Error))
  {
    if(m_errorCallback)
      m_errorCallback();
  }
  else 
  {
    if(events.Has(EventFlag::ReadyRead))
    {
      if(m_readyReadCallback)
        m_readyReadCallback();
    }
    if(events.Has(EventFlag::ReadyWrite))
    {
      if(m_readyWriteCallback)
        m_readyWriteCallback();
    }
  }
}



void Socket::Listen(Endpoint endpoint)
{
  Open();

  // Specifying the address
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(endpoint.Port());
  serverAddress.sin_addr.s_addr = endpoint.IP().Get();
  
  // Binding socket.
  if(bind(m_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
  {
    throw std::system_error({errno, std::generic_category()}, std::string("Socket::Cannot bind address ") + endpoint.ToString());    
  }
  else  
  {
    // Set socket listening
    if(listen(m_fd, 64) == -1)
    {
      throw std::system_error({errno, std::generic_category()}, std::string("Socket::Cannot set socket listening ") + endpoint.ToString());
    }
    else
    {
      log.Info("Server is listening on " + endpoint.ToString());
    }
  }
}

void Socket::Accept()
{
  sockaddr addr;
  socklen_t addrlen;
  int peerSocket = accept(m_fd , (struct sockaddr *) &addr, &addrlen);
}

void Socket::Connect(Endpoint endpoint)
{
  Open();

  // Specifying the address
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(endpoint.Port());
  serverAddress.sin_addr.s_addr = endpoint.IP().Get();

  if(connect(m_fd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
  {
    if(errno != EINPROGRESS)
    {
      throw std::system_error({errno, std::generic_category()}, std::string("Socket::Cannot connect to server ") + endpoint.ToString());
    }
  }
}


void Socket::Close()
{
  if(m_fd != -1)
  {
    close(m_fd);
    m_fd = -1;
  }
}

std::size_t Socket::ReadData(std::span<std::byte> buffer)
{
  std::size_t retVal = 0;
  ssize_t received = recv(m_fd, buffer.data(), buffer.size(), 0);
  if(received >= 0)
  {
    retVal = static_cast<std::size_t>(received);
  }
  else if(errno != EAGAIN)
  {
    throw std::system_error({errno, std::generic_category()}, std::string("Socket::ReadData error!"));
  }
  
  return retVal;
}

std::size_t Socket::SendData(std::span<std::byte> buffer)
{
  std::size_t retVal = 0;
  ssize_t sended = send(m_fd, buffer.data(), buffer.size(), 0);
  if(sended >= 0)
  {
    retVal = static_cast<std::size_t>(sended);
  }
  else if(errno != EAGAIN)
  {
    throw std::system_error({errno, std::generic_category()}, std::string("Socket::SendData error!"));
  }

  return retVal;
}

void Socket::SetOnReadyRead(SocketCallback callback)
{
  m_readyReadCallback = callback;
}

void Socket::SetOnReadyWrite(SocketCallback callback)
{
  m_readyWriteCallback = callback;
}

void Socket::SetOnError(SocketCallback callback)
{
  m_errorCallback = callback;
}
}