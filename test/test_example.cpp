#include <gtest/gtest.h>
#include "example.hpp"

TEST(Main, Square){
    EXPECT_EQ(sqr(2),4);
}

TEST(Main, EqualsOne)
{
    EXPECT_EQ(1, 1);
}
