#ifndef GAP_UTILS_BUFFER_H
#define GAP_UTILS_BUFFER_H

#include <memory>
#include <span>
#include <cstddef>

class Buffer
{
  public:
    Buffer(std::size_t size);
    
    void AddUsage(std::size_t used);
    std::span<std::byte> GetWritable();

    std::span<std::byte> GetData();
    
    void DropData(std::size_t count);
    
    void Realloc(std::size_t size);

    std::size_t TotalSize() const;
    std::size_t FreeSize() const;


  private:
    std::size_t m_size {0};
    std::size_t m_used {0};
        
    std::unique_ptr<std::byte[]> m_buffer;

};

#endif
