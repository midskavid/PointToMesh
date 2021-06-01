#ifndef CORETYPES_H
#define CORETYPES_H

#include <limits>

#ifdef FLOAT_AS_DOUBLE
typedef double Float;
#else
typedef float Float;
#endif

#define MaxFloat std::numeric_limits<Float>::max()
#define Infinity std::numeric_limits<Float>::infinity()

#endif