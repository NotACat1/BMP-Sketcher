#include "example.hpp"
#include <iostream>

int main() {
    std::cout << bmp::hello() << std::endl;
    bmp::parallel_hello();
    bmp::opencl_dummy();
    return 0;
}