#ifndef __BLIB_ODRA__
#define __BLIB_ODRA__

#ifdef __SAMkEncodeMACHINkEncodeEXECUTION__
#undef __SAMkEncodeMACHINkEncodeEXECUTION__
#endif
#ifndef __BLIB_SYSTEM_INFO_INCLUDED__
#include <blib/sys/system_info.hpp>
#endif

#ifndef __STD_STRING_INCLUDED__
#define __STD_STRING_INCLUDED__
#include <string>
#endif
#ifndef __STD_DEQUkEncodeINCLUDED__
#define __STD_DEQUkEncodeINCLUDED__
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
#ifndef BOOST_TYPkEncodeTRAITS_HPP
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
         /** \var SIZkEncodeOF_GP_REG
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

      struct OperandTraits
      {
         typedef Immediate ImmediateType;
         typedef Mem MemType;
         typedef Reg RegType;
         template<EOperandType kIn>
         struct SelectOperandType
         {
            typename boost::conditional<kOpTypeReg == kIn
               ,RegType, typename boost::conditional<kOpTypeMem == kIn
               ,MemType, ImmediateType>::type>::type OperandType;
         };
      };

      template<EOperandType kOperandType,typename OperandTraits,typename TPlatformTraits = PlatformTraits>
      class Operand : public OperandTraits::SelectOperandType<kOperandType>::OperandType
      {
      public:
         typedef typename OperandTraits::SelectOperandType<kOperandType>::OperandType OperandType;
         typedef boost::uint16_t SizeType;
         EOperandType operandType()const
         {
            return kOperandType;
         }
         SizeType operandSize()const
         {
            return _operandSize;
         }
         OperandType& operand()
         {
            return _operand;
         }
      private:
         OperandType _operand;
         SizeType _operandSize;
      };

   } // namespace register_details


   namespace instruction_details{
      /// Instruction ID
      enum InstrID
      {
         kInstADC, kInstADD, kInstAND,
         kInstBSF, kInstBSR, kInstBSWAP, kInstBT, kInstBTC, kInstBTR, kInstBTS,
         kInstCALL, kInstCBW, kInstCLC, kInstCLD, kInstCLI, kInstCLTS, kInstCMC, kInstCMOVCC, kInstCMP, kInstCMPS_B, kInstCMPS_W, kInstCMPS_D, kInstCMPS_Q, kInstCMPXCHG,
         kInstCMPXCHG8B, kInstCMPXCHG16B, kInstCPUID, kInstCWD, kInstCDQ, kInstCQO,
         kInstDEC, kInstDIV,
         kInstENTER,
         kInstHLT,
         kInstIDIV, kInstIMUL, kInstIN, kInstINC, kInstINS_B, kInstINS_W, kInstINS_D, kInstINVD, kInstINVLPG, kInstINT3, kInstINTN, kInstINTO, kInstIRET, kInstIRETD, kInstIRETQ,
         kInstJMP, kInstJCC,
         kInstLAR, kInstLEA, kInstLEAVE, kInstLLDT, kInstLMSW, kInstLSL, kInstLTR, kInstLODS_B, kInstLODS_W, kInstLODS_D, kInstLODS_Q, kInstLOOP,
         kInstMOV, kInstMOVBE, kInstMOVS_B, kInstMOVS_W, kInstMOVS_D, kInstMOVS_Q, kInstMOVZX, kInstMOVSX, kInstMOVSXD, kInstMUL,
         kInstNEG, kInstNOP, kInstNOT,
         kInstOR, kInstOUT, kInstOUTS_B, kInstOUTS_W, kInstOUTS_D,
         kInstPOP, kInstPOPAD, kInstPOPF, kInstPOPFD, kInstPOPFQ, kInstPUSH, kInstPUSHAD, kInstPUSHF, kInstPUSHFD, kInstPUSHFQ,
         kInstRDMSR, kInstRDPMC, kInstRDTSC, kInstRET, kInstRCL, kInstRCR, kInstROL, kInstROR, kInstRSM,
         kInstSAR, kInstSHL, kInstSHR, kInstSBB, kInstSCAS_B, kInstSCAS_W, kInstSCAS_D, kInstSCAS_Q, kInstSETCC, kInstSHLD, kInstSHRD, kInstSGDT, kInstSIDT, kInstSLDT, kInstSMSW, kInstSTC, kInstSTD, kInstSTI,
         kInstSTOS_B, kInstSTOS_W, kInstSTOS_D, kInstSTOS_Q, kInstSUB, kInstSWAPGS, kInstSYSCALL, kInstSYSENTER, kInstSYSEXIT, kInstSYSRET,
         kInstTEST,
         kInstUD2,
         kInstVERR, kInstVERW,
         kInstWAIT, kInstWBINVD, kInstWRMSR,
         kInstXADD, kInstXCHG, kInstXGETBV, kInstXLATB, kInstXOR,

         kInstF2XM1, kInstFABS, kInstFADD, kInstFADDP, kInstFIADD,
         kInstFBLD, kInstFBSTP, kInstFCHS, kInstFCLEX, kInstFNCLEX, kInstFCMOVCC, kInstFCOM, kInstFCOMP, kInstFCOMPP, kInstFCOMI, kInstFCOMIP, kInstFCOS,
         kInstFDECSTP, kInstFDIV, kInstFDIVP, kInstFIDIV, kInstFDIVR, kInstFDIVRP, kInstFIDIVR,
         kInstFFREE,
         kInstFICOM, kInstFICOMP, kInstFILD, kInstFINCSTP, kInstFINIT, kInstFNINIT, kInstFIST, kInstFISTP,
         kInstFLD, kInstFLD1, kInstFLDCW, kInstFLDENV, kInstFLDL2E, kInstFLDL2T, kInstFLDLG2, kInstFLDLN2, kInstFLDPI, kInstFLDZ,
         kInstFMUL, kInstFMULP, kInstFIMUL,
         kInstFNOP,
         kInstFPATAN, kInstFPREM, kInstFPREM1, kInstFPTAN,
         kInstFRNDINT, kInstFRSTOR,
         kInstFSAVE, kInstFNSAVE, kInstFSCALE, kInstFSIN, kInstFSINCOS, kInstFSQRT, kInstFST, kInstFSTP, kInstFSTCW, kInstFNSTCW, kInstFSTENV, kInstFNSTENV, kInstFSTSW, kInstFNSTSW,
         kInstFSUB, kInstFSUBP, kInstFISUB, kInstFSUBR, kInstFSUBRP, kInstFISUBR,
         kInstFTST,
         kInstFUCOM, kInstFUCOMP, kInstFUCOMPP, kInstFUCOMI, kInstFUCOMIP,
         kInstFXAM, kInstFXCH, kInstFXRSTOR, kInstFXSAVE, kInstFXTRACT,
         kInstFYL2X, kInstFYL2XP1,

         kInstADDPS, kInstADDSS, kInstADDPD, kInstADDSD, kInstADDSUBPS, kInstADDSUBPD, kInstANDPS, kInstANDPD, kInstANDNPS, kInstANDNPD,
         kInstBLENDPS, kInstBLENDPD, kInstBLENDVPS, kInstBLENDVPD,
         kInstCLFLUSH, kInstCMPPS, kInstCMPSS, kInstCMPPD, kInstCMPSD, kInstCOMISS, kInstCOMISD, kInstCRC32,
         kInstCVTDQ2PD, kInstCVTDQ2PS, kInstCVTPD2DQ, kInstCVTPD2PI, kInstCVTPD2PS, kInstCVTPI2PD, kInstCVTPI2PS, kInstCVTPS2DQ, kInstCVTPS2PD, kInstCVTPS2PI, kInstCVTSD2SI,
         kInstCVTSD2SS, kInstCVTSI2SD, kInstCVTSI2SS, kInstCVTSS2SD, kInstCVTSS2SI, kInstCVTTPD2DQ, kInstCVTTPD2PI, kInstCVTTPS2DQ, kInstCVTTPS2PI, kInstCVTTSD2SI, kInstCVTTSS2SI,
         kInstDIVPS, kInstDIVSS, kInstDIVPD, kInstDIVSD, kInstDPPS, kInstDPPD,
         kInstEMMS, kInstEXTRACTPS,
         kInstFISTTP,
         kInstHADDPS, kInstHADDPD, kInstHSUBPS, kInstHSUBPD,
         kInstINSERTPS,
         kInstLDDQU, kInstLDMXCSR, kInstLFENCE,
         kInstMASKMOVDQU, kInstMASKMOVQ, kInstMAXPS, kInstMAXSS, kInstMAXPD, kInstMAXSD, kInstMFENCE, kInstMINPS, kInstMINSS, kInstMINPD, kInstMINSD, kInstMONITOR,
         kInstMOVAPD, kInstMOVAPS, kInstMOVD, kInstMOVDDUP, kInstMOVDQA, kInstMOVDQU, kInstMOVDQ2Q, kInstMOVHLPS, kInstMOVLHPS, kInstMOVHPS, kInstMOVHPD, kInstMOVLPS, kInstMOVLPD,
         kInstMOVMSKPS, kInstMOVMSKPD, kInstMOVNTDQ, kInstMOVNTDQA, kInstMOVNTI, kInstMOVNTPD, kInstMOVNTPS, kInstMOVNTQ, kInstMOVQ, kInstMOVQ2DQ, kInstMOVSD, kInstMOVSS,
         kInstMOVSHDUP, kInstMOVSLDUP, kInstMOVUPS, kInstMOVUPD, kInstMPSADBW, kInstMULPS, kInstMULSS, kInstMULPD, kInstMULSD, kInstMWAIT,
         kInstORPS, kInstORPD,
         kInstPABSB, kInstPABSD, kInstPABSW, kInstPACKSSDW, kInstPACKSSWB, kInstPACKUSDW, kInstPACKUSWB, kInstPADDB, kInstPADDD, kInstPADDQ, kInstPADDSB, kInstPADDSW, kInstPADDUSB,
         kInstPADDUSW, kInstPADDW, kInstPALIGNR, kInstPAND, kInstPANDN, kInstPAUSE, kInstPAVGB, kInstPAVGW,
         kInstPBLENDVB, kInstPBLENDW,
         kInstPCMPEQB, kInstPCMPEQW, kInstPCMPEQD, kInstPCMPEQQ, kInstPCMPESTRI, kInstPCMPESTRM, kInstPCMPISTRI, kInstPCMPISTRM, kInstPCMPGTB, kInstPCMPGTW, kInstPCMPGTD, kInstPCMPGTQ,
         kInstPEXTRB, kInstPEXTRW, kInstPEXTRD, kInstPEXTRQ,
         kInstPHADDW, kInstPHADDD, kInstPHADDSW, kInstPHMINPOSUW, kInstPHSUBW, kInstPHSUBD, kInstPHSUBSW,
         kInstPINSRB, kInstPINSRW, kInstPINSRD, kInstPINSRQ,
         kInstPMADDUBSW, kInstPMADDWD, kInstPMAXSB, kInstPMAXSW, kInstPMAXSD, kInstPMAXUB, kInstPMAXUW, kInstPMAXUD, kInstPMINSB, kInstPMINSW, kInstPMINSD, kInstPMINUB, kInstPMINUW,
         kInstPMINUD, kInstPMOVMSKB, kInstPMOVSXBW, kInstPMOVSXBD, kInstPMOVSXBQ, kInstPMOVSXWD, kInstPMOVSXWQ, kInstPMOVSXDQ, kInstPMOVZXBW, kInstPMOVZXBD, kInstPMOVZXBQ, kInstPMOVZXWD,
         kInstPMOVZXWQ, kInstPMOVZXDQ, kInstPMULDQ, kInstPMULHRSW, kInstPMULHUW, kInstPMULHW, kInstPMULLW, kInstPMULLD, kInstPMULUDQ,
         kInstPOPCNT, kInstPOR,
         kInstPREFETCH,
         kInstPSADBW, kInstPSHUFB, kInstPSHUFD, kInstPSHUFHW, kInstPSHUFLW, kInstPSHUFW, kInstPSIGNB, kInstPSIGNW, kInstPSIGND, kInstPSLLW, kInstPSLLD, kInstPSLLQ, kInstPSLLDQ, kInstPSRAW,
         kInstPSRAD, kInstPSRLW, kInstPSRLD, kInstPSRLQ, kInstPSRLDQ, kInstPSUBB, kInstPSUBW, kInstPSUBD, kInstPSUBQ, kInstPSUBSB, kInstPSUBSW, kInstPSUBUSB, kInstPSUBUSW,
         kInstPTEST,
         kInstPUNPCKHBW, kInstPUNPCKHWD, kInstPUNPCKHDQ, kInstPUNPCKHQDQ, kInstPUNPCKLBW, kInstPUNPCKLWD, kInstPUNPCKLDQ, kInstPUNPCKLQDQ,
         kInstPXOR,
         kInstRCPPS, kInstRCPSS, kInstROUNDPS, kInstROUNDPD, kInstROUNDSS, kInstROUNDSD, kInstRSQRTPS, kInstRSQRTSS,
         kInstSFENCE, kInstSHUFPS, kInstSHUFPD, kInstSQRTPS, kInstSQRTSS, kInstSQRTPD, kInstSQRTSD, kInstSTMXCSR, kInstSUBPS, kInstSUBSS, kInstSUBPD, kInstSUBSD,
         kInstUCOMISS, kInstUCOMISD, kInstUNPCKHPS, kInstUNPCKHPD, kInstUNPCKLPS, kInstUNPCKLPD,
         kInstXORPS, kInstXORPD,

         kInstVBROADCASTSS, kInstVBROADCASTSD, kInstVBROADCASTF128,
         kInstVEXTRACTF128,
         kInstVINSERTF128,
         kInstVMASKMOVPS, kInstVMASKMOVPD,
         kInstVPERMILPD, kInstVPERMILPS, kInstVPERM2F128,
         kInstVTESTPS, kInstVTESTPD,
         kInstVZEROALL, kInstVZEROUPPER,

         kInstAESENC, kInstAESENCLAST, kInstAESDEC, kInstAESDECLAST, kInstAESIMC, kInstAESKEYGENASSIST,
         kInstPCLMULQDQ,

         // FMA
         kInstVFMADD132PD, kInstVFMADD213PD, kInstVFMADD231PD, kInstVFMADD132PS, kInstVFMADD213PS, kInstVFMADD231PS,
         kInstVFMADD132SD, kInstVFMADD213SD, kInstVFMADD231SD, kInstVFMADD132SS, kInstVFMADD213SS, kInstVFMADD231SS,
         kInstVFMADDSUB132PD, kInstVFMADDSUB213PD, kInstVFMADDSUB231PD, kInstVFMADDSUB132PS, kInstVFMADDSUB213PS, kInstVFMADDSUB231PS,
         kInstVFMSUBADD132PD, kInstVFMSUBADD213PD, kInstVFMSUBADD231PD, kInstVFMSUBADD132PS, kInstVFMSUBADD213PS, kInstVFMSUBADD231PS,
         kInstVFMSUB132PD, kInstVFMSUB213PD, kInstVFMSUB231PD, kInstVFMSUB132PS, kInstVFMSUB213PS, kInstVFMSUB231PS,
         kInstVFMSUB132SD, kInstVFMSUB213SD, kInstVFMSUB231SD, kInstVFMSUB132SS, kInstVFMSUB213SS, kInstVFMSUB231SS,
         kInstVFNMADD132PD, kInstVFNMADD213PD, kInstVFNMADD231PD, kInstVFNMADD132PS, kInstVFNMADD213PS, kInstVFNMADD231PS,
         kInstVFNMADD132SD, kInstVFNMADD213SD, kInstVFNMADD231SD, kInstVFNMADD132SS, kInstVFNMADD213SS, kInstVFNMADD231SS,
         kInstVFNMSUB132PD, kInstVFNMSUB213PD, kInstVFNMSUB231PD, kInstVFNMSUB132PS, kInstVFNMSUB213PS, kInstVFNMSUB231PS,
         kInstVFNMSUB132SD, kInstVFNMSUB213SD, kInstVFNMSUB231SD, kInstVFNMSUB132SS, kInstVFNMSUB213SS, kInstVFNMSUB231SS,

         // F16C
         kInstRDFSBASE, kInstRDGSBASE, kInstRDRAND, kInstWRFSBASE, kInstWRGSBASE, kInstVCVTPH2PS, kInstVCVTPS2PH,

         // BMI
         kInstANDN, kInstBEXTR, kInstBLSI, kInstBLSMSK, kInstBLSR, kInstBZHI, kInstLZCNT, kInstMULX, kInstPDEP, kInstPEXT, kInstRORX, kInstSARX, kInstSHLX, kInstSHRX, kInstTZCNT, kInstINVPCID,

         // XOP
         kInstVFRCZPD, kInstVFRCZPS, kInstVFRCZSD, kInstVFRCZSS,
         kInstVPCMOV, kInstVPCOMB, kInstVPCOMD, kInstVPCOMQ, kInstVPCOMUB, kInstVPCOMUD, kInstVPCOMUQ, kInstVPCOMUW, kInstVPCOMW, kInstVPERMIL2PD, kInstVPERMIL2PS,
         kInstVPHADDBD, kInstVPHADDBQ, kInstVPHADDBW, kInstVPHADDDQ, kInstVPHADDUBD, kInstVPHADDUBQ, kInstVPHADDUBW, kInstVPHADDUDQ, kInstVPHADDUWD, kInstVPHADDUWQ,
         kInstVPHADDWD, kInstVPHADDWQ, kInstVPHSUBBW, kInstVPHSUBDQ, kInstVPHSUBWD,
         kInstVPMACSDD, kInstVPMACSDQH, kInstVPMACSDQL, kInstVPMACSSDD, kInstVPMACSSDQH, kInstVPMACSSDQL, kInstVPMACSSWD, kInstVPMACSSWW, kInstVPMACSWD, kInstVPMACSWW,
         kInstVPMADCSSWD, kInstVPMADCSWD,
         kInstVPPERM, kInstVPROTB, kInstVPROTD, kInstVPROTQ, kInstVPROTW, kInstVPSHAB, kInstVPSHAD, kInstVPSHAQ, kInstVPSHAW, kInstVPSHLB, kInstVPSHLD, kInstVPSHLQ, kInstVPSHLW,

         // FMA4
         kInstVFMADDPD, kInstVFMADDPS, kInstVFMADDSD, kInstVFMADDSS,
         kInstVFMADDSUBPD, kInstVFMADDSUBPS,
         kInstVFMSUBADDPD, kInstVFMSUBADDPS,
         kInstVFMSUBPD, kInstVFMSUBPS, kInstVFMSUBSD, kInstVFMSUBSS,
         kInstVFNMADDPD, kInstVFNMADDPS, kInstVFNMADDSD, kInstVFNMADDSS,
         kInstVFNMSUBPD, kInstVFNMSUBPS, kInstVFNMSUBSD, kInstVFNMSUBSS,

         // AVX2
         kInstVBROADCASTI128, kInstVPBROADCASTB, kInstVPBROADCASTW, kInstVPBROADCASTD, kInstVPBROADCASTQ,
         kInstPBLENDD, kInstVPERMD, kInstVPERMQ, kInstVPERMPS, kInstVPERMPD, kInstVPERM2I128,
         kInstVEXTRACTI128, kInstVINSERTI128, kInstVMASKMOVD, kInstVMASKMOVQ, kInstVPSLLVD, kInstVPSLLVQ, kInstVPSRAVD, kInstVPSRLVD, kInstVPSRLVQ,
         kInstVGATHERDPS, kInstVGATHERQPS, kInstVGATHERDPD, kInstVGATHERQPD, kInstVPGATHERDD, kInstVPGATHERQD, kInstVPGATHERDQ, kInstVPGATHERQQ,

         // jitasm compiler instructions
         kInstCOMPILER_DECLARkEncodeREG_ARG, ///< Declare register argument
         kInstCOMPILER_DECLARkEncodeSTACK_ARG, ///< Declare stack argument
         kInstCOMPILER_DECLARkEncodeRESULT_REG, ///< Declare result register (eax/rax/xmm0)
         kInstCOMPILER_PROLOG, ///< Function prolog
         kInstCOMPILER_EPILOG ///< Function epilog
      };// enum InstrID

      enum JumpCondition
      {
         kJmpO, kJmpNO, kJmpB, kJmpAE, kJmpE, kJmpNE, kJmpBE, kJmpA, kJmpS, kJmpNS, kJmpP, kJmpNP, kJmpL, kJmpGE, kJmpLE, kJmpG,
         kJmpCXZ, kJmpECXZ, kJmpRCXZ,
      };// enum JumpCondition

      enum EncodingFlags
      {
         kEncodeSPECIAL = 1 << 0,
         kEncodeOPERAND_SIZkEncodePREFIX = 1 << 1, ///< Operand-size override prefix
         kEncodeREP_PREFIX = 1 << 2, ///< REP prefix
         kEncodeREXW_PREFIX = 1 << 3, ///< REX.W
         kEncodeMANDATORY_PREFIX_66 = 1 << 4, ///< Mandatory prefix 66
         kEncodeMANDATORY_PREFIX_F2 = 1 << 5, ///< Mandatory prefix F2
         kEncodeMANDATORY_PREFIX_F3 = 1 << 6, ///< Mandatory prefix F3
         kEncodeVEX = 1 << 7,
         kEncodeXOP = 1 << 8,
         kEncodeVEX_L = 1 << 9,
         kEncodeVEX_W = 1 << 10,
         kEncodeVEX_MMMMM_SHIFT = 11,
         kEncodeVEX_MMMMM_MASK = 0x1F << kEncodeVEX_MMMMM_SHIFT,
         kEncodeVEX_0F = 1 << kEncodeVEX_MMMMM_SHIFT,
         kEncodeVEX_0F38 = 2 << kEncodeVEX_MMMMM_SHIFT,
         kEncodeVEX_0F3A = 3 << kEncodeVEX_MMMMM_SHIFT,
         kEncodeXOP_M00011 = 3 << kEncodeVEX_MMMMM_SHIFT,
         kEncodeXOP_M01000 = 8 << kEncodeVEX_MMMMM_SHIFT,
         kEncodeXOP_M01001 = 9 << kEncodeVEX_MMMMM_SHIFT,
         kEncodeVEX_PP_SHIFT = 16,
         kEncodeVEX_PP_MASK = 0x3 << kEncodeVEX_PP_SHIFT,
         kEncodeVEX_66 = 1 << kEncodeVEX_PP_SHIFT,
         kEncodeVEX_F3 = 2 << kEncodeVEX_PP_SHIFT,
         kEncodeVEX_F2 = 3 << kEncodeVEX_PP_SHIFT,
         kEncodeXOP_P00 = 0 << kEncodeVEX_PP_SHIFT,
         kEncodeXOP_P01 = 1 << kEncodeVEX_PP_SHIFT,

         kEncodeVEX_128 = kEncodeVEX,
         kEncodeVEX_256 = kEncodeVEX | kEncodeVEX_L,
         kEncodeVEX_LIG = kEncodeVEX,
         kEncodeVEX_LZ = kEncodeVEX,
         kEncodeVEX_66_0F = kEncodeVEX_66 | kEncodeVEX_0F,
         kEncodeVEX_66_0F38 = kEncodeVEX_66 | kEncodeVEX_0F38,
         kEncodeVEX_66_0F3A = kEncodeVEX_66 | kEncodeVEX_0F3A,
         kEncodeVEX_F2_0F = kEncodeVEX_F2 | kEncodeVEX_0F,
         kEncodeVEX_F2_0F38 = kEncodeVEX_F2 | kEncodeVEX_0F38,
         kEncodeVEX_F2_0F3A = kEncodeVEX_F2 | kEncodeVEX_0F3A,
         kEncodeVEX_F3_0F = kEncodeVEX_F3 | kEncodeVEX_0F,
         kEncodeVEX_F3_0F38 = kEncodeVEX_F3 | kEncodeVEX_0F38,
         kEncodeVEX_F3_0F3A = kEncodeVEX_F3 | kEncodeVEX_0F3A,
         kEncodeVEX_W0 = 0,
         kEncodeVEX_W1 = kEncodeVEX_W,
         kEncodeVEX_WIG = 0,
         kEncodeXOP_128 = kEncodeXOP,
         kEncodeXOP_256 = kEncodeXOP | kEncodeVEX_L,
         kEncodeXOP_W0 = 0,
         kEncodeXOP_W1 = kEncodeVEX_W,

         // Aliases
         kEncodeVEX_128_0F_WIG = kEncodeVEX_128 | kEncodeVEX_0F | kEncodeVEX_WIG,
         kEncodeVEX_256_0F_WIG = kEncodeVEX_256 | kEncodeVEX_0F | kEncodeVEX_WIG,
         kEncodeVEX_128_66_0F_WIG = kEncodeVEX_128 | kEncodeVEX_66_0F | kEncodeVEX_WIG,
         kEncodeVEX_256_66_0F_WIG = kEncodeVEX_256 | kEncodeVEX_66_0F | kEncodeVEX_WIG,
         kEncodeVEX_128_66_0F38_WIG = kEncodeVEX_128 | kEncodeVEX_66_0F38 | kEncodeVEX_WIG,
         kEncodeVEX_256_66_0F38_WIG = kEncodeVEX_256 | kEncodeVEX_66_0F38 | kEncodeVEX_WIG,
         kEncodeVEX_128_66_0F38_W0 = kEncodeVEX_128 | kEncodeVEX_66_0F38 | kEncodeVEX_W0,
         kEncodeVEX_256_66_0F38_W0 = kEncodeVEX_256 | kEncodeVEX_66_0F38 | kEncodeVEX_W0,
         kEncodeVEX_128_66_0F38_W1 = kEncodeVEX_128 | kEncodeVEX_66_0F38 | kEncodeVEX_W1,
         kEncodeVEX_256_66_0F38_W1 = kEncodeVEX_256 | kEncodeVEX_66_0F38 | kEncodeVEX_W1,
         kEncodeVEX_128_66_0F3A_W0 = kEncodeVEX_128 | kEncodeVEX_66_0F3A | kEncodeVEX_W0,
         kEncodeVEX_256_66_0F3A_W0 = kEncodeVEX_256 | kEncodeVEX_66_0F3A | kEncodeVEX_W0,
      };// enum EncodingFlags

      /// Instruction
      struct Instr
      {
         //static const size_t _maxOperandCount = 6;
         InstrID _instrId; ///< Instruction ID
         boost::uint32_t _opcode; ///< Opcode
         boost::uint32_t _encodingFlags; ///< EncodingFlags
         //register_details::Operand _operand[_maxOperandCount]; ///< Operands

         Instr(const InstrID id,const boost::uint32_t opcode, boost::uint32_t encoding_flag
/*            ,const register_details::Operand& opd1 = register_details::Operand(), const register_details::Operand& opd2 = register_details::Operand()
            ,const register_details::Operand& opd3 = register_details::Operand(), const register_details::Operand& opd4 = register_details::Operand()
            ,const register_details::Operand& opd5 = register_details::Operand(), const register_details::Operand& opd6 = register_details::Operand()*/)
            :_instrId(id), _opcode(opcode), _encodingFlags(encoding_flag) 
         {
            //_operand[0] = opd1;
            //_operand[1] = opd2;
            //_operand[2] = opd3;
            //_operand[3] = opd4;
            //_operand[4] = opd5;
            //_operand[5] = opd6;
         }

         InstrID instrId()const
         {
            return _instrId;
         }
         //const register_details::Operand& GetOpd(size_t index) const {return opd_[index];}
         //register_details::Operand& GetOpd(size_t index) {return opd_[index];}
      };

   }// namespace instruction_details

}  // namespace odra
}  // namespace blib

#endif  // #ifndef __BLIB_ODRA__
