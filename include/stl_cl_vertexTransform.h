#ifndef STL_CL_vXFORM
#define STL_CL_vXFORM

// System includes
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>

#include "cli.h"
#include "stl.h"

// OpenCL includes
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#elif __linux
#include <CL/cl.h>
#else
#error Platform not supported
#endif


extern void stlclVertexTransform(
	XformMat *A, 
	std::vector<float> &verticies, 
	float *vertexBuffer,
	CLI* cli,
	std::vector<cl_int> &errors);


#endif