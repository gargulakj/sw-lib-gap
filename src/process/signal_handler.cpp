#include <signal.h>
#include <sys/signalfd.h>
#include <stdexcept>
#include <system_error>
#include <unistd.h>
#include "signal_handler.h"

#include "log.h"

namespace Gap
{

SignalHandler::SignalHandler(std::vector<int> signalSet) :
  m_signalSet(signalSet)
{  
  // Fill structure with signals which are handle by FD.
  sigset_t signals;
  sigemptyset(&signals);
  
  for(int sig : m_signalSet)
  {
    sigaddset(&signals, sig);
  }

  // Block signals which is handle by FD
  if (sigprocmask(SIG_BLOCK, &signals, NULL) == -1)
  {
    throw std::system_error({errno, std::generic_category()}, "Cannot block signals handled by FD!");
  }

  // Create FD for signal handling.
  m_fd = signalfd(-1, &signals, SFD_NONBLOCK);
  if (m_fd == -1)
  {
    throw std::system_error({errno, std::generic_category()}, "Cannot create FD for signal handling!");
  }
}

SignalHandler::~SignalHandler()
{
  if(m_fd != -1)
  {
    close(m_fd);
  }
}

void SignalHandler::onEvent(EventFlags events)
{
  if(!events.Has(EventFlag::Error))
  {
    if(m_onSignal)
    {
      signalfd_siginfo sigInfo;
      ssize_t readSize = 0;
      size_t infoSize = sizeof(sigInfo);

      // Read info about signal.
      while(readSize != -1)
      {
        readSize = read(m_fd, &sigInfo, infoSize);
        if(readSize == infoSize)
        {
          // For now is interesting only signal number.
          m_onSignal(sigInfo.ssi_signo);
        }
      }    
    }
  }
}

void SignalHandler::SetOnSignal(SignalCallback callback)
{
  m_onSignal = callback;
}

int SignalHandler::GetFd() const
{
  return m_fd;
}

}