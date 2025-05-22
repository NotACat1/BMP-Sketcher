#include "example.hpp"
#include <gtest/gtest.h>

TEST(BMPLibrary, HelloTest) {
    EXPECT_EQ(bmp::hello(), "BMP Library is working!");
}