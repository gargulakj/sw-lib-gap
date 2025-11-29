#include <cstring>
#include <algorithm>
#include "buffer.h"

Buffer::Buffer(std::size_t size) :
  m_size(size)
{  
  m_buffer = std::make_unique<std::byte[]>(m_size);
}

void Buffer::AddUsage(std::size_t used)
{
  m_used += used;
}

std::span<std::byte> Buffer::GetWritable()
{
  return { m_buffer.get() + m_used, m_size - m_used };
}

std::span<std::byte> Buffer::GetData()
{
  return { m_buffer.get(), m_used };
}

void Buffer::DropData(std::size_t count)
{
  if(count < m_size)
  {    
    std::shift_left(m_buffer.get(), m_buffer.get() + m_size, count);

    if(m_used > count)
    {
      m_used -= count;
    }
    else
    {
      m_used = 0;
    }
  }
  else 
  {
    throw std::runtime_error("Buffer::Cannot drop data. Requested drop is " 
      + std::to_string(count) + " but capacity is " + std::to_string(m_size));
  }
}

void Buffer::Realloc(std::size_t size)
{
  std::unique_ptr<std::byte[]> newBuffer (new std::byte[size]);
  std::memcpy(newBuffer.get(), m_buffer.get(), (size > m_size) ? m_size : size);
  m_buffer = std::move(newBuffer);

  if(m_used > size)
  {
    m_used = size;
  }

  m_size = size;
}

std::size_t Buffer::TotalSize() const
{
  return m_size;
}

std::size_t Buffer::FreeSize() const
{
  return m_size - m_used;
}
