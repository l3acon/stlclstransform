
// compute STL normal vectors for
// given verticies on GPU
// 

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

void stlclComputeNormal(
    std::vector<float> &verticies, 
    float *normalBuffer, 
    CLI * cli,
    std::vector<cl_int> &errors)
{
    cl_int localstatus;
    unsigned int nVerticies = verticies.size();


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
        &localstatus);
    errors.push_back(localstatus);

    // Use clCreateBuffer() to create a buffer object (d_C) 
    // with enough space to hold the output data
    bufferC = clCreateBuffer(
        cli->context, 
        CL_MEM_WRITE_ONLY,                 
        normalBytes, 
        NULL, 
        &localstatus);
    errors.push_back(localstatus);

    // Use clEnqueueWriteBuffer() to write input array A to
    // the device buffer bufferA
    localstatus = clEnqueueWriteBuffer(
        cli->cmdQueue, 
        bufferA, 
        CL_FALSE,       //non-blocking buffer to device
        0, 
        vertexBytes,                         
        verticies.data(), 
        0, 
        NULL, 
        NULL);
    errors.push_back(localstatus);

    // Associate the input and output buffers with the 
    // kernel 
    // using clSetKernelArg()
    localstatus  = clSetKernelArg(
        cli->kernel, 
        0, 
        sizeof(cl_mem), 
        &bufferA);

    errors.push_back(localstatus);


    localstatus = clSetKernelArg(
        cli->kernel, 
        1, 
        sizeof(cl_mem), 
        &bufferC);

    errors.push_back(localstatus);


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
    localstatus = clEnqueueNDRangeKernel(
        cli->cmdQueue, 
        cli->kernel, 
        1, 
        NULL, 
        globalWorkSize, 
        NULL, 
        0, 
        NULL, 
        NULL);

    errors.push_back(localstatus);

    clEnqueueReadBuffer(
        cli->cmdQueue, 
        bufferC, 
        CL_TRUE,            // CL_TRUE is a BLOCKING read
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

    return ;
}
