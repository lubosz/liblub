#include "CLBuffer.h"
#include "System/Logger.h"

#include "OpenCL.h"

CLBuffer::CLBuffer(cl_context *context, cl_uint size, void * data) {
    this->size = size;
    this->data = data;
    allocate(context);
}

CLBuffer::~CLBuffer() {
    cl_int status = clReleaseMemObject(memory);
    if (status != CL_SUCCESS) {
        LogError << "Failed to release OpenCL pixel buffer: "<< status;
    }
}

void CLBuffer::allocate(cl_context * context) {
    cl_int status;
    memory = clCreateBuffer(
                *context,
                CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
                size,
                data,
                &status);
    if (status != CL_SUCCESS) {
        LogError << "Failed to create OpenCL pixel buffer:" << status;
    }
}
void CLBuffer::enqueueRead(cl_command_queue* commandQueue) {
    /* Enqueue readBuffer */
    cl_event event;
    cl_int status = clEnqueueReadBuffer(
                *commandQueue,
                memory,
                CL_TRUE,
                0,
                size,
                data,
                0,
                NULL,
                &event);
    if (status != CL_SUCCESS) {
        LogError << "Failed to read the OpenCL buffer:" << status;
    }
    OpenCL::waitForEvent(event);
}

void CLBuffer::enqueueWrite(cl_command_queue* commandQueue) {
    /* Enqueue writeBuffer */
    cl_event event;
    cl_int status = clEnqueueWriteBuffer(
                *commandQueue,
                memory,
                CL_TRUE,
                0,
                size,
                data,
                0,
                NULL,
                &event);
    if (status != CL_SUCCESS) {
        LogError << "Failed to write the OpenCL buffer:" << status;
    }
    OpenCL::waitForEvent(event);
}
