#include <chrono>
#include <iostream>
#include <array>
#include "log.h"

namespace Gap {

Log log;

void Log::AppendTime(std::string& destination)
{
  // Array definition - for time
  const size_t timeLogLen = 48;
  size_t availableLen = timeLogLen - 1;
  size_t writeIt = 0;
  std::array<char,timeLogLen> timeStrArray{};

  // Get time
  auto now = std::chrono::system_clock::now();  
  std::time_t time = std::chrono::system_clock::to_time_t(now);
  uint16_t milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000;
    
  // Write date and time
  writeIt += std::strftime(timeStrArray.begin(), availableLen, "[%F %T", std::localtime(&time));
  availableLen -= writeIt;
  
  // Write miliseconds
  if(availableLen > 0)
  {
    snprintf(timeStrArray.begin() + writeIt, availableLen, ".%03u]", milliseconds);
  }

  destination.append(timeStrArray.data());
}

void Log::Info(std::string msg)
{
 
  std::string outMessage;
  AppendTime(outMessage);
  outMessage += " INFO: ";
  outMessage += msg;
  
  std::cout << outMessage << "\n" << std::flush;
  
}

}