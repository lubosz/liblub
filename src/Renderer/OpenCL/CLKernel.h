#ifndef CLKERNEL_H
#define CLKERNEL_H

#include <CL/cl.h>
#include <string>

#include "renderconfig.h"

#include "CLBuffer.h"

using std::string;

class CLKernel
{
public:
    CLKernel(string fileName);

    cl_kernel kernel;
    cl_program program;

    string fileName;
    unsigned int workGroupSize = 1;

    string readSources();
    void setAccumulationArg(const int enableAccumulation, const float x, const float y);

    void createProgram(cl_context *context);
    void createKernel(cl_device_id *devices, string name);
    void buildProgram(cl_device_id *devices);

    void enqueueRangeKernel(size_t* globalThreads, size_t* localThreads, cl_command_queue* commandQueue);

    void setArg(unsigned position, unsigned size, void * data);
};

#endif // CLKERNEL_H
