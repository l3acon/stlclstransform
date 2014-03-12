
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


int main() 
{
    const char * stlFile = "Ring.stl";
    std::vector<float> verticies;
    std::vector<float> normals;
    float * vBuffer;
    float xformMat[12];
    //float tol = 1e-6;

    for (int i = 0; i < 12; ++i)
        xformMat[i] = i;

    if(stlRead(stlFile, verticies, normals))
    {
        cout<<"ERROR: reading file"<<endl;
        return 1;
    }

    if( fmod(verticies.size(),9.0) !=  0 || fmod(normals.size(),3.0) != 0 )
    {
        cout<< verticies.size()<<endl;
        cout<< normals.size()<<endl;
        cout<<"ERROR: verticies and normals don't match up"<<endl;
        return 1;
    }
    
    //for (int i = 0; i < verticies.size() ; ++i)
    //{
    //    printf("%d %f\n", i, verticies[i] );
    //}

    std::vector<cl_int> errors;
    vBuffer = (float*) malloc(sizeof(float)*verticies.size());
    if( stlclVertexTransform(xformMat, verticies, vBuffer, errors))
        for( std::vector<cl_int>::const_iterator i = errors.begin(); i != errors.end(); ++i)
            PrintCLIStatus(*i);

    //for (int i = 0; i < verticies.size(); ++i)
    //{
    //    printf("n:%d xf1:%f  xf2:%f\n", i, vBuffer[i], verticies[i] );
    //}

    //printf("Error in: %f",(stlVerifyTransform(xformMat, vBuffer, verticies.data(), verticies.size()/9 )));

    return 0;
}


