#ifndef __BLIB_ASSEMBLER_INCLUDED__
#define __BLIB_ASSEMBLER_INCLUDED__

#ifndef __BLIB_INSTRUCTIONDETAILS_INCLUDED__
#include <blib/odra/InstructionDetails.hpp>
#endif
#ifndef __BLIB_SYSTEM_INFO_INCLUDED__
#include <blib/sys/system_info.hpp>
#endif
#ifndef __BLIB_MEMORY_INCLUDED__
#include <blib/odra/Memory.hpp>
#endif

namespace blib{namespace odra{namespace assembler{

   template<
       class PlatformTraits
      ,class InstructionTraits
      ,class ExecutableBuffer
      ,class DebuggerTraits>
   class Assembler
   {
   };// class Assembler

}// namespace assembler
}// namespace odra
}// namespace blib
#endif// __BLIB_ASSEMBLER_INCLUDED__