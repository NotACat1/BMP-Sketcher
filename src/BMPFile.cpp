/**
 * @file BMPFile.cpp
 * @brief Implementation of methods for working with BMP images
 * @details Contains complete implementation of BMPFile class for loading, saving and processing BMP files
 */

#include "BMPFile.hpp"
#include <stdexcept>
#include <cstring>
#include <algorithm>

/**
 * @brief Loads BMP image from file
 * @param filename Path to BMP file
 * @return true if file loaded successfully, false on error
 * @throws std::runtime_error on invalid file format or unsupported BMP type
 */
bool BMPFile::load(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) return false;

    try {
        readHeaders(file);

        // Check "BM" signature
        if (bmp_header_.signature != 0x4D42)
            throw std::runtime_error("Not a BMP file");

        // Only support 24 and 32 bits per pixel
        if (dib_header_.bits_per_pixel != 24 && dib_header_.bits_per_pixel != 32)
            throw std::runtime_error("Only 24/32-bit BMP supported");

        // Don't support compressed BMP
        if (dib_header_.compression != 0)
            throw std::runtime_error("Compressed BMP not supported");

        readPixels(file);
    } catch (const std::exception& e) {
        return false;
    }

    return true;
}

/**
 * @brief Reads BMP file headers
 * @param file Open file stream
 */
void BMPFile::readHeaders(std::ifstream& file) {
    file.read(reinterpret_cast<char*>(&bmp_header_), sizeof(BMPHeader));
    file.read(reinterpret_cast<char*>(&dib_header_), sizeof(DIBHeader));
}

/**
 * @brief Reads pixel data from file
 * @param file Open file stream positioned at start of pixel data
 */
void BMPFile::readPixels(std::ifstream& file) {
    file.seekg(bmp_header_.data_offset, std::ios::beg);

    const int w = width();
    const int h = height();
    pixels_.resize(w * h);

    const size_t row_size = getRowSize();
    std::vector<uint8_t> row(row_size);

    for (int y = 0; y < h; ++y) {
        file.read(reinterpret_cast<char*>(row.data()), row_size);

        for (int x = 0; x < w; ++x) {
            size_t pos = x * (is32bit() ? 4 : 3);
            Pixel& p = pixels_[index(x, rowIndex(y))];

            p.b = row[pos];
            p.g = row[pos + 1];
            p.r = row[pos + 2];
            if (is32bit()) p.a = row[pos + 3];
        }
    }
}

/**
 * @brief Saves BMP image to file
 * @param filename Path to save file
 * @return true if file saved successfully, false on error
 */
bool BMPFile::save(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) return false;

    try {
        writeHeaders(file);

        const int w = width();
        const int h = height();
        const size_t row_size = getRowSize();
        std::vector<uint8_t> row(row_size, 0);

        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                const Pixel& p = pixels_[index(x, rowIndex(y))];
                size_t pos = x * (is32bit() ? 4 : 3);

                row[pos] = p.b;
                row[pos + 1] = p.g;
                row[pos + 2] = p.r;
                if (is32bit()) row[pos + 3] = p.a;
            }
            file.write(reinterpret_cast<char*>(row.data()), row_size);
        }
    } catch (...) {
        return false;
    }

    return true;
}

/**
 * @brief Writes BMP headers to file
 * @param file File stream for writing
 */
void BMPFile::writeHeaders(std::ofstream& file) const {
    file.write(reinterpret_cast<const char*>(&bmp_header_), sizeof(BMPHeader));
    file.write(reinterpret_cast<const char*>(&dib_header_), sizeof(DIBHeader));
}

/**
 * @brief Gets pixel by coordinates
 * @param x X coordinate (0 to width-1)
 * @param y Y coordinate (0 to height-1)
 * @return Pixel structure with color
 * @throws std::out_of_range if coordinates are out of bounds
 */
BMPFile::Pixel BMPFile::getPixel(int x, int y) const {
    if (!inBounds(x, y)) throw std::out_of_range("Pixel out of range");
    return pixels_[index(x, y)];
}

/**
 * @brief Sets pixel color by coordinates
 * @param x X coordinate (0 to width-1)
 * @param y Y coordinate (0 to height-1)
 * @param pixel New pixel color
 * @throws std::out_of_range if coordinates are out of bounds
 */
void BMPFile::setPixel(int x, int y, Pixel pixel) {
    if (!inBounds(x, y)) throw std::out_of_range("Pixel out of range");
    pixels_[index(x, y)] = pixel;
}

/**
 * @brief Flips image vertically
 */
void BMPFile::flipVertically() {
    const int w = width();
    const int h = height();
    for (int y = 0; y < h / 2; ++y) {
        for (int x = 0; x < w; ++x) {
            std::swap(pixels_[index(x, y)], pixels_[index(x, h - 1 - y)]);
        }
    }
    dib_header_.height = -dib_header_.height;
}

/**
 * @brief Converts image to black and white
 */
void BMPFile::convertToBlackAndWhite() {
    for (Pixel& p : pixels_) {
        uint8_t brightness = static_cast<uint8_t>(0.299 * p.r + 0.587 * p.g + 0.114 * p.b);
        if (brightness > 127) {
            p = {255, 255, 255, p.a}; // white
        } else {
            p = {0, 0, 0, p.a}; // black
        }
    }
}

/**
 * @brief Creates a new blank BMP image
 * @param width Image width in pixels
 * @param height Image height in pixels
 * @param format Pixel format (BGR24 or BGRA32)
 * @param fill_color Pixel to fill the image with (default: black)
 */
void BMPFile::create(int width, int height, PixelFormat format, Pixel fill_color) {
    if (width <= 0 || height <= 0)
        throw std::invalid_argument("Invalid image dimensions");

    dib_header_ = DIBHeader{};
    dib_header_.width = width;
    dib_header_.height = -height; // top-down
    dib_header_.bits_per_pixel = (format == PixelFormat::BGRA32) ? 32 : 24;
    dib_header_.image_size = static_cast<uint32_t>(getRowSize() * height);
    
    bmp_header_ = BMPHeader{};
    bmp_header_.data_offset = sizeof(BMPHeader) + sizeof(DIBHeader);
    bmp_header_.file_size = bmp_header_.data_offset + dib_header_.image_size;

    pixels_.resize(width * height, fill_color);
}

/**
 * @brief Calculates row size with padding
 * @return Row size in bytes
 */
size_t BMPFile::getRowSize() const {
    size_t bytesPerPixel = is32bit() ? 4 : 3;
    return ((width() * bytesPerPixel + 3) & ~3);
}

/**
 * @brief Calculates index in pixel array
 * @param x X coordinate
 * @param y Y coordinate
 * @return Linear index in pixels_ array
 */
size_t BMPFile::index(int x, int y) const {
    return y * width() + x;
}

/**
 * @brief Checks if pixel coordinates are valid
 * @param x X coordinate
 * @param y Y coordinate
 * @return true if coordinates are within image bounds, false otherwise
 */
bool BMPFile::inBounds(int x, int y) const {
    return x >= 0 && x < width() && y >= 0 && y < height();
}

/**
 * @brief Adjusts Y coordinate according to image orientation
 * @param y Original Y coordinate
 * @return Adjusted row index
 */
int BMPFile::rowIndex(int y) const {
    return dib_header_.height > 0 ? height() - 1 - y : y;
}