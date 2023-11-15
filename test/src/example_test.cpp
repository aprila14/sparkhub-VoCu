#include "defines.h"
#include "test_utils.h"

#include "gtest/gtest.h"



TEST(ExampleTest, normalUsage)
{
    resetCmock();
    ASSERT_EQ(3+3, 6);
}
