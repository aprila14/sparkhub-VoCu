#include "defines.h"
#include "test_utils.h"

#include "gtest/gtest.h"


#include "cobs.h"


TEST(CobsTest, encode)
{
    std::vector<uint8_t> input = {0x13, 0x25, 0x00, 0x27, 0x00, 0x01};
    std::vector<uint8_t> output(100, 0);  // to have more space than needed

    std::vector<uint8_t> expectedOutput = {0x03, 0x13, 0x25, 0x02, 0x27, 0x02, 0x01};

    size_t encodedSize = cobs::encode(input.data(), input.size(), output.data());

    ASSERT_EQ(encodedSize, expectedOutput.size());
    output.resize(expectedOutput.size());

    ASSERT_EQ(expectedOutput, output);

}

TEST(CobsTest, decode)
{
    std::vector<uint8_t> input = {0x03, 0x13, 0x25, 0x02, 0x27, 0x02, 0x01};
    std::vector<uint8_t> output(100, 0);  // to have more space than needed

    std::vector<uint8_t> expectedOutput = {0x13, 0x25, 0x00, 0x27, 0x00, 0x01};

    size_t decodedSize = cobs::decode(input.data(), input.size(), output.data());

    ASSERT_EQ(decodedSize, expectedOutput.size());
    output.resize(expectedOutput.size());

    ASSERT_EQ(expectedOutput, output);
}
