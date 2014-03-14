#include <cstdio>
#include <cstdlib>
#include <string.h>
#include "cli.h"


// OpenCL includes
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#elif __linux
#include <CL/cl.h>
#else
#error Platform not supported
#endif

#define STATUS_CHAR_SIZE 35

using namespace std;


CLI * cliInitialize()
{
    CLI *cli =  (CLI*) malloc( sizeof(CLI)) ;

    //-----------------------------------------------------
    // STEP 1: Discover and initialize the platforms
    //-----------------------------------------------------
    // Use clGetPlatformIDs() to retrieve the number of 
    // platforms
    cli->status = clGetPlatformIDs(0, NULL, &cli->numPlatforms);
    // Allocate enough space for each platform
    cli->platforms = (cl_platform_id*)malloc(cli->numPlatforms * sizeof(cl_platform_id));
    // Fill in platforms with clGetPlatformIDs()
    cli->status = clGetPlatformIDs(cli->numPlatforms, cli->platforms, 
                NULL);

    //-----------------------------------------------------
    // STEP 2: Discover and initialize the devices
    //----------------------------------------------------- 
    // Use clGetDeviceIDs() to retrieve the number of 
    // devices present
    cli->status = clGetDeviceIDs(
        cli->platforms[0], 
        CL_DEVICE_TYPE_ALL, 
        0, 
        NULL, 
        &cli->numDevices);

    // Allocate enough space for each device
    cli->devices = 
        (cl_device_id*)malloc(
            cli->numDevices * sizeof(cl_device_id));

    // Fill in devices with clGetDeviceIDs()
    cli->status = clGetDeviceIDs(
        cli->platforms[0], 
        CL_DEVICE_TYPE_ALL,
        cli->numDevices, 
        cli->devices, 
        NULL);

    //-----------------------------------------------------
    // STEP 3: Create a context
    //----------------------------------------------------- 
    
    // Create a context using clCreateContext() and 
    // associate it with the devices
    cli->context = clCreateContext(
        NULL, 
        cli->numDevices, 
        cli->devices, 
        NULL, 
        NULL, 
        &cli->status);

    //-----------------------------------------------------
    // STEP 4: Create a command queue
    //----------------------------------------------------- 
    // Create a command queue using clCreateCommandQueue(),
    // and associate it with the device you want to execute 
    // on
    cli->cmdQueue = clCreateCommandQueue(
        cli->context, 
        cli->devices[0], 
        0, 
        &cli->status);

    return cli;
}

// special function for buffering and setting kernel arguments
void cliKernelArgsSet(
    void* ptr,              //I want to restrict this
    size_t bufferBytes,  
    int argn, 
    cl_mem_flags memflag,
    CLI* cli)
{
    cli->clMemDes[argn] = clCreateBuffer(
        cli->context,
        memflag,
        bufferBytes,
        NULL,
        &cli->status);

    if(memflag == CL_MEM_READ_ONLY)
        cli->status = clEnqueueWriteBuffer(
            cli->cmdQueue,
            cli->clMemDes[argn],
            CL_FALSE,
            0,
            bufferBytes,
            ptr,
            0,
            NULL,
            NULL);

    cli->status = clSetKernelArg(
        cli->kernel,
        argn,
        sizeof(cli->clMemDes[argn]),
        &ptr);
    return;
}

void cliBuild (CLI* cli, const char* programSource, const char * kernel_name)
{

    //-----------------------------------------------------
    // STEP 7: Create and compile the program
    //----------------------------------------------------- 

    // Create a program using clCreateProgramWithSource()
    cli->program = clCreateProgramWithSource(
        cli->context, 
        1, 
        (const char**)&programSource,                                 
        NULL, 
        &cli->status);

    // Build (compile) the program for the devices with
    // clBuildProgram()
    cli->status = clBuildProgram(
        cli->program, 
        cli->numDevices, 
        cli->devices, 
        NULL, 
        NULL, 
        NULL);
   
    //-----------------------------------------------------
    // STEP 8: Create the kernel
    //----------------------------------------------------- 
    // Use clCreateKernel() to create a kernel from the 
    // vector addition function (named "vecadd")
    cli->kernel = clCreateKernel(cli->program, kernel_name , &cli->status);
}

void cliRelease(CLI* cli)
{
	clReleaseKernel(cli->kernel);
    clReleaseProgram(cli->program);
    clReleaseCommandQueue(cli->cmdQueue);
    clReleaseContext(cli->context);
    free(cli->platforms);
    free(cli->devices);

}

void cliStatus(const cl_int err, char* stat)
{
    //printing is inefficient anyway
    const char zero[STATUS_CHAR_SIZE] = {};
    strcpy(stat,zero);
    switch (err) 
    {
        case CL_SUCCESS:                            strcpy(stat, "Success!"); break;
        case CL_DEVICE_NOT_FOUND:                   strcpy(stat, "Device not found."); break;
        case CL_DEVICE_NOT_AVAILABLE:               strcpy(stat, "Device not available"); break;
        case CL_COMPILER_NOT_AVAILABLE:             strcpy(stat, "Compiler not available"); break;
        case CL_MEM_OBJECT_ALLOCATION_FAILURE:      strcpy(stat, "Memory object allocation failure"); break;
        case CL_OUT_OF_RESOURCES:                   strcpy(stat, "Out of resources"); break;
        case CL_OUT_OF_HOST_MEMORY:                 strcpy(stat, "Out of host memory"); break;
        case CL_PROFILING_INFO_NOT_AVAILABLE:       strcpy(stat, "Profiling information not available"); break;
        case CL_MEM_COPY_OVERLAP:                   strcpy(stat, "Memory copy overlap"); break;
        case CL_IMAGE_FORMAT_MISMATCH:              strcpy(stat, "Image format mismatch"); break;
        case CL_IMAGE_FORMAT_NOT_SUPPORTED:         strcpy(stat, "Image format not supported"); break;
        case CL_BUILD_PROGRAM_FAILURE:              strcpy(stat, "Program build failure"); break;
        case CL_MAP_FAILURE:                        strcpy(stat, "Map failure"); break;
        case CL_INVALID_VALUE:                      strcpy(stat, "Invalid value"); break;
        case CL_INVALID_DEVICE_TYPE:                strcpy(stat, "Invalid device type"); break;
        case CL_INVALID_PLATFORM:                   strcpy(stat, "Invalid platform"); break;
        case CL_INVALID_DEVICE:                     strcpy(stat, "Invalid device"); break;
        case CL_INVALID_CONTEXT:                    strcpy(stat, "Invalid context"); break;
        case CL_INVALID_QUEUE_PROPERTIES:           strcpy(stat, "Invalid queue properties"); break;
        case CL_INVALID_COMMAND_QUEUE:              strcpy(stat, "Invalid command queue"); break;
        case CL_INVALID_HOST_PTR:                   strcpy(stat, "Invalid host pointer"); break;
        case CL_INVALID_MEM_OBJECT:                 strcpy(stat, "Invalid memory object"); break;
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:    strcpy(stat, "Invalid image format descriptor"); break;
        case CL_INVALID_IMAGE_SIZE:                 strcpy(stat, "Invalid image size"); break;
        case CL_INVALID_SAMPLER:                    strcpy(stat, "Invalid sampler"); break;
        case CL_INVALID_BINARY:                     strcpy(stat, "Invalid binary"); break;
        case CL_INVALID_BUILD_OPTIONS:              strcpy(stat, "Invalid build options"); break;
        case CL_INVALID_PROGRAM:                    strcpy(stat, "Invalid program"); break;
        case CL_INVALID_PROGRAM_EXECUTABLE:         strcpy(stat, "Invalid program executable"); break;
        case CL_INVALID_KERNEL_NAME:                strcpy(stat, "Invalid kernel name"); break;
        case CL_INVALID_KERNEL_DEFINITION:          strcpy(stat, "Invalid kernel definition"); break;
        case CL_INVALID_KERNEL:                     strcpy(stat, "Invalid kernel"); break;
        case CL_INVALID_ARG_INDEX:                  strcpy(stat, "Invalid argument index"); break;
        case CL_INVALID_ARG_VALUE:                  strcpy(stat, "Invalid argument value"); break;
        case CL_INVALID_ARG_SIZE:                   strcpy(stat, "Invalid argument size"); break;
        case CL_INVALID_KERNEL_ARGS:                strcpy(stat, "Invalid kernel arguments"); break;
        case CL_INVALID_WORK_DIMENSION:             strcpy(stat, "Invalid work dimension"); break;
        case CL_INVALID_WORK_GROUP_SIZE:            strcpy(stat, "Invalid work group size"); break;
        case CL_INVALID_WORK_ITEM_SIZE:             strcpy(stat, "Invalid work item size"); break;
        case CL_INVALID_GLOBAL_OFFSET:              strcpy(stat, "Invalid global offset"); break;
        case CL_INVALID_EVENT_WAIT_LIST:            strcpy(stat, "Invalid event wait list"); break;
        case CL_INVALID_EVENT:                      strcpy(stat, "Invalid event"); break;
        case CL_INVALID_OPERATION:                  strcpy(stat, "Invalid operation"); break;
        case CL_INVALID_GL_OBJECT:                  strcpy(stat, "Invalid OpenGL object"); break;
        case CL_INVALID_BUFFER_SIZE:                strcpy(stat, "Invalid buffer size"); break;
        case CL_INVALID_MIP_LEVEL:                  strcpy(stat, "Invalid mip-map level"); break;
    }
    return; 
}

void PrintCLIStatus(cl_int err)
{
    char tmp[STATUS_CHAR_SIZE];
    cliStatus(err, tmp);
    //if(err)
        printf("%s\n", tmp);
}
