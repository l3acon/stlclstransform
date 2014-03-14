
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <time.h>


#include "stl_cl_vertexTransform.h"
#include "stl_cl_computeNormal.h"
#include "stl.h"
#include "cli.h"
#include "kernels.h"

#define CL_ERRORS 1
#define TIME 0

#if TIME
#define BENCHSIZE 10
#endif

using namespace std;

int main() 
{
    const char* stlFile = "Ring.stl";

    //use this vector for erros
    std::vector<cl_int> errors;

    std::vector<float> verticies;
    std::vector<float> normals;

    //later we can just use the memory in a std::vector?
    float * vertexBuffer;
    float * normalBuffer;

    float matTransform[12];
    //float tol = 1e-6;

    //initalize our transform matrix naively
    for (int i = 0; i < 12; ++i)
        matTransform[i] = i;

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
    
    //for (int i = 0; i < verticies.size() ; ++i)
    //{
    //    printf("%d %f\n", i, verticies[i] );
    //}

    CLI *cli_computeNormal = cliInitialize();     
    cliBuild(cli_computeNormal, stl_cl_computeNormal_kernel_source, "_kComputeNormal");

    CLI *cli_vertexTransform = cliInitialize();
    cliBuild(cli_vertexTransform, stl_cl_vertexTransform_kernel_source, "_kVertexTransform");


    #if TIME
    timespec watch[BENCHSIZE], stop[BENCHSIZE];
    for (int i = 0; i < BENCHSIZE; ++i)
    {
        clock_gettime(CLOCK_REALTIME, &watch[i]);
    #endif

        vertexBuffer = (float*) malloc( sizeof(float)*verticies.size());

        stlclVertexTransform(matTransform, verticies, vertexBuffer, errors, cli_vertexTransform);

        #if CL_ERRORS
            for( std::vector<cl_int>::const_iterator iter = errors.begin(); iter != errors.end(); ++iter)
            {
                printf("clVertexTransform:");
                PrintCLIStatus(*iter);
            }
        #endif

        
        // Z sort

        normalBuffer = (float*) malloc( sizeof(float)*normals.size());
        stlclComputeNormal(verticies, normalBuffer, errors, cli_computeNormal);
        
        #if CL_ERRORS
            for( std::vector<cl_int>::const_iterator iter = errors.begin(); iter != errors.end(); ++iter)
            {
                printf("clComputeNormal: ");
                PrintCLIStatus(*iter);
            }
        #endif
        
    #if TIME    
        clock_gettime(CLOCK_REALTIME, &stop[i]); // Works on Linux but not OSX
    }
    double acc = 0.0;
    for (int i = 0; i < BENCHSIZE; ++i)
        acc += stop[i].tv_sec - watch[i].tv_sec + (stop[i].tv_nsec - watch[i].tv_nsec)/1e9;
    printf("[elapsed time] %f\n", acc/BENCHSIZE);
    #endif

    //for (int i = 0; i < verticies.size(); ++i)
    //{
    //    printf("n:%d xf1:%f  xf2:%f\n", i, vertexBuffer[i], verticies[i] );
    //}

    //printf("Error in: %f",(stlVerifyTransform(matTransform, vertexBuffer, verticies.data(), verticies.size()/9 )));
    
    cliRelease(cli_computeNormal);
    cliRelease(cli_vertexTransform);

    return 0;
}


