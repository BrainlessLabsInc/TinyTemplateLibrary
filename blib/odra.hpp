#ifndef __BLIB_ODRA__
#define __BLIB_ODRA__

#ifdef __SAME_MACHINE_EXECUTION__
#undef __SAME_MACHINE_EXECUTION__
#endif
#ifndef __BLIB_SYSTEM_INFO_INCLUDED__
#include <blib/sys/system_info.hpp>
#endif

//---------------------------------------
// Microsoft Visual Studio
//---------------------------------------
#if defined( _MSC_VER )

   #if( _MSC_VER >= 1500 ) 
   #define BLIB_COMPILER_SUPPORTS_SSSE3
   #define BLIB_COMPILER_SUPPORTS_SSE4a
   #define BLIB_COMPILER_SUPPORTS_SSE41
   #define BLIB_COMPILER_SUPPORTS_SSE42

   #include <intrin.h>          // All intrinsics, including SSE3
   #include <intrin.h>          // All intrinsics, including SSE4a
   #include <smmintrin.h>       // Functions common to SSE4.1 and SSE5
   #include <mmintrin.h>
   #include <xmmintrin.h>
   #include <emmintrin.h>
   #endif
//---------------------------------------
// GCC
//---------------------------------------
#elif defined( __GNUC__ )
   #if( __GNUC__       >= 4 ) && ( __GNUC_MINOR__ >= 3 )
   #define BLIB_COMPILER_SUPPORTS_SSSE3
   #define BLIB_COMPILER_SUPPORTS_SSE4a
   #define BLIB_COMPILER_SUPPORTS_SSE41
   #define BLIB_COMPILER_SUPPORTS_SSE42
   #define BLIB_COMPILER_SUPPORTS_SSE5
   #endif

   #include <pmmintrin.h>           // SSE3
   #include <ammintrin.h>           // All intrinsics, including SSE4a
   #include <bmmintrin.h>           // SSE5
   #include <mmintrin-common.h>     // Functions common to SSE4.1 and SSE5
#endif
 

#ifndef __STD_STRING_INCLUDED__
#define __STD_STRING_INCLUDED__
#include <string>
#endif
#ifndef __STD_DEQUE_INCLUDED__
#define __STD_DEQUE_INCLUDED__
#include <deque>
#endif
#ifndef __STD_VECTOR_INCLUDED__
#define __STD_VECTOR_INCLUDED__
#include <vector>
#endif
#ifndef __STD_MAP_INCLUDED__
#define __STD_MAP_INCLUDED__
#include <map>
#endif
#ifndef __STD_ALGORITHM_INCLUDED__
#define __STD_ALGORITHM_INCLUDED__
#include <algorithm>
#endif

namespace blib{namespace odra{

   namespace register_details{
      /// Physical register ID
      enum PhysicalRegID
      {
         EAX=0, ECX, EDX, EBX, ESP, EBP, ESI, EDI, R8D, R9D, R10D, R11D, R12D, R13D, R14D, R15D,
         AL=0, CL, DL, BL, AH, CH, DH, BH, R8B, R9B, R10B, R11B, R12B, R13B, R14B, R15B,
         AX=0, CX, DX, BX, SP, BP, SI, DI, R8W, R9W, R10W, R11W, R12W, R13W, R14W, R15W,
         RAX=0, RCX, RDX, RBX, RSP, RBP, RSI, RDI, R8, R9, R10, R11, R12, R13, R14, R15,
         ST0=0, ST1, ST2, ST3, ST4, ST5, ST6, ST7,
         MM0=0, MM1, MM2, MM3, MM4, MM5, MM6, MM7,
         XMM0=0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, XMM7, XMM8, XMM9, XMM10, XMM11, XMM12, XMM13, XMM14, XMM15,
         YMM0=0, YMM1, YMM2, YMM3, YMM4, YMM5, YMM6, YMM7, YMM8, YMM9, YMM10, YMM11, YMM12, YMM13, YMM14, YMM15,
         INVALID=666
      };

      /** \var NUM_OF_PHYSICAL_REG
      * Number of physical register
      */
      /** \var SIZE_OF_GP_REG
      * Size of general-purpose register
      */
#ifdef __PLATFORM64__
      const int NUM_OF_PHYSICAL_REG = 16;
      const int SIZE_OF_GP_REG = 8;
#else
      const int NUM_OF_PHYSICAL_REG = 8;
      const int SIZE_OF_GP_REG = 4;
#endif

      /// Register type
      enum RegType
      {
         kRegGP,        ///< General purpose register
         kRegMMX,        ///< MMX register
         kRegXMM,        ///< XMM register
         kRegYMM,        ///< YMM register
         kRegFPU,        ///< FPU register
         kRegSymbolicGP,    ///< Symbolic general purpose register
         kRegSymbolicMMX,  ///< Symbolic MMX register
         kRegSymbolicXMM,  ///< Symbolic XMM register
         kRegSymbolicMM,    ///< Symbolic YMM register
         kRegInvalid = 666
      };
   } // namespace register_details

}  // namespace odra
}  // namespace blib

#endif  // #ifndef __BLIB_ODRA__