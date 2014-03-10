#ifndef STL_CL_vXFORM
#define STL_CL_vXFORM

// System includes
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>

#include "cli.h"
#include "stl_cl_vertexTransform.h"

// OpenCL includes
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#elif __linux
#include <CL/cl.h>
#else
#error Platform not supported
#endif



extern void stlclVertexTransform(float *xform, 
	std::vector<float> &verticies, 
	float *tempVBuff);


#endif