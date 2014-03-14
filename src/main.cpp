
// System includes
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <time.h> 


#include "stl_cl_vertexTransform.h"
#include "stl_cl_computeNormal.h"
#include "stl.h"
#include "cli.h"

#define CL_ERRORS FALSE

using namespace std;

extern cl_int stlclComputeNormal(
    std::vector<float> &verticies, 
    float *tempVBuff,
    std::vector<cl_int> &cliStati);

int main() 
{
    const char* stlFile = "Ring.stl";
    clock_t watch;
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
        cout<<"ERROR: verticies and normals don't make sense up"<<endl;
        return 1;
    }
    
    //for (int i = 0; i < verticies.size() ; ++i)
    //{
    //    printf("%d %f\n", i, verticies[i] );
    //}

    // start cloock
    watch = clock();

    vertexBuffer = (float*) malloc( sizeof(float)*verticies.size());

    stlclVertexTransform(matTransform, verticies, vertexBuffer, errors);

    #if CL_ERRORS
        for( std::vector<cl_int>::const_iterator i = errors.begin(); i != errors.end(); ++i)
        {
            printf("clVertexTransform:");
            PrintCLIStatus(*i);
        }
    #endif


    normalBuffer = (float*) malloc( sizeof(float)*normals.size());
    stlclComputeNormal(verticies, normalBuffer, errors);
    
    #if CL_ERRORS
        for( std::vector<cl_int>::const_iterator i = errors.begin(); i != errors.end(); ++i)
        {
            printf("clComputeNormal: ");
            PrintCLIStatus(*i);
        }
    #endif
    
    //end stopwatch
    watch = clock() - watch;


    //for (int i = 0; i < verticies.size(); ++i)
    //{
    //    printf("n:%d xf1:%f  xf2:%f\n", i, vertexBuffer[i], verticies[i] );
    //}

    //printf("Error in: %f",(stlVerifyTransform(matTransform, vertexBuffer, verticies.data(), verticies.size()/9 )));
    
    printf("%f seconds elapsed\nAssuming %ld clocks/sec\n", ((double)watch)/CLOCKS_PER_SEC,(long)CLOCKS_PER_SEC );

    return 0;
}


