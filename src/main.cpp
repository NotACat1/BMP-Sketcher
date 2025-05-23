#include <iostream>
#include "BMPProcessor.hpp"
#include "DrawStrategyFactory.hpp"

/**
 * @brief Main entry point for BMP image processing application
 * 
 * Parses command line arguments, configures and executes image processing pipeline
 * 
 * @param argc Argument count
 * @param argv Argument values
 * @return int Program exit status (0 for success, non-zero for errors)
 */
int main(int argc, char* argv[]) {
    try {
        // 1. Parse command line arguments
        BMPProcessor::Config config = BMPProcessor::Config::parse(argc, argv);
        
        // 2. Create and configure drawing strategy
        auto strategy = DrawStrategyFactory::create(config.strategy_type);
        strategy->setColor(config.color);
        strategy->setThickness(config.thickness);

        // 3. Initialize processor with configuration and strategy
        BMPProcessor processor(config, std::move(strategy));

        // 4. Execute image processing pipeline
        if (processor.process()) {
            std::cout << "Success: Image processed and saved to '" 
                      << config.output_file << "'\n";
            
            // 5. Display processed image if successful
            processor.display();
        } else {
            std::cerr << "Error: Failed to process image\n";
            return EXIT_FAILURE;
        }

    } catch (const std::exception& e) {
        // Handle errors and display usage help
        std::cerr << "\nError: " << e.what() << "\n\n";
        BMPProcessor::Config::printHelp(argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}