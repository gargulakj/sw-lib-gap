#ifndef GAP_LOG_H
#define GAP_LOG_H

#include <string>

namespace Gap {

class Log
{
  private:  
    void AppendTime(std::string& destination);

  public:
    void Info(std::string msg);
};

extern Log log;

}

#endif