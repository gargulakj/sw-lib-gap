#ifndef FLAGS_H
#define FLAGS_H

#include <cstdint>
#include <type_traits>

template <typename FlagType> class Flags 
{
  public:
    void Clear() 
    { 
      m_value = 0;
    }

    void Set(FlagType flag)
    {
      m_value |= static_cast<std::underlying_type_t<FlagType>>(flag);
    }

    bool Has(FlagType flag)
    {
      std::underlying_type_t<FlagType> flagTyped = static_cast<std::underlying_type_t<FlagType>>(flag);
      return ((m_value & flagTyped) == flagTyped);
    }
    
  private:
    std::underlying_type_t<FlagType> m_value {0};
};

#endif