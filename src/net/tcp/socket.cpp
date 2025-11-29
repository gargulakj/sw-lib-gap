#include <sys/socket.h>
#include <netinet/in.h>
#include <system_error>
#include <unistd.h>
#include <cerrno>
#include <utility>
#include "socket.h"
#include "log.h"

namespace Gap {

Socket::Socket(int socketFd)
{
  m_fd = socketFd;
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
    int errCode = 0;
    socklen_t errCodeSize = sizeof(errCode);
    getsockopt(m_fd, SOL_SOCKET, SO_ERROR, &errCode, &errCodeSize);

    if(m_errorCallback)
      m_errorCallback(errCode);
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



void Socket::Listen(IPv4Endpoint endpoint)
{
  Open();

  // Specifying the address
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(endpoint.Port());
  serverAddress.sin_addr.s_addr = endpoint.IP().Get();

  SetOption(SO_REUSEADDR, 1);
  
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

int Socket::Accept(IPv4Endpoint& remoteEndpoint)
{
  struct sockaddr addr;
  socklen_t addrlen = sizeof(addr);
  int connectionSocket = accept(m_fd , &addr, &addrlen);

  if(connectionSocket == -1)
  {
    throw std::system_error({errno, std::generic_category()}, std::string("Socket::Cannot accept connection! ") );
  }
  else
  {
    sockaddr_in *sin = reinterpret_cast<sockaddr_in*>(&addr);
    if(sin != nullptr)
    {
      IPv4Endpoint endpoint {{sin->sin_addr.s_addr}, ntohs(sin->sin_port)};
      remoteEndpoint = std::move(endpoint);      
    }    
  }

  return connectionSocket;
}

void Socket::Connect(IPv4Endpoint endpoint)
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

void Socket::SetOption(int option, int value)
{
  if(setsockopt(m_fd, SOL_SOCKET, option, static_cast<void*>(&value), sizeof(value)) == -1)
  {
    throw std::system_error({errno, std::generic_category()}, std::string("Socket::Cannot set option ") + std::to_string(option) + " ");
  }
}

int Socket::GetOption(int option)
{
  int value = 0;
  socklen_t valueLen = 0;
  if(getsockopt(m_fd, SOL_SOCKET, option, static_cast<void*>(&value), &valueLen) == -1)
  {
    throw std::system_error({errno, std::generic_category()}, std::string("Socket::Cannot get option ") + std::to_string(option) + " ");    
  }
  return value;
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

void Socket::SetOnReadyRead(SocketDataCallback callback)
{
  m_readyReadCallback = callback;
}

void Socket::SetOnReadyWrite(SocketDataCallback callback)
{
  m_readyWriteCallback = callback;
}

void Socket::SetOnError(SocketErrorCallback callback)
{
  m_errorCallback = callback;
}
}