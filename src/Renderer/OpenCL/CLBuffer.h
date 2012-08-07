#ifndef CLBUFFER_H
#define CLBUFFER_H

#include <CL/cl.h>

class CLBuffer {
 public:
    CLBuffer(cl_context * context, cl_uint size, void * data);
    ~CLBuffer();
    void allocate(cl_context * context);
    cl_mem memory;
    void * data;
    unsigned size;

    void enqueueWrite(cl_command_queue* commandQueue);
    void enqueueRead(cl_command_queue* commandQueue);
};

#endif // CLBUFFER_H
