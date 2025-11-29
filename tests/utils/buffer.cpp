#include <gtest/gtest.h>
#include <cstring>
#include "utils/buffer.h"
#include "log.h"

static void addToBuff(Buffer& buffer, uint32_t count)
{
  std::vector<std::byte> pattern;
  for(uint32_t i = 0; i < count; i++)
  {
    pattern.push_back(std::byte{static_cast<uint8_t>(i)});
  }

  std::memcpy(buffer.GetWritable().data(), pattern.data(), pattern.size());
  buffer.AddUsage(pattern.size());
}

// Buffer test
TEST(Utils, Buffer) {
  
  Buffer buff(256);

  addToBuff(buff, 200);

  // Check buffer size
  EXPECT_EQ(buff.GetData().size(), 200);
  EXPECT_EQ(buff.FreeSize(), 56);
  EXPECT_EQ(buff.TotalSize(), 256);

  // Check 0 - 199
  for(uint8_t i = 0; i < 200; i++)
  {
    EXPECT_EQ(static_cast<uint8_t>(buff.GetData()[i]), i);
  }

  addToBuff(buff, 56);
 
  // Check buffer size
  EXPECT_EQ(buff.GetData().size(), 256);
  EXPECT_EQ(buff.FreeSize(), 0);
  EXPECT_EQ(buff.TotalSize(), 256);

  // Check 200 - 255
  for(uint16_t i = 0; i < 56; i++)
  {
    EXPECT_EQ(static_cast<uint8_t>(buff.GetData()[i+200]), i);
  }

  // Check drop data
  buff.DropData(100);
  EXPECT_EQ(buff.GetData().size(), 156);
  EXPECT_EQ(buff.FreeSize(), 100);
  EXPECT_EQ(buff.TotalSize(), 256);

  for(uint16_t i = 100; i < 156; i++)
  {
    EXPECT_EQ(static_cast<uint8_t>(buff.GetData()[i]), i - 100);
  }

  // Increase size
  buff.Realloc(1024);

  EXPECT_EQ(buff.GetData().size(), 156);
  EXPECT_EQ(buff.FreeSize(), 868);
  EXPECT_EQ(buff.TotalSize(), 1024);
  // Check data not affected
  for(uint16_t i = 100; i < 156; i++)
  {
    EXPECT_EQ(static_cast<uint8_t>(buff.GetData()[i]), i - 100);
  }
  
}

