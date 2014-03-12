#ifndef STL_CL_vXFORM
#define STL_CL_vXFORM

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



extern cl_int stlclVertexTransform(
	float *matTransform, 
	std::vector<float> &verticies, 
	float *tempVBuff,
	std::vector<cl_int> &cliStati);


#endif