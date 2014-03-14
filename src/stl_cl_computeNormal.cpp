
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>

#include "cli.h"
#include "stl_cl_computeNormal.h"

// OpenCL includes
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#elif __linux
#include <CL/cl.h>
#else
#error Platform not supported
#endif

using namespace std;

// this kernel takes ~16 FLOPS
// Global memory reads: 9*4 (vi) = 36 BYTES
// Global memory writes: 3*4 (verto) = 12 BYTES

/// (GDDR5 can transfer at most 32 BYTES per clock)


cl_int stlclComputeNormal(
    std::vector<float> &verticies, 
    float *normalBuffer, 
    std::vector<cl_int> &cliStati,
    CLI * cli)
{
    unsigned int nVerticies = verticies.size();

    //check if there are correct number of verticies.
    //if( fmod((float) nVerticies, 3.0)  != 0 )   
    //{
    //    printf("Error num vertex is incorrect\n");
    //    exit(1); //change this
    //}

    //size_t vertexBytes = sizeof(float)*12;
    size_t vertexBytes = nVerticies * sizeof(float);

    size_t normalBytes = nVerticies * sizeof(float)/3;

    //initalize CL interface and build kernel
    // declare CL memory buffers
    cl_mem bufferA;
    cl_mem bufferC;

    // Use clCreateBuffer() to create a buffer object (d_A) 
    // that will contain the data from the host array 
    bufferA = clCreateBuffer(
        cli->context, 
        CL_MEM_READ_ONLY,                         
        vertexBytes, 
        NULL, 
        &cli->status);
    cliStati.push_back(cli->status);

    // Use clCreateBuffer() to create a buffer object (d_C) 
    // with enough space to hold the output data
    bufferC = clCreateBuffer(
        cli->context, 
        CL_MEM_WRITE_ONLY,                 
        normalBytes, 
        NULL, 
        &cli->status);
    cliStati.push_back(cli->status);

        
    // -- prototype copied here for reference --
    //cl_int clEnqueueWriteBuffer (   cl_command_queue command_queue,
    //    cl_mem buffer,
    //    cl_bool blocking_write,
    //    size_t offset,
    //    size_t cb,
    //    const void *ptr,
    //    cl_uint num_events_in_wait_list,
    //    const cl_event *event_wait_list,
    //    cl_event *event)
    //

    // Use clEnqueueWriteBuffer() to write input array A to
    // the device buffer bufferA
    cli->status = clEnqueueWriteBuffer(
        cli->cmdQueue, 
        bufferA, 
        CL_FALSE,       //non-blocking buffer to device
        0, 
        vertexBytes,                         
        verticies.data(), 
        0, 
        NULL, 
        NULL);
    cliStati.push_back(cli->status);

    // Associate the input and output buffers with the 
    // kernel 
    // using clSetKernelArg()
    cli->status  = clSetKernelArg(
        cli->kernel, 
        0, 
        sizeof(cl_mem), 
        &bufferA);

    cliStati.push_back(cli->status);


    cli->status = clSetKernelArg(
        cli->kernel, 
        1, 
        sizeof(cl_mem), 
        &bufferC);

    cliStati.push_back(cli->status);


    //if( fmod(nVerticies, 9.0)  != 0 )
    //{
    //    printf("Worksize Error\n");
    //    exit(1); //change this
    //}

    // Define an index space (global work size) of work 
    // items for 
    // execution. A workgroup size (local work size) is not 
    // required, 
    // but can be used.
    size_t globalWorkSize[1];
    // There are 'elements' work-items 
    globalWorkSize[0] = nVerticies/9;

    // STEP 11: Enqueue the kernel for execution
    
    // Execute the kernel by using 
    // clEnqueueNDRangeKernel().
    // 'globalWorkSize' is the 1D dimension of the 
    // work-items
    cli->status = clEnqueueNDRangeKernel(
        cli->cmdQueue, 
        cli->kernel, 
        1, 
        NULL, 
        globalWorkSize, 
        NULL, 
        0, 
        NULL, 
        NULL);

    cliStati.push_back(cli->status);

    // STEP 12: Read the output buffer back to the host
    
    // cl_int clEnqueueReadBuffer ( cl_command_queue command_queue,
    //      cl_mem buffer,
    //      cl_bool blocking_read,
    //      size_t offset,
    //      size_t cb,
    //      void *ptr,
    //      cl_uint num_events_in_wait_list,
    //      const cl_event *event_wait_list,
    //      cl_event *event)
    //

    clEnqueueReadBuffer(
        cli->cmdQueue, 
        bufferC, 
        CL_TRUE, 
        0, 
        normalBytes, 
        normalBuffer, 
        0, 
        NULL, 
        NULL);


    // Free OpenCL resources
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferC);

    // Free host resources

    return cli->status;
}
