#include <cstdio>
#include <cstdlib>
#include "cli.h"


// OpenCL includes
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#elif __linux
#include <CL/cl.h>
#else
#error Platform not supported
#endif

using namespace std;

CLI * cliInitialize()
{
    CLI *cli =  (CLI*) malloc(sizeof(CLI));

    //-----------------------------------------------------
    // STEP 1: Discover and initialize the platforms
    //-----------------------------------------------------
    // Use clGetPlatformIDs() to retrieve the number of 
    // platforms
    cli->status = clGetPlatformIDs(0, NULL, &cli->numPlatforms);
    // Allocate enough space for each platform
    cli->platforms = (cl_platform_id*)malloc(cli->numPlatforms*sizeof(cl_platform_id));
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
            cli->numDevices*sizeof(cl_device_id));

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
