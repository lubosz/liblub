#ifndef OPENCL_H
#define OPENCL_H

#include <CL/cl.h>
#include <string>

using std::string;

#include "CLBuffer.h"
#include "CLKernel.h"

class OpenCL {
 public:
    OpenCL();

    cl_command_queue commandQueue;
    cl_context context;
    cl_device_id *devices = NULL;
    size_t deviceListSize;

    void updateCamera();
    void init();
    cl_platform_id initPlattforms();
    void initDevices();
    void createContext(cl_platform_id platform);
    void printInfo();
    void createCommandQueue();
    static void waitForEvent(cl_event event);
};

#endif // OPENCL_H
