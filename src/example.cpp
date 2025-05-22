#include "example.hpp"
#include <iostream>
#include <omp.h>
#include <CL/cl.h>

namespace bmp {

std::string hello() {
    return "BMP Library is working!";
}

void parallel_hello() {
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        printf("Hello from thread %d\n", id);
    }
}

void opencl_dummy() {
    cl_uint platformCount;
    clGetPlatformIDs(0, nullptr, &platformCount);
    printf("OpenCL platforms available: %u\n", platformCount);
}

}