#ifndef STL_CL_cNORM
#define STL_CL_cNORM

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

extern cl_int stlclComputeNormal(
	std::vector<float> &verticies, 
	float *normalBuffer,
	std::vector<cl_int> &cliStati);

#endif