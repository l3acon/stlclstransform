
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
    cl_mem bufferA = cliKernelArgs(
        verticies.data(),
        vertexBytes,
        0,
        CL_MEM_READ_ONLY,
        cli,
        errors);


    cl_mem bufferC = cliKernelArgs(
        normalBuffer,
        normalBytes,
        1,
        CL_MEM_WRITE_ONLY,
        cli,
        errors);

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
