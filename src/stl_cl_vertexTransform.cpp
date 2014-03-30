
// transform STL verticies on GPU
// notes:
//test in GLGraphicWidget.cpp
//glMultMatrixd(m_viewpoint.transformMatrix().data())


#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>

#include "cli.h"
#include "stl.h"
#include "stl_cl_vertexTransform.h"

using namespace std;

void stlclVertexTransform(
    XformMat* transform, 
    std::vector<float> &verticies, 
    float *vertexBuffer, 
    CLI* cli,
    std::vector<cl_int> &errors)
{
    cl_int localstatus;
    unsigned int nVerticies = verticies.size();
    size_t vertexBytes = nVerticies * sizeof(float);
    float transformArray[transform->size];
    
    for (int i = 0; i < transform->size; ++i)
        transformArray[i] = transform->stlTransformMatrix[i];

    cl_mem bufferA = cliKernelArgs(
        transformArray,
        transform->size*sizeof(float),
        0,
        CL_MEM_READ_ONLY,
        cli,
        errors);

    cl_mem bufferB = cliKernelArgs(
        &verticies.front(),
        vertexBytes,
        1,
        CL_MEM_READ_ONLY,
        cli,
        errors);

    cl_mem bufferC = cliKernelArgs(
        vertexBuffer,
        vertexBytes,
        2,
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


    // STEP 12: Read the output buffer back to the host
    
    clEnqueueReadBuffer(
        cli->cmdQueue, 
        bufferC, 
        CL_TRUE,        // CL_TRUE is a BLOCKING read
        0, 
        vertexBytes, 
        vertexBuffer, 
        0, 
        NULL, 
        NULL);

    // Free OpenCL resources
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);

    // Free host resources

    return;
}
