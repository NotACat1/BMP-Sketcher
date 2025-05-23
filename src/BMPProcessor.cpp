#include "BMPProcessor.hpp"
#include <iostream>
#include <sstream>
#include <getopt.h>
#include <cstring>
#include <iomanip>

BMPProcessor::Config BMPProcessor::Config::parse(int argc, char* argv[]) {
    Config config;

    const struct option long_options[] = {
        {"input", required_argument, nullptr, 'i'},
        {"output", required_argument, nullptr, 'o'},
        {"thickness", required_argument, nullptr, 't'},
        {"color", required_argument, nullptr, 'c'},
        {"display", required_argument, nullptr, 'd'},
        {"strategy", required_argument, nullptr, 's'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, 0, nullptr, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "i:o:t:c:d:s:h", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'i':
                config.input_file = optarg;
                break;
            case 'o':
                config.output_file = optarg;
                break;
            case 't':
                config.thickness = std::max(1u, static_cast<unsigned int>(std::stoul(optarg)));
                break;
            case 'c': {
                std::istringstream iss(optarg);
                std::string token;
                int rgba[4] = {0, 0, 0, 255};
                int i = 0;
                while (std::getline(iss, token, ',') && i < 4) {
                    rgba[i++] = std::stoi(token);
                }
                config.color = {static_cast<uint8_t>(rgba[0]),
                                static_cast<uint8_t>(rgba[1]),
                                static_cast<uint8_t>(rgba[2]),
                                static_cast<uint8_t>(rgba[3])};
                break;
            }
            case 'd':
                if (std::strlen(optarg) >= 2) {
                    config.display_chars = {optarg[0], optarg[1]};
                }
                break;
            case 's':
                config.strategy_name = optarg;
                if (config.strategy_name == "none") {
                    config.strategy_type = DrawStrategyFactory::StrategyType::NONE;
                } else if (config.strategy_name == "openmp") {
                    config.strategy_type = DrawStrategyFactory::StrategyType::OPENMP;
                } else if (config.strategy_name == "thread") {
                    config.strategy_type = DrawStrategyFactory::StrategyType::THREAD;
                } else {
                    throw std::runtime_error("Unknown strategy: " + config.strategy_name);
                }
                break;
            case 'h':
                printHelp(argv[0]);
                std::exit(0);
            default:
                throw std::runtime_error("Invalid arguments. Use --help for usage.");
        }
    }

    if (config.input_file.empty()) {
        throw std::runtime_error("Input file is required. Use --input or -i.");
    }

    return config;
}

void BMPProcessor::Config::printHelp(const std::string& program_name) {
    const std::string indent(4, ' ');
    
    std::cout << "BMP Image Processor - Tool for processing BMP images with various drawing strategies\n\n"
              << "Usage:\n"
              << indent << program_name << " -i <input.bmp> [OPTIONS]\n\n"
              << "Required arguments:\n"
              << indent << std::left << std::setw(20) << "-i, --input <file>" 
              << "Input BMP image file path\n\n"
              << "Optional arguments:\n"
              << indent << std::left << std::setw(20) << "-o, --output <file>" 
              << "Output BMP file path (default: output.bmp)\n"
              << indent << std::left << std::setw(20) << "-t, --thickness <n>" 
              << "Drawing thickness in pixels (default: 1)\n"
              << indent << std::left << std::setw(20) << "-c, --color R,G,B[,A]" 
              << "Drawing color in RGBA format (default: 0,0,0,255)\n"
              << indent << std::left << std::setw(20) << "-d, --display XY" 
              << "Characters for console display (foreground X, background Y) (default: \"# \")\n"
              << indent << std::left << std::setw(20) << "-s, --strategy <name>" 
              << "Drawing strategy: none, openmp, thread (default: none)\n"
              << indent << std::left << std::setw(20) << "-h, --help" 
              << "Show this help message and exit\n\n"
              << "Examples:\n"
              << indent << program_name << " -i image.bmp -o result.bmp -t 3 -c 255,0,0 -s openmp\n"
              << indent << program_name << " -i drawing.bmp --color 0,128,255,200 --display \"@.\"\n";
}

BMPProcessor::BMPProcessor(const Config& config, std::unique_ptr<IDrawStrategy> strategy)
    : config_(config), draw_strategy_(std::move(strategy)) 
{
    if (draw_strategy_) {
        draw_strategy_->setColor(config_.color);
        draw_strategy_->setThickness(config_.thickness);
    }
}

bool BMPProcessor::process() {
    try {
        bmp_.load(config_.input_file);
        //bmp_.convertToBlackAndWhite();        
        if (draw_strategy_) {
            draw_strategy_->draw(bmp_);
        }
        bmp_.convertToBlackAndWhite();
        bmp_.save(config_.output_file);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

void BMPProcessor::display() const {
    int width = bmp_.width();
    int height = bmp_.height();
    auto [on_char, off_char] = config_.display_chars;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            try {
                auto px = bmp_.getPixel(x, y);
                uint8_t brightness = static_cast<uint8_t>(0.299 * px.r + 0.587 * px.g + 0.114 * px.b);
                std::cout << (brightness > 127 ? on_char : off_char);
            } catch (const std::out_of_range&) {
                std::cout << '?';
            }
        }
        std::cout << '\n';
    }
}