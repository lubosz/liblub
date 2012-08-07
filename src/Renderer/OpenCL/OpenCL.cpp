
#include "Scene/Scene.h"
#include "OpenCL.h"
#include <math.h>

OpenCL::OpenCL() {
}

cl_platform_id OpenCL::initPlattforms() {
    cl_uint numPlatforms;
    cl_platform_id platform = NULL;
    cl_int status = clGetPlatformIDs(0, NULL, &numPlatforms);
    if (status != CL_SUCCESS) {
        LogFatal << "Failed to get OpenCL platforms";
    } else {
        LogInfo << numPlatforms << "OpenCL Plattforms found";
    }

    if (numPlatforms > 0) {
        cl_platform_id *platforms = new cl_platform_id[numPlatforms];
        status = clGetPlatformIDs(numPlatforms, platforms, NULL);
        if (status != CL_SUCCESS) {
            LogError << "Failed to get OpenCL platform IDs";
        }

        for (unsigned i = 0; i < numPlatforms; ++i) {
            char pbuf[100];
            status = clGetPlatformInfo(platforms[i],
                                       CL_PLATFORM_VENDOR,
                                       sizeof(pbuf),
                                       pbuf,
                                       NULL);

            status = clGetPlatformIDs(numPlatforms, platforms, NULL);
            if (status != CL_SUCCESS) {
                LogError << "Failed to get OpenCL platform IDs";
            }
            LogInfo << "OpenCL Platform"<< i<< pbuf;
        }

        platform = platforms[0];
        delete platforms;
        return platform;
    }
}

void OpenCL::initDevices() {
    cl_int status;
    /* Get the size of device list data */
    status = clGetContextInfo(
                context,
                CL_CONTEXT_DEVICES,
                0,
                NULL,
                &deviceListSize);
    if (status != CL_SUCCESS) {
        LogError << "Failed to get OpenCL context info size:"<< status;
    }

    devices = new cl_device_id[deviceListSize];
    if (devices == NULL) {
        LogError << "Failed to allocate memory for OpenCL device list:" << status;
    }

    /* Get the device list data */
    status = clGetContextInfo(
                context,
                CL_CONTEXT_DEVICES,
                deviceListSize,
                devices,
                NULL);
    if (status != CL_SUCCESS) {
        LogError << "Failed to get OpenCL context info:"<< status;
    }
}

void OpenCL::createContext(cl_platform_id platform) {
    cl_int status;

    cl_context_properties cps[3] ={
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)platform,
        0
    };

    cl_context_properties *cprops = (NULL == platform) ? NULL : cps;

    cl_device_type dType = CL_DEVICE_TYPE_ALL;

    context = clCreateContextFromType(
                cprops,
                dType,
                NULL,
                NULL,
                &status);
    if (status != CL_SUCCESS) {
        LogError << "Failed to open OpenCL context";
    }
}

void OpenCL::printInfo() {
    cl_int status;
    /* Print devices list */
    unsigned int i;
    for (i = 0; i < deviceListSize / sizeof(cl_device_id); ++i) {
        cl_device_type type = 0;
        status = clGetDeviceInfo(devices[i],
                                 CL_DEVICE_TYPE,
                                 sizeof(cl_device_type),
                                 &type,
                                 NULL);
        if (status != CL_SUCCESS) {
            LogError << "Failed to get OpenCL device info:"<< status;
        }

        char name[256];
        status = clGetDeviceInfo(devices[i],
                                 CL_DEVICE_NAME,
                                 sizeof(char[256]),
                                 &name,
                                 NULL);
        if (status != CL_SUCCESS) {
            LogError << "Failed to get OpenCL device info:" << status;
        }

        LogInfo << "OpenCL Device" << i << "Name"<< name << "Type" << type;

        cl_uint units = 0;
        status = clGetDeviceInfo(devices[i],
                                 CL_DEVICE_MAX_COMPUTE_UNITS,
                                 sizeof(cl_uint),
                                 &units,
                                 NULL);
        if (status != CL_SUCCESS) {
            LogError << "Failed to get OpenCL device info: %d\n", status;
        }

        LogInfo << "OpenCL Device" << i << ": Compute units = "<< units;

        size_t gsize = 0;
        status = clGetDeviceInfo(devices[i],
                                 CL_DEVICE_MAX_WORK_GROUP_SIZE,
                                 sizeof(size_t),
                                 &gsize,
                                 NULL);
        if (status != CL_SUCCESS) {
            LogError << "Failed to get OpenCL device info:" << status;
        }

        LogInfo << "OpenCL Device" << i << ": Max. work group size =" << (unsigned int)gsize;
    }
}

void OpenCL::createCommandQueue() {
    cl_int status;
    cl_command_queue_properties prop = 0;
    commandQueue = clCreateCommandQueue(
                context,
                devices[0],
                prop,
                &status);
    if (status != CL_SUCCESS) {
        LogError << "Failed to create OpenCL command queue:" << status;
    }
}

void OpenCL::init() {
    cl_platform_id platform = initPlattforms();
    createContext(platform);
    initDevices();
    printInfo();
    createCommandQueue();
}

void OpenCL::waitForEvent(cl_event event) {
    cl_int status;
    /* Wait for read buffer to finish execution */
    status = clWaitForEvents(1, &event);
    if (status != CL_SUCCESS)
        LogError << "Failed to wait the read of OpenCL buffer:" << status;
    clReleaseEvent(event);
}

