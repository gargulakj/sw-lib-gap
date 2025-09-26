#include "process.h"
#include <stdexcept>

namespace Gap {

Process::Process()
{
  m_epollFd = epoll_create1(0);
  if (m_epollFd == -1)
  {
    std::runtime_error("Cannot create epoll instance!");
  }
}

}