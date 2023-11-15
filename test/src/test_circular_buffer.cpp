#include <gtest/gtest.h>


#include "circular_buffer.h"



TEST(CircularBuffer, general)
{
    int a = 9;
    int b = a+4;
    int c = a+b;
    uint8_t buffer[5] = {0};

    CircularBuffer circularBuffer(buffer, sizeof(buffer));

    ASSERT_EQ(circularBuffer.readByte(), CIRCULAR_BUFFER__NO_DATA);
    ASSERT_TRUE(circularBuffer.writeByte(0x11));
    ASSERT_EQ(circularBuffer.readByte(), 0x11);

    ASSERT_TRUE(circularBuffer.writeByte(0x12));
    ASSERT_TRUE(circularBuffer.writeByte(0x13));
    ASSERT_TRUE(circularBuffer.writeByte(0x14));

    ASSERT_EQ(circularBuffer.readByte(), 0x12);
    ASSERT_EQ(circularBuffer.readByte(), 0x13);
    ASSERT_EQ(circularBuffer.readByte(), 0x14);
    ASSERT_EQ(circularBuffer.readByte(), CIRCULAR_BUFFER__NO_DATA);

    std::vector<uint8_t> data = {0x25, 0x26, 0x27};
    ASSERT_TRUE(circularBuffer.writeData(data.data(), static_cast<uint32_t>(data.size())));

    ASSERT_EQ(circularBuffer.readByte(), 0x25);
    ASSERT_EQ(circularBuffer.readByte(), 0x26);
    ASSERT_EQ(circularBuffer.readByte(), 0x27);
    ASSERT_EQ(circularBuffer.readByte(), CIRCULAR_BUFFER__NO_DATA);

}


TEST(CircularBuffer, single_bytes)
{
    uint32_t bufferSize = 6;
    uint8_t buffer[bufferSize];

    CircularBuffer circularBuffer(buffer, sizeof(buffer));

    for (int i = 0; i < 30; i++)
    {
        uint8_t byte = static_cast<uint8_t>(0x11 + i);
        ASSERT_EQ(circularBuffer.readByte(), CIRCULAR_BUFFER__NO_DATA);
        ASSERT_TRUE(circularBuffer.writeByte(byte));
        ASSERT_EQ(circularBuffer.readByte(), byte);
    }

}

TEST(CircularBuffer, arrays)
{
    uint32_t bufferSize = 7;
    uint8_t buffer[bufferSize];

    CircularBuffer circularBuffer(buffer, sizeof(buffer));


    for (int i = 0; i < 5; i++)
    {
        std::vector<uint8_t> data1 = {0x15, 0x16, 0x17};
        std::vector<uint8_t> data2 = {0x25, 0x26, 0x27};
        std::vector<uint8_t> data3 = {0x35, 0x36, 0x37};

        ASSERT_TRUE(circularBuffer.writeData(data1.data(), static_cast<uint32_t>(data1.size())));
        ASSERT_TRUE(circularBuffer.writeData(data2.data(), static_cast<uint32_t>(data2.size())));
        ASSERT_FALSE(circularBuffer.writeData(data3.data(), static_cast<uint32_t>(data3.size())));

        ASSERT_EQ(circularBuffer.readByte(), data1[0]);
        ASSERT_EQ(circularBuffer.readByte(), data1[1]);
        ASSERT_EQ(circularBuffer.readByte(), data1[2]);
        ASSERT_EQ(circularBuffer.readByte(), data2[0]);
        ASSERT_EQ(circularBuffer.readByte(), data2[1]);
        ASSERT_EQ(circularBuffer.readByte(), data2[2]);
        ASSERT_EQ(circularBuffer.readByte(), CIRCULAR_BUFFER__NO_DATA);
    }

}
