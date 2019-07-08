#include <gtest/gtest.h>
#include "example.hpp"

TEST(SqrTest, HandleValues){
    EXPECT_EQ(sqr(2),4);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
