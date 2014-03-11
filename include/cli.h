#ifndef CLI_H
#define CLI_H

#include <cstdio>
#include <cstdlib>
#include <string.h>

// OpenCL includes
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#elif __linux
#include <CL/cl.h>
#else
#error Platform not supported
#endif

typedef struct cli_s
{
	// internal status to check the output of each API call
    cl_int status;
    //initialize our data we're keeping track of
    // used for  1: Discover and initialize the platforms
    cl_uint numPlatforms;
    cl_platform_id *platforms;

    // used for 2: Discover and initialize the devices
    cl_uint numDevices;
    cl_device_id *devices;

    // used for 3: Create a context
    cl_context context;

    // used for 4: Create a command queue
    cl_command_queue cmdQueue;

    // used for 7: Create and compile the program     
    cl_program program;
   
    // used for 8: Create the kernel
    cl_kernel kernel;

} CLI;


extern CLI* cliInitialize();

extern void cliBuild (CLI* cli, const char* programSource, const char * kernel_name);

extern void cliRelease(CLI* cli);

extern void clStatus(cl_int err, char*stat);

#endif