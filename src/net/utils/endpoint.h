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
    IPAddress(in_addr_t address);
    IPAddress(std::string address);
    in_addr_t Get() const;
    std::string ToString() const;
    
};

class IPv4Endpoint
{
  private:
    IPAddress m_ipAddress {"0.0.0.0"};
    uint16_t m_port{0};  

  public:
    IPv4Endpoint(){};
    IPv4Endpoint(IPAddress address, uint16_t port);
    IPAddress IP() const;
    uint16_t Port() const;
    std::string ToString() const;
};

}

#endif