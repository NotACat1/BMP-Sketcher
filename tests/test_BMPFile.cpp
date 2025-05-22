#include "BMPFile.hpp"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

// Утилита: генерация временного пути
std::string getTempFilename(const std::string& base = "temp_test.bmp") {
    return std::filesystem::temp_directory_path() / base;
}

TEST(BMPFileTest, CreateAndPixelAccess) {
    BMPFile bmp;
    BMPFile::Pixel red(255, 0, 0);
    bmp.create(10, 10, BMPFile::PixelFormat::BGR24, red);

    EXPECT_EQ(bmp.width(), 10);
    EXPECT_EQ(bmp.height(), 10);
    EXPECT_FALSE(bmp.is32bit());

    for (int y = 0; y < bmp.height(); ++y) {
        for (int x = 0; x < bmp.width(); ++x) {
            EXPECT_EQ(bmp.getPixel(x, y), red);
        }
    }
}

TEST(BMPFileTest, SetAndGetPixel) {
    BMPFile bmp;
    bmp.create(5, 5, BMPFile::PixelFormat::BGRA32, {});

    BMPFile::Pixel green(0, 255, 0);
    bmp.setPixel(2, 2, green);

    EXPECT_EQ(bmp.getPixel(2, 2), green);
}

TEST(BMPFileTest, FlipVertically) {
    BMPFile bmp;
    bmp.create(3, 3, BMPFile::PixelFormat::BGR24, {});

    BMPFile::Pixel top_pixel(255, 0, 0);
    bmp.setPixel(1, 0, top_pixel);
    bmp.flipVertically();

    EXPECT_EQ(bmp.getPixel(1, 2), top_pixel);
}

TEST(BMPFileTest, ConvertToBlackAndWhite) {
    BMPFile bmp;
    bmp.create(2, 1, BMPFile::PixelFormat::BGR24, {});

    bmp.setPixel(0, 0, {255, 255, 255});  // bright
    bmp.setPixel(1, 0, {10, 10, 10});     // dark
    bmp.convertToBlackAndWhite();

    EXPECT_EQ(bmp.getPixel(0, 0), BMPFile::Pixel(255, 255, 255));
    EXPECT_EQ(bmp.getPixel(1, 0), BMPFile::Pixel(0, 0, 0));
}

TEST(BMPFileTest, SaveAndLoad) {
    BMPFile bmp1;
    BMPFile::Pixel blue(0, 0, 255);
    bmp1.create(4, 4, BMPFile::PixelFormat::BGRA32, blue);

    std::string filename = getTempFilename();

    ASSERT_TRUE(bmp1.save(filename));

    BMPFile bmp2;
    ASSERT_TRUE(bmp2.load(filename));

    EXPECT_EQ(bmp2.width(), 4);
    EXPECT_EQ(bmp2.height(), 4);
    EXPECT_TRUE(bmp2.is32bit());
    EXPECT_EQ(bmp2.getPixel(0, 0), blue);

    std::filesystem::remove(filename);  // clean up
}

TEST(BMPFileTest, LoadInvalidFile) {
    std::string filename = getTempFilename("invalid.bmp");
    std::ofstream out(filename);
    out << "Not a BMP file";
    out.close();

    BMPFile bmp;
    EXPECT_FALSE(bmp.load(filename));

    std::filesystem::remove(filename);
}
