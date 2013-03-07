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
   
   struct NullDebugger
   {};

   template<class PlatformTraits = blib::system_info::SystemInfo
         ,class InstructionTraits = blib::odra::instruction_details::InstructionTraits
         ,class ExecutableBuffer = blib::odra::memory::ExeBuffer
         ,class DebuggerTraits = NullDebugger>
   class Assembler
   {
   private:
      ExecutableBuffer _codeBuffer;
   public:
      const boost::uint8_t* code()const
      {
         return _codeBuffer.data();
      }
      boost::uint64_t codeSize()const
      {
         _codeBuffer.size();
      }
      void clearCodeBuffer()
      {
         _codeBuffer.clear();
      }      
      void pushBackCode(const boost::uint8_t aByte)
      {
         _codeBuffer.push_back(aByte);
      }
      template<typename Size>
      void pushBackCode(const Size aWord)
      {
         _codeBuffer += (boost::uint8_t*)&aWord;
      }
      //! @brief Emit single @a opCode without operands.
      void pushBackOpCode(const boost::uint32_t aOpCode)
      {
         // instruction prefix
         if (aOpCode & 0xFF000000)
         {
            pushBackCode((boost::uint8_t)((aOpCode & 0xFF000000) >> 24));
         }
         // instruction opcodes
         if (aOpCode & 0x00FF0000)
         {
            pushBackCode((boost::uint8_t)((aOpCode & 0x00FF0000) >> 16));
         }
         if (aOpCode & 0x0000FF00)
         {
            pushBackCode((boost::uint8_t)((aOpCode & 0x0000FF00) >>  8));
         }
         // last opcode is always emitted (can be also 0x00)
         pushBackCode((boost::uint8_t)(aOpCode & 0x000000FF));
      }
      //! @brief Emit CS (code segmend) prefix. TODO
      //!
      //! Behavior of this function is to emit code prefix only if memory operand
      //! address uses code segment. Code segment is used through memory operand
      //! with attached @c AsmJit::Label.
      // void _emitSegmentPrefix(const Operand& rm)

      //! @brief Emit MODR/M byte.
      void pushBackMod(const boost::uint8_t m, const boost::uint8_t o, const boost::uint8_t r)
      { 
         pushBackCode(((m & 0x03) << 6) | ((o & 0x07) << 3) | (r & 0x07));
      }

      //! @brief Emit SIB byte.
      void pushBackSib(const boost::uint8_t s, const boost::uint8_t i, const boost::uint8_t b)
      { 
         pushBackCode(((s & 0x03) << 6) | ((i & 0x07) << 3) | (b & 0x07));
      }
   };// class Assembler

}// namespace assembler
}// namespace odra
}// namespace blib
#endif// __BLIB_ASSEMBLER_INCLUDED__
