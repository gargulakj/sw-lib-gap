#ifndef GAP_UTILS_ENDPOINT_H
#define GAP_UTILS_ENDPOINT_H

#include <cstdint>
#include <string>
#include <arpa/inet.h>

namespace Gap {

class IPAddress
{   
  private:
    in_addr_t m_addr;
    
  public:
    IPAddress(std::string address);
    in_addr_t Get() const;
    std::string ToString() const;
    
};

class Endpoint
{
  private:
    IPAddress m_ipAddress;
    uint16_t m_port;  

  public:
    Endpoint(IPAddress address, uint16_t port);
    IPAddress IP() const;
    uint16_t Port() const;
    std::string ToString() const;
};

}

#endif