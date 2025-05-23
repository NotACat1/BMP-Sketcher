#include "Strategy/DrawCrossThreadStrategy.hpp"

DrawCrossThreadStrategy::DrawCrossThreadStrategy(BMPFile::Pixel color, unsigned int thickness)
    : color_(color), thickness_(std::max(1u, thickness)) {}

void DrawCrossThreadStrategy::draw(BMPFile& image) {
    const int width = image.width();
    const int height = image.height();
    
    // Draw cross
    drawLine(image, 0, 0, width - 1, height - 1); // Vertical
    drawLine(image, 0, height - 1, width - 1, 0); // Horizontal
}

std::string DrawCrossThreadStrategy::getName() const {
    return "Cross Drawing Strategy (Thread-based)";
}

void DrawCrossThreadStrategy::setColor(const BMPFile::Pixel& color) {
    color_ = color;
}

BMPFile::Pixel DrawCrossThreadStrategy::getColor() const {
    return color_;
}

void DrawCrossThreadStrategy::setThickness(unsigned int thickness) {
    thickness_ = std::max(1u, thickness);
}

unsigned int DrawCrossThreadStrategy::getThickness() const {
    return thickness_;
}

void DrawCrossThreadStrategy::drawLine(BMPFile& image, int x0, int y0, int x1, int y1) {
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
    const int ystep = (y0 < y1) ? 1 : -1;
    
    // Determine optimal number of threads
    const unsigned num_threads = std::max(1u, std::thread::hardware_concurrency());
    const int chunk_size = std::max(1, dx / static_cast<int>(num_threads));
    
    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    
    for (unsigned t = 0; t < num_threads; ++t) {
        const int start = x0 + t * chunk_size;
        const int end = (t == num_threads - 1) ? x1 : start + chunk_size;
        
        threads.emplace_back([=, &image]() {
            int y = y0;
            int error = dx / 2;
            
            for (int x = start; x <= end; x++) {
                drawThickPixelArea(image, steep ? y : x, steep ? x : y);
                error -= dy;
                if (error < 0) {
                    y += ystep;
                    error += dx;
                }
            }
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
}

void DrawCrossThreadStrategy::drawThickPixel(BMPFile& image, int x, int y) {
    try {
        image.setPixel(x, y, color_);
    } catch (const std::out_of_range&) {
        // Silently ignore out-of-bounds pixels
    }
}

void DrawCrossThreadStrategy::drawThickPixelArea(BMPFile& image, int x, int y) {
    if (thickness_ == 1) {
        drawThickPixel(image, x, y);
        return;
    }
    
    const int half = thickness_ / 2;
    for (int dy = -half; dy <= half; ++dy) {
        for (int dx = -half; dx <= half; ++dx) {
            drawThickPixel(image, x + dx, y + dy);
        }
    }
}