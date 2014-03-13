
// System includes
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>

#include "stl_cl_vertexTransform.h"
#include "stl_cl_computeNormal.h"
#include "stl.h"
#include "cli.h"

using namespace std;

extern cl_int stlclComputeNormal(
    std::vector<float> &verticies, 
    float *tempVBuff,
    std::vector<cl_int> &cliStati);

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
        cout<<"ERROR: verticies and normals don't make sense up"<<endl;
        return 1;
    }
    
    //for (int i = 0; i < verticies.size() ; ++i)
    //{
    //    printf("%d %f\n", i, verticies[i] );
    //}

    vertexBuffer = (float*) malloc( sizeof(float)*verticies.size());

    if( stlclVertexTransform(matTransform, verticies, vertexBuffer, errors))
    {
        for( std::vector<cl_int>::const_iterator i = errors.begin(); i != errors.end(); ++i)
        {
            printf("clVertexTransform:");
            PrintCLIStatus(*i);
        }
    }

    normalBuffer = (float*) malloc( sizeof(float)*normals.size());
    if( stlclComputeNormal(verticies, normalBuffer, errors) )
    {
        for( std::vector<cl_int>::const_iterator i = errors.begin(); i != errors.end(); ++i)
        {
            printf("clComputeNormal:");
            PrintCLIStatus(*i);
        }
    }

    //for (int i = 0; i < verticies.size(); ++i)
    //{
    //    printf("n:%d xf1:%f  xf2:%f\n", i, vertexBuffer[i], verticies[i] );
    //}

    //printf("Error in: %f",(stlVerifyTransform(matTransform, vertexBuffer, verticies.data(), verticies.size()/9 )));

    return 0;
}


