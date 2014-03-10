#ifndef STL_CL_cNORM
#define STL_CL_cNORM
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

#include "cli.h"

// OpenCL includes
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#elif __linux
#include <CL/cl.h>
#else
#error Platform not supported
#endif

using namespace std;

void stlclComputeNormal(float *xform, std::vector<float> &verticies, float *tempVBuff);

#endif