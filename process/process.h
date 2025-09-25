namespace Gaf {

//#include <sys/epoll.h>

class Process
{
  public:
    Process();
  
  private:
    int m_epollFd;
};

}