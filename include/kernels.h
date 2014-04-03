
// kernels for OpenCL STL Vertex Transform and
// STL Compute Normal 

#ifndef KERNELS_H
#define KERNELS_H



// this kernel takes ~40-50 FLOPS
// Global memory reads: 12*4 (xMat) + 9*4 (vi) = 84 BYTES
// Global memory writes: 9*4 (verto) = 36 BYTES

// (GDDR5 can transfer at most 32 BYTES per clock)

const char * stl_cl_vertexTransform_kernel_source  =
"__kernel                                                   "
"\nvoid _kVertexTransform(                          		"
"\n            __global float *xMat,                   		"
"\n            __global float *vi,                    		"
"\n            __global float *verto)                       "      //restrict?
"\n{                                                        "
"\n                                                         "
"\n    // Get the work-item’s unique ID                     "
"\n    unsigned int i = 9*get_global_id(0);                "
"\n                                                         "
"\n    // do the matTransform                               "
"\n                                                         "
"\n    // x1-3 = 0 1 2                                      "
"\n    // y1-3 = 3 4 5                                      "
"\n    // z1-3 = 6 7 8                                      "
"\n                                                         "
"\n    //there's a decent chance this doesnt actually work  "
"\n    //x coordinates                                      "
"\n    verto[i+0] = xMat[0]*vi[i+0] + xMat[1]*vi[i+3] + xMat[2]*vi[i+6] + xMat[3];      "
"\n    verto[i+1] = xMat[0]*vi[i+1] + xMat[1]*vi[i+4] + xMat[2]*vi[i+7] + xMat[3];      "
"\n    verto[i+2] = xMat[0]*vi[i+2] + xMat[1]*vi[i+5] + xMat[2]*vi[i+8] + xMat[3];      "
"\n                                                                                     "
"\n    //y coordinates                                                                  "
"\n    verto[i+3] = xMat[4]*vi[i+0] + xMat[5]*vi[i+3] + xMat[6]*vi[i+6] + xMat[7];      "
"\n    verto[i+4] = xMat[4]*vi[i+1] + xMat[5]*vi[i+4] + xMat[6]*vi[i+7] + xMat[7];      "
"\n    verto[i+5] = xMat[4]*vi[i+2] + xMat[5]*vi[i+5] + xMat[6]*vi[i+8] + xMat[7];      "
"\n                                                                                     "
"\n    //z coordinates                                                                  "
"\n    verto[i+6] = xMat[8]*vi[i+0] + xMat[9]*vi[i+3] + xMat[10]*vi[i+6] + xMat[11];    "
"\n    verto[i+7] = xMat[8]*vi[i+1] + xMat[9]*vi[i+4] + xMat[10]*vi[i+7] + xMat[11];    "
"\n    verto[i+8] = xMat[8]*vi[i+2] + xMat[9]*vi[i+5] + xMat[10]*vi[i+8] + xMat[11];    "
"\n                                                                                     "
"\n}                                                                                    "
;


// this kernel takes ~16 FLOPS
// Global memory reads: 9*4 (vi) = 36 BYTES
// Global memory writes: 3*4 (verto) = 12 BYTES

// (GDDR5 can transfer at most 32 BYTES per clock)

const char * stl_cl_computeNormal_kernel_source  =
"__kernel                                               "
"\nvoid _kComputeNormal(                                "
"\n            __global float *vi,                      "
"\n            __global float *no)                   "
"\n{                                                    "
"\n                                                     "
"\n    // Get the work-item’s unique ID                 "
"\n    unsigned int ii = 9*get_global_id(0);            "
"\n    unsigned int io = 3*get_global_id(0);            "
"\n                                                     "
"\n    //fairly sure this actually works                "
"\n    float t[4];                                      "
"\n    t[0] = (vi[ii+4]-vi[ii+3])*(vi[ii+8]-vi[ii+6])   "
" - (vi[ii+7]-vi[ii+6])*(vi[ii+5]-vi[ii+3]); "
"\n    t[1] = (vi[ii+7]-vi[ii+6])*(vi[ii+2]-vi[ii+0])   "
" - (vi[ii+1]-vi[ii+0])*(vi[ii+8]-vi[ii+6]); "
"\n    t[2] = (vi[ii+1]-vi[ii+0])*(vi[ii+5]-vi[ii+3])   "
" - (vi[ii+4]-vi[ii+3])*(vi[ii+2]-vi[ii+0]); "
"\n    t[3] = t[1]+t[2]+t[3];                           "
"\n                                                     "
"\n    no[io  ] = t[0]/t[3];                         "   
"\n    no[io+1] = t[1]/t[3];                         "  
"\n    no[io+2] = t[2]/t[3];                         "  
"\n}                                                    "
;

#endif
