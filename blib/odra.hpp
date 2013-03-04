#ifndef __BLIB_ODRA__
#define __BLIB_ODRA__

#ifdef __SAME_MACHINE_EXECUTION__
#undef __SAME_MACHINE_EXECUTION__
#endif
#ifndef __BLIB_SYSTEM_INFO_INCLUDED__
#include <blib/sys/system_info.hpp>
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
#ifndef BOOST_TYPE_TRAITS_HPP
#include <boost/type_traits.hpp>
#endif
#ifndef BOOST_ATOMIC_HPP
#include <boost/atomic.hpp>
#endif

namespace blib{
   namespace utils{
      template<typename T>
      T invalid_value();
   }// namespace utils
}

namespace blib{namespace odra{

   namespace register_details{
      using namespace blib::utils;
      template<typename T>
      T invalid_value();
      /// Physical register ID
      enum EPhysicalRegID
      {
         EAX=0, ECX, EDX, EBX, ESP, EBP, ESI, EDI, R8D, R9D, R10D, R11D, R12D, R13D, R14D, R15D,
         AL, CL, DL, BL, AH, CH, DH, BH, R8B, R9B, R10B, R11B, R12B, R13B, R14B, R15B,
         AX0, CX, DX, BX, SP, BP, SI, DI, R8W, R9W, R10W, R11W, R12W, R13W, R14W, R15W,
         RAX0, RCX, RDX, RBX, RSP, RBP, RSI, RDI, R8, R9, R10, R11, R12, R13, R14, R15,
         ST00, ST1, ST2, ST3, ST4, ST5, ST6, ST7,
         MM0, MM1, MM2, MM3, MM4, MM5, MM6, MM7,
         XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, XMM7, XMM8, XMM9, XMM10, XMM11, XMM12, XMM13, XMM14, XMM15,
         YMM0, YMM1, YMM2, YMM3, YMM4, YMM5, YMM6, YMM7, YMM8, YMM9, YMM10, YMM11, YMM12, YMM13, YMM14, YMM15,
         kPhysicalRegIdMax,
         kInvalidRegId =666
      };
      template<>
      EPhysicalRegID invalid_value<EPhysicalRegID>()
      {
         return kInvalidRegId;
      }
      /// Register type
      enum ERegType
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
         kRegSymbolicMax,
         kRegTypeInvalid = 666
      };
      template<>
      ERegType invalid_value<ERegType>()
      {
         return kRegTypeInvalid;
      }


      /// Operand type
      enum EOperandType
      {
         kOpTypeNone,
         kOpTypeReg,
         kOpTypeMem,
         kOpTypeImm,
         kOpTypeMask = 0x03,

         kOpTypeDummy = 1 << 2, ///< The operand which has this flag is not encoded. This is for register allocator.
         kOpTypeRead = 1 << 3, ///< The operand is used for reading.
         kOpTypeWrite = 1 << 4, ///< The operand is used for writing.
         kOpInvalid = 666
      };
      template<>
      EOperandType invalid_value<EOperandType>()
      {
         return kOpInvalid;
      }

      struct PlatformTraits
      {
         /** \var NUM_OF_PHYSICAL_REG
         * Number of physical register
         */
         /** \var SIZE_OF_GP_REG
         * Size of general-purpose register
         */
#ifdef __PLATFORM64__
         static const int numOfPhysicalRegs = 16;
         static const int gpRegSize = 8;
         static const int platformSize = 64;
#else
         static const int numOfPhysicalRegs = 8;
         static const int gpRegSize = 4;
         static const int platformSize = 32;
#endif
      };

      template<>
      boost::int64_t invalid_value<boost::int64_t>()
      {
         return 0;
      }
      template<bool kIsSymbolic>
      class RegId
      {
         typedef RegId MyType;
         typedef typename boost::conditional<kIsSymbolic, boost::int64_t,EPhysicalRegID>::type RegIdType;
      private:
         RegIdType _regId;
         ERegType _regType;
      public:
         RegId()
            :_regId(invalid_value<RegIdType>()),_regType(invalid_value<ERegType>())
         {
            if(kIsSymbolic)
            {
               initRegId<kIsSymbolic>();
            }
         }
         RegId(const RegId& aRhs)
            :_regId(invalid_value<RegIdType>()),_regType(aRhs._regType)
         {
            if(kIsSymbolic)
            {
               initRegId<kIsSymbolic>();
            }
         }
         RegId(const RegIdType aRegId, const ERegType aRegType)
            :_regId(invalid_value<RegIdType>()),_regType(aRegType)
         {    
            if(isSymbolic)
            {
               initRegId<kIsSymbolic>();
            }
         }
         bool operator==(const RegId& aRegId)const
         {
            return (_regId == aRegId._regId) && (_regType == aRegId._regType);
         }
         bool operator!=(const RegId& aRegId)const
         {
            return !(*this == aRegId);
         }
 
         bool isInvalid()const
         {
            return (invalid_value<RegIdType>() == _regId) || (invalid_value<ERegType>() == _regType);
         }

         bool isSymbolic()const
         {
            return kIsSymbolic;
         }
      private:
         template<bool isSym>
         void initRegId();

         template<>
         void initRegId<true>()
         {
            static mutable boost::atomic<RegIdType> _nextId(1);
            _regId = _nextId.fetch_add(1, boost::memory_order_relaxed);
         }
      };

      // Register
      struct Reg
      {
         RegId<false> _regId;
         bool _isReassignable;
         Reg()
            :_isReassignable(false)
         {}
      };

      // Memory
      struct Mem
      {
         RegId<false> _base;
         RegId<false> _index;
         boost::int64_t _scale;
         boost::int64_t _disp;
         boost::uint8_t _baseSize; // OpdSize
         boost::uint8_t _indexSize; // OpdSize
         Mem()
            :_scale(0),_disp(0),_baseSize(0),_indexSize(0)
         {}
      };

      // Immediate
      struct Immediate
      {
         typedef boost::int64_t ValueType;
         ValueType _value;
         Immediate(const ValueType aValue)
            :_value(aValue)
         {}
      };

      template<typename OperandTraits,typename TPlatformTraits = PlatformTraits>
      class Operand
      {
      public:
         typedef typename OperandTraits::OperandType OperandType;
         typedef boost::uint16_t SizeType;
      private:
         OperandType _operandType;
         SizeType _operandSize;
      };

   } // namespace register_details

}  // namespace odra
}  // namespace blib

#endif  // #ifndef __BLIB_ODRA__
