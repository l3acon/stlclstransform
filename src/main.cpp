
// MATTHEW FERNANDEZ 2014
// 
// should probably add a license
// 
// benchmark/test program for STL transform on large STL files
// OpenCL implimentation for GPGPU
//

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <time.h>

#include "sorting.h"
#include "stl_cl_vertexTransform.h"
#include "stl_cl_computeNormal.h"
#include "stl.h"
#include "cli.h"
#include "kernels.h"

#define CL_ERRORS 1

#ifndef _WIN32
#ifndef __APPLE__
#define TIME 1
#define BENCHSIZE 10
#endif
#endif

using namespace std;

int main() 
{
    const char* stlFile = "Ring.stl";

    std::vector<float> verticies;
    std::vector<float> normals;
    std::vector<cl_int> errorsVT;
    std::vector<cl_int> errorsCN;
    
    //later we can just use the memory in a std::vector?
    float * vertexBuffer;
    float * normalBuffer;

    XformMat A;

    //initalize our transform matrix naively
    for (int i = 0; i < A.size; ++i)
        A.stlTransformMatrix[i] = (float) i;

    //file stuff
    if(stlRead(stlFile, verticies, normals))
    {
        std::cout<<"ERROR: reading file"<<std::endl;
        return 1;
    }

    //check sanity for verticies and normals
    if( fmod(verticies.size(),9.0) !=  0 || fmod(normals.size(),3.0) != 0 )
    {
        std::cout<<"ERROR: verticies and normals don't make sense up"<<std::endl;
        return 1;
    }

    // set up CLInterface resrouces
    CLI *cli_vertexTransform = (CLI*) malloc( sizeof(CLI));
    cliInitialize(cli_vertexTransform, errorsVT);
    cliBuild(
        cli_vertexTransform, 
        stl_cl_vertexTransform_kernel_source, 
        "_kVertexTransform",
        errorsVT);

    CLI *cli_computeNormal = (CLI*) malloc( sizeof(CLI));
    cliInitialize(cli_computeNormal, errorsCN);
    cliBuild(
        cli_computeNormal, 
        stl_cl_computeNormal_kernel_source, 
        "_kComputeNormal", 
        errorsCN);

    // do the benchmark
    #if TIME
    timespec watch[BENCHSIZE], stop[BENCHSIZE];
    for (int i = 0; i < BENCHSIZE; ++i)
    {
        clock_gettime(CLOCK_REALTIME, &watch[i]);
    #endif

        vertexBuffer = (float*) malloc( sizeof(float) * verticies.size());

        // do vertext transform
        stlclVertexTransform(
            &A, 
            verticies, 
            vertexBuffer, 
            cli_vertexTransform, 
            errorsVT);

        #if CL_ERRORS
        printf("_kVertexTransform:\n");
        PrintCLIStatus(errorsVT);
        #endif

        // Z sort
        //qsort(vertexBuffer, verticies.size() - 1, sizeof(float)*9, vertex_comparator);

        // compute normals
        normalBuffer = (float*) malloc( sizeof(float) * normals.size());
        stlclComputeNormal(
            verticies.size(),
            vertexBuffer, 
            normalBuffer, 
            cli_computeNormal, 
            errorsCN);
        
        #if CL_ERRORS
        printf("_kComputeNormal:\n");
        PrintCLIStatus(errorsCN);
        #endif
        
    #if TIME    
        clock_gettime(CLOCK_REALTIME, &stop[i]); // Works on Linux but not OSX
    }

    double acc = 0.0;
    for (int i = 0; i < BENCHSIZE; ++i)
        acc += stop[i].tv_sec - watch[i].tv_sec + (stop[i].tv_nsec - watch[i].tv_nsec)/1e9;
    printf("[elapsed time] %f\n", acc/BENCHSIZE);
    #endif

    cliRelease(cli_computeNormal);
    cliRelease(cli_vertexTransform);
    return 0;
}


