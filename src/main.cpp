#include "BMPFile.hpp"
#include <iostream>

int main() {
    BMPFile image;

    BMPFile bmp;
bmp.create(640, 480, BMPFile::PixelFormat::BGRA32, BMPFile::Pixel(255, 0, 0)); // красный фон
bmp.save("/home/notacat/Изображения/red_image.bmp");

    // Загрузка изображения
    if (!image.load("/home/notacat/Изображения/input.bmp")) {
        std::cerr << "Ошибка при загрузке input.bmp\n";
        return 1;
    }

    std::cout << "Файл загружен: " << image.width() << "x" << image.height()
              << (image.is32bit() ? " (32-bit)" : " (24-bit)") << '\n';

    // Преобразование в чёрно-белое
    image.convertToBlackAndWhite();

    // Сохранение результата
    if (!image.save("/home/notacat/Изображения/output_bw.bmp")) {
        std::cerr << "Ошибка при сохранении output_bw.bmp\n";
        return 1;
    }

    std::cout << "Сохранено: output_bw.bmp\n";
    
    return 0;
}