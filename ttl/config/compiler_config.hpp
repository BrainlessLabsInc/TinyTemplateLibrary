#ifndef __TTL_COMPILER_CONFIG_INCLUDED__
#define __TTL_COMPILER_CONFIG_INCLUDED__

//Mostly credited to Boost Config
#if defined(__GCCXML__)
// GCC-XML emulates other compilers, it has to appear first here!
#define TTL_COMPILER 'GXML'
#define TTL_GCCXML
#elif defined __CUDACC__
// NVIDIA CUDA C++ compiler for GPU
#define TTL_COMPILER 'CUDA'
#define TTL_CUDA 
#elif defined __COMO__
// Comeau C++
#define TTL_COMPILER 'COMO'
#define TTL_COMEAU
#elif defined __clang__
// Clang C++ emulates GCC, so it has to appear early.
#define TTL_COMPILER 'CLAN'
#define TTL_CLANG
#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)
// Intel
#define TTL_COMPILER 'INTL'
#define TTL_INTEL
# elif defined __GNUC__
//  GNU C++:
#define TTL_COMPILER 'GCPP'
#define TTL_GCC
#elif defined _MSC_VER
//  Microsoft Visual C++
#define TTL_COMPILER 'MSVC'
#define TTL_MSVC
#else
#define TTL_COMPILER 'UNSP'
#define TTL_COMPILER_UNSUPPORTED
#endif

#endif// __TTL_COMPILER_CONFIG_INCLUDED__