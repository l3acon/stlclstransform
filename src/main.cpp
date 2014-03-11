
// MAC & Intel:
//clang -framework OpenCL *.c
//Linux & AMD: 
//gcc -std=c11 -I /opt/AMDAPP/include/ *.c -lOpenCL

//test in GLGraphicWidget.cpp
//glMultMatrixd(m_viewpoint.transformMatrix().data())

// System includes
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

#include "stl_cl_vertexTransform.h"
#include "stl_cl_computeNormal.h"
#include "stl.h"


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


    vBuffer = (float*) malloc(sizeof(float)*verticies.size());
    stlclVertexTransform(xformMat, verticies, vBuffer);

    //for (int i = 0; i < verticies.size(); ++i)
    //{
    //    printf("n:%d xf1:%f  xf2:%f\n", i, vBuffer[i], verticies[i] );
    //}

    //printf("Error in: %f",(stlVerifyTransform(xformMat, vBuffer, verticies.data(), verticies.size()/9 )));

    return 0;
}


