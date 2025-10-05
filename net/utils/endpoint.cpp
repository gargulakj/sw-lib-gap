
#include "endpoint.h"

namespace Gap {

IPAddress::IPAddress(std::string address)
{
    m_addr = inet_addr(address.data());
}

in_addr_t IPAddress::Get() const
{
    return m_addr;
}

std::string IPAddress::ToString() const
{
    struct in_addr address;
    address.s_addr = m_addr;
    return std::string(inet_ntoa(address));
}

Endpoint::Endpoint(IPAddress address, uint16_t port) :
    m_ipAddress(address),
    m_port(port)
{

}

IPAddress Endpoint::IP() const 
{
    return m_ipAddress;
}

uint16_t Endpoint::Port() const 
{
    return m_port;
}

std::string Endpoint::ToString() const
{
    std::string endpointStr(m_ipAddress.ToString());
    endpointStr += ":";
    endpointStr += std::to_string(m_port);
    return endpointStr;
}

}
