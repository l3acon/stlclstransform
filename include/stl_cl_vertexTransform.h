#ifndef STL_CL_vXFORM
#define STL_CL_vXFORM

// System includes
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>

#include "cli.h"
#include "stl.h"


extern void stlclVertexTransform(
	XformMat *A, 
	std::vector<float> &verticies, 
	float *vertexBuffer,
	CLI* cli,
	std::vector<cl_int> &errors);


#endif