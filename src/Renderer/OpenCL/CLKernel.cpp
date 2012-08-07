#include "CLKernel.h"

#include <QFile>
#include "System/Logger.h"
#include "OpenCL.h"

CLKernel::CLKernel(string fileName) {
    this->fileName = fileName;
}

string CLKernel::readSources() {
    QFile file(QString::fromStdString(fileName));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        LogError << "Can not open File." << fileName;
    QString source = file.readAll();
    return source.toStdString();
}

void CLKernel::setArg(unsigned position, unsigned size, void * data) {
    cl_int status = clSetKernelArg(
                kernel,
                position,
                size,
                data);
    if (status != CL_SUCCESS) {
        LogError << "Failed to set OpenCL arg. #" << position << status;
    }
}

void CLKernel::setAccumulationArg(int enableAccumulation, float x, float y) {
    setArg(2, sizeof(int), &enableAccumulation);
    setArg(3, sizeof(float), &x);
    setArg(4, sizeof(float), &y);
}

void CLKernel::createProgram(cl_context * context) {
    cl_int status;
    string sources = readSources();
    const char * sourcesChar = sources.c_str();
    program = clCreateProgramWithSource(
                *context,
                1,
                &sourcesChar,
                NULL,
                &status);
    if (status != CL_SUCCESS) {
        LogError << "Failed to open OpenCL kernel sources:" << status;
    }
}

void CLKernel::createKernel(cl_device_id *devices, string name) {
    cl_int status;
    kernel = clCreateKernel(program, name.c_str(), &status);
    if (status != CL_SUCCESS) {
        LogError << "Failed to create OpenCL kernel:" << status;
    }

    size_t gsize = 0;
    status = clGetKernelWorkGroupInfo(kernel,
                                      devices[0],
                                      CL_KERNEL_WORK_GROUP_SIZE,
                                      sizeof(size_t),
                                      &gsize,
                                      NULL);
    if (status != CL_SUCCESS) {
        LogError << "Failed to get OpenCL kernel work group size info:" << status;
    }

    workGroupSize = (unsigned int) gsize;
    LogInfo << "OpenCL Device 0: kernel work group size =" << workGroupSize;
}

void CLKernel::buildProgram(cl_device_id *devices) {
    cl_int status = clBuildProgram(program, 1, devices, "-I.", NULL, NULL);
    if (status != CL_SUCCESS) {
        LogError << "Failed to build OpenCL kernel:" << status;

        size_t retValSize;
        status = clGetProgramBuildInfo(
                    program,
                    devices[0],
                    CL_PROGRAM_BUILD_LOG,
                    0,
                    NULL,
                    &retValSize);
        if (status != CL_SUCCESS) {
            LogError << "Failed to get OpenCL kernel info size:" << status;
        }

        char *buildLog = (char *)malloc(retValSize + 1);
        status = clGetProgramBuildInfo(
                    program,
                    devices[0],
                    CL_PROGRAM_BUILD_LOG,
                    retValSize,
                    buildLog,
                    NULL);
        if (status != CL_SUCCESS) {
            LogError << "Failed to get OpenCL kernel info:" << status;
        }
        buildLog[retValSize] = '\0';

        LogError << "OpenCL Programm Build Log:"<< buildLog;
    }
}

void CLKernel::enqueueRangeKernel(size_t* globalThreads, size_t* localThreads, cl_command_queue* commandQueue) {
    cl_event event;
    cl_int status = clEnqueueNDRangeKernel(
                *commandQueue,
                kernel,
                1,
                NULL,
                globalThreads,
                localThreads,
                0,
                NULL,
                &event);
    if (status != CL_SUCCESS) {
        LogError <<  "Failed to enqueue OpenCL work:" << status;
    }
    OpenCL::waitForEvent(event);
}
