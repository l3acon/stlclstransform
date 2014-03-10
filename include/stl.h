#ifndef STL_H
#define STL_H


#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>


int stlRead(const char* stlFile, 
    std::vector<float> &verticies , 
    std::vector<float> &normals);

int stlWrite(const char* stlFile, 
    std::vector<float> &verticies, 
    std::vector<float> &normals);

float stlVerifyTransform(const float* xMat, 
    float* v, float* xformedv, 
    unsigned int nFaces);


#endif