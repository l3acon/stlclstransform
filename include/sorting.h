#include <cstdlib>
#include <vector>


// void qsort (void* base, 
// 	size_t num, 
// 	size_t size, 
// 	int (*compar)(const void*,const void*));

// verticies are laid out
// x1 x2 x3 y1 y2 y3 z1 z2 z3
// we'll sort on z1

extern int vertex_comparator(const void *v1, const void *v2);