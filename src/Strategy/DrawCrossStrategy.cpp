#include "Strategy/DrawCrossStrategy.hpp"

DrawCrossStrategy::DrawCrossStrategy(BMPFile::Pixel color, unsigned int thickness)
    : color_(color), thickness_(std::max(1u, thickness)) {}

void DrawCrossStrategy::draw(BMPFile& image) {
    const int width = image.width();
    const int height = image.height();
    
    // Draw cross
    drawLine(image, 0, 0, width - 1, height - 1); // Vertical
    drawLine(image, 0, height - 1, width - 1, 0); // Horizontal
}

std::string DrawCrossStrategy::getName() const {
    return "Cross Drawing Strategy (Single-threaded)";
}

void DrawCrossStrategy::setColor(const BMPFile::Pixel& color) {
    color_ = color;
}

BMPFile::Pixel DrawCrossStrategy::getColor() const {
    return color_;
}

void DrawCrossStrategy::setThickness(unsigned int thickness) {
    thickness_ = std::max(1u, thickness);
}

unsigned int DrawCrossStrategy::getThickness() const {
    return thickness_;
}

void DrawCrossStrategy::drawLine(BMPFile& image, int x0, int y0, int x1, int y1) {
    bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);
    
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    
    const int dx = x1 - x0;
    const int dy = std::abs(y1 - y0);
    int error = dx / 2;
    const int ystep = (y0 < y1) ? 1 : -1;
    int y = y0;
    
    for (int x = x0; x <= x1; x++) {
        drawThickPixel(image, steep ? y : x, steep ? x : y);
        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
}

void DrawCrossStrategy::drawThickPixel(BMPFile& image, int x, int y) {
    if (thickness_ == 1) {
        try {
            image.setPixel(x, y, color_);
        } catch (const std::out_of_range&) {
            // Silently ignore out-of-bounds pixels
        }
        return;
    }
    
    const int half = thickness_ / 2;
    for (int dy = -half; dy <= half; ++dy) {
        for (int dx = -half; dx <= half; ++dx) {
            try {
                image.setPixel(x + dx, y + dy, color_);
            } catch (const std::out_of_range&) {
                // Silently ignore out-of-bounds pixels
            }
        }
    }
}