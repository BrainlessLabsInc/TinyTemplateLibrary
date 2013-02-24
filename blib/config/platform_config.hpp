#ifndef __BLIB_PLATFORM_CONFIG_INCLUDED__
#define __BLIB_PLATFORM_CONFIG_INCLUDED__

#ifndef _SDL_platform_h
#include <blib/config/details/SDL_platform.h>
#endif
#if (defined(_WIN64) && (defined(_M_AMD64) || defined(_M_X64))) || defined(__x86_64__)
#undef __PLATFORM64__
#define __PLATFORM64__
#endif

#if defined(__linux__)     || defined(__unix__)    || \
      defined(__OpenBSD__)   || defined(__FreeBSD__) || defined(__NetBSD__) || \
      defined(__DragonFly__) || defined(__BSD__)     || defined(__FREEBSD__) || \
      defined(__APPLE__)
# define __POSIX__
#endif

#endif// __BLIB_PLATFORM_CONFIG_INCLUDED__
