#ifndef __BLIB_ODRA__
#define __BLIB_ODRA__

#ifndef __BLIB_SYSTEM_INFkINCLUDED__
#include <blib/sys/system_info.hpp>
#endif

#ifndef __STD_STRINkInsGrpINCLUDED__
#define __STD_STRINkInsGrpINCLUDED__
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
         /** \var NUM_OkInsFlagPHYSICAL_REG
         * Number of physical register
         */
         /** \var SIZkEncodeOkInsFlagGP_REG
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
      enum EInstrID
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
         kInstCOMPILER_DECLARkEncodeREkInsGrpARG, ///< Declare register argument
         kInstCOMPILER_DECLARkEncodeSTACK_ARG, ///< Declare stack argument
         kInstCOMPILER_DECLARkEncodeRESULT_REG, ///< Declare result register (eax/rax/xmm0)
         kInstCOMPILER_PROLOG, ///< Function prolog
         kInstCOMPILER_EPILOG ///< Function epilog
      };// enum InstrID

      enum eJumpCondition
      {
         kJmpO, kJmpNO, kJmpB, kJmpAE, kJmpE, kJmpNE, kJmpBE, kJmpA, kJmpS, kJmpNS, kJmpP, kJmpNP, kJmpL, kJmpGE, kJmpLE, kJmpG,
         kJmpCXZ, kJmpECXZ, kJmpRCXZ,
      };// enum JumpCondition

      enum EEncodingFlags
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
         kEncodeVEX_128_0kInsFlagWIG = kEncodeVEX_128 | kEncodeVEX_0F | kEncodeVEX_WIG,
         kEncodeVEX_256_0kInsFlagWIG = kEncodeVEX_256 | kEncodeVEX_0F | kEncodeVEX_WIG,
         kEncodeVEX_128_66_0kInsFlagWIG = kEncodeVEX_128 | kEncodeVEX_66_0F | kEncodeVEX_WIG,
         kEncodeVEX_256_66_0kInsFlagWIG = kEncodeVEX_256 | kEncodeVEX_66_0F | kEncodeVEX_WIG,
         kEncodeVEX_128_66_0F38_WIG = kEncodeVEX_128 | kEncodeVEX_66_0F38 | kEncodeVEX_WIG,
         kEncodeVEX_256_66_0F38_WIG = kEncodeVEX_256 | kEncodeVEX_66_0F38 | kEncodeVEX_WIG,
         kEncodeVEX_128_66_0F38_W0 = kEncodeVEX_128 | kEncodeVEX_66_0F38 | kEncodeVEX_W0,
         kEncodeVEX_256_66_0F38_W0 = kEncodeVEX_256 | kEncodeVEX_66_0F38 | kEncodeVEX_W0,
         kEncodeVEX_128_66_0F38_W1 = kEncodeVEX_128 | kEncodeVEX_66_0F38 | kEncodeVEX_W1,
         kEncodeVEX_256_66_0F38_W1 = kEncodeVEX_256 | kEncodeVEX_66_0F38 | kEncodeVEX_W1,
         kEncodeVEX_128_66_0F3A_W0 = kEncodeVEX_128 | kEncodeVEX_66_0F3A | kEncodeVEX_W0,
         kEncodeVEX_256_66_0F3A_W0 = kEncodeVEX_256 | kEncodeVEX_66_0F3A | kEncodeVEX_W0,
      };// enum EncodingFlags


      // --------------------------------------------------------------------------
      // [Instruction Groups]
      // --------------------------------------------------------------------------

      //! @brief Instruction groups.
      //!
      //! This should be only used by assembler, because it's @c AsmJit::Assembler
      //! specific grouping. Each group represents one 'case' in the Assembler's 
      //! main emit method.
      enum EInstrGroup
      {
         // Group categories.
         kInstGrpEMIT,

         kInstGrpALU,
         kInstGrpBSWAP,
         kInstGrpBT,
         kInstGrpCALL,
         kInstGrpCRC32,
         kInstGrpENTER,
         kInstGrpIMUL,
         kInstGrpINC_DEC,
         kInstGrpJ,
         kInstGrpJMP,
         kInstGrpLEA,
         kInstGrpM,
         kInstGrpMOV,
         kInstGrpMOV_PTR,
         kInstGrpMOVSX_MOVZX,
         kInstGrpMOVSXD,
         kInstGrpPUSH, // I_PUSH is implemented before I_POP
         kInstGrpPOP,
         kInstGrpR_RM,
         kInstGrpRM_B,
         kInstGrpRM,
         kInstGrpRM_R,
         kInstGrpREP,
         kInstGrpRET,
         kInstGrpROT,
         kInstGrpSHLD_SHRD,
         kInstGrpTEST,
         kInstGrpXCHG,

         // Group for x87 FP instructions in format mem or st(i), st(i) (fadd, fsub, fdiv, ...)
         kInstGrpX87_FPU,
         // Group for x87 FP instructions in format st(i), st(i)
         kInstGrpX87_STI,
         // Group for fld/fst/fstp instruction, internally uses I_X87_MEM group.
         kInstGrpX87_MEM_STI,
         // Group for x87 FP instructions that uses Word, DWord, QWord or TWord memory pointer.
         kInstGrpX87_MEM,
         // Group for x87 FSTSW/FNSTSW instructions
         kInstGrpX87_FSTSW,

         // Group for movbe instruction
         kInstGrpMOVBE,

         // Group for MMX/SSE instructions in format (X)MM|Reg|Mem <- (X)MM|Reg|Mem,
         // 0x66 prefix must be set manually in opcodes.
         // - Primary opcode is used for instructions in (X)MM <- (X)MM/Mem format,
         // - Secondary opcode is used for instructions in (X)MM/Mem <- (X)MM format.
         kInstGrpMMU_MOV,

         // Group for movd and movq instructions.
         kInstGrpMMU_MOVD,
         kInstGrpMMU_MOVQ,

         // Group for pextrd, pextrq and pextrw instructions (it's special instruction
         // not similar to others)
         kInstGrpMMU_PEXTR,

         // Group for prefetch instruction
         kInstGrpMMU_PREFETCH,

         // Group for MMX/SSE instructions in format (X)MM|Reg <- (X)MM|Reg|Mem|Imm,
         // 0x66 prefix is added for MMX instructions that used by SSE2 registers.
         // - Primary opcode is used for instructions in (X)MM|Reg <- (X)MM|Reg|Mem format,
         // - Secondary opcode is iused for instructions in (X)MM|Reg <- Imm format.
         kInstGrpMMU_RMI,
         kInstGrpMMU_RM_IMM8,

         // Group for 3dNow instructions
         kInstGrpMMU_RM_3DNOW
      };

      // --------------------------------------------------------------------------
      // [Instruction Core Flags]
      // --------------------------------------------------------------------------

      //! @brief Instruction core flags.
      enum EInstrFlags
      {
         //! @brief No flags.
         kInstFlgNONE = 0x00,
         //! @brief Instruction is jump, conditional jump, call or ret.
         kInstFlgJUMP = 0x01,
         //! @brief Instruction will overwrite first operand - o[0].
         kInstFlgMOV = 0x02,
         //! @brief Instruction is X87 FPU.
         kInstFlgFPU = 0x04,
         //! @brief Instruction can be prepended using LOCK prefix
         //! (usable for multithreaded applications).
         kInstFlgLOCKABLE = 0x08,

         //! @brief Instruction is special, this is for @c Compiler.
         kInstFlgSPECIAL = 0x10,
         //! @brief Instruction always performs memory access.
         //!
         //! This flag is always combined with @c kInstFlgSPECIAL and signalizes that
         //! there is implicit address which is accessed (usually EDI/RDI or ESI/EDI).
         kInstFlgSPECIAL_MEM = 0x20
      };

      // --------------------------------------------------------------------------
      // [Instruction Operand Flags]
      // --------------------------------------------------------------------------

      //! @brief Instruction operand flags.
      enum EInstrOperand
      {
         // X86, MM, XMM
         kInstOprGB          = 0x0001,
         kInstOprGW          = 0x0002,
         kInstOprGD          = 0x0004,
         kInstOprGQ          = 0x0008,
         kInstOprMM          = 0x0010,
         kInstOprXMM         = 0x0020,
         kInstOprMEM         = 0x0040,
         kInstOprIMM         = 0x0080,


         kInstOprGB_MEM      = kInstOprGB    | kInstOprMEM,
         kInstOprGW_MEM      = kInstOprGW    | kInstOprMEM,
         kInstOprGD_MEM      = kInstOprGD    | kInstOprMEM,
         kInstOprGQ_MEM      = kInstOprGQ    | kInstOprMEM,

         kInstOprGQDWB       = kInstOprGQ    | kInstOprGD    | kInstOprGW    | kInstOprGB,
         kInstOprGQDW        = kInstOprGQ    | kInstOprGD    | kInstOprGW,
         kInstOprGQD         = kInstOprGQ    | kInstOprGD,
         kInstOprGWB         = kInstOprGW    | kInstOprGB,

         kInstOprGQDWB_MEM   = kInstOprGQDWB | kInstOprMEM,
         kInstOprGQDW_MEM    = kInstOprGQDW  | kInstOprMEM,
         kInstOprGQD_MEM     = kInstOprGQD   | kInstOprMEM,
         kInstOprGWB_MEM     = kInstOprGWB   | kInstOprMEM,

         kInstOprMM_MEM      = kInstOprMM    | kInstOprMEM,
         kInstOprXMM_MEM     = kInstOprXMM   | kInstOprMEM,
         kInstOprMM_XMM      = kInstOprMM    | kInstOprXMM,
         kInstOprMM_XMM_MEM  = kInstOprMM    | kInstOprXMM   | kInstOprMEM,

         // X87
         kInstOprFM_2        = kInstOprMEM   | 0x0100,
         kInstOprFM_4        = kInstOprMEM   | 0x0200,
         kInstOprFM_8        = kInstOprMEM   | 0x0400,
         kInstOprFM_10       = kInstOprMEM   | 0x0800,

         kInstOprFM_2_4      = kInstOprFM_2  | kInstOprFM_4,
         kInstOprFM_2_4_8    = kInstOprFM_2  | kInstOprFM_4  | kInstOprFM_8,
         kInstOprFM_4_8      = kInstOprFM_4  | kInstOprFM_8,
         kInstOprFM_4_8_10   = kInstOprFM_4  | kInstOprFM_8  | kInstOprFM_10,

         // Don't emit REX prefix.
         kInstOprNOREX       = 0x2000
      };
      // ============================================================================
      // [AsmJit::Instruction Description]
      // ============================================================================

      struct InstructionDescription
      {
         // --------------------------------------------------------------------------
         // [Members]
         // --------------------------------------------------------------------------

         //! @brief Instruction code.
         boost::uint16_t _code;
         //! @brief Instruction name index in instructionName[] array.
         boost::uint16_t _nameIndex;
         //! @brief Instruction group, used also by @c Compiler.
         boost::uint8_t _group;
         //! @brief Instruction type flags.
         boost::uint8_t _flags;
         //! @brief First and second operand flags (some groups depends on these settings, used also by @c Compiler).
         boost::uint16_t _oflags[2];
         //! @brief If instruction has only memory operand, this is register opcode.
         boost::uint16_t _opCodeR;
         //! @brief Primary and secondary opcodes.
         boost::uint32_t opCode[2];

         //! @brief Get the instruction name (null terminated string).
         const char* getName()const
         { 
            return _instructionName + _nameIndex;
         }

         //! @brief Get whether the instruction is conditional or standard jump.
         bool isJump()const 
         { 
            return (_flags & kInstFlgJUMP) != 0;
         }
         //! @brief Get whether the instruction is MOV type.
         bool isMov()const 
         { 
            return (_flags & kInstFlgMOV) != 0;
         }
         //! @brief Get whether the instruction is X87 FPU type.
         bool isFPU()const
         { 
            return (_flags & kInstFlgFPU) != 0;
         }
         //! @brief Get whether the instruction can be prefixed by LOCK prefix.
         bool isLockable()const
         { 
            return (_flags & kInstFlgLOCKABLE) != 0; 
         }

         //! @brief Get whether the instruction is special type (this is used by
         //! @c Compiler to manage additional variables or functionality).
         bool isSpecial()const 
         { 
            return (_flags & kInstFlgSPECIAL) != 0; 
         }
         //! @brief Get whether the instruction is special type and it performs
         //! memory access.
         bool isSpecialMem()const 
         { 
            return (_flags & kInstFlgSPECIAL_MEM) != 0; 
         }

         // ============================================================================
         // [Instruction Name]
         // ============================================================================

         // Following {DATA SECTION} is auto-generated using InstructionDescription data.
         //
         // ${INSTRUCTION_DATA_BEGIN}
         static const char* _instructionName;
      };
      const char* InstructionDescription::_instructionName = 
            "adc\0"
            "add\0"
            "addpd\0"
            "addps\0"
            "addsd\0"
            "addss\0"
            "addsubpd\0"
            "addsubps\0"
            "amd_prefetch\0"
            "amd_prefetchw\0"
            "and\0"
            "andnpd\0"
            "andnps\0"
            "andpd\0"
            "andps\0"
            "blendpd\0"
            "blendps\0"
            "blendvpd\0"
            "blendvps\0"
            "bsf\0"
            "bsr\0"
            "bswap\0"
            "bt\0"
            "btc\0"
            "btr\0"
            "bts\0"
            "call\0"
            "cbw\0"
            "cdqe\0"
            "clc\0"
            "cld\0"
            "clflush\0"
            "cmc\0"
            "cmova\0"
            "cmovae\0"
            "cmovb\0"
            "cmovbe\0"
            "cmovc\0"
            "cmove\0"
            "cmovg\0"
            "cmovge\0"
            "cmovl\0"
            "cmovle\0"
            "cmovna\0"
            "cmovnae\0"
            "cmovnb\0"
            "cmovnbe\0"
            "cmovnc\0"
            "cmovne\0"
            "cmovng\0"
            "cmovnge\0"
            "cmovnl\0"
            "cmovnle\0"
            "cmovno\0"
            "cmovnp\0"
            "cmovns\0"
            "cmovnz\0"
            "cmovo\0"
            "cmovp\0"
            "cmovpe\0"
            "cmovpo\0"
            "cmovs\0"
            "cmovz\0"
            "cmp\0"
            "cmppd\0"
            "cmpps\0"
            "cmpsd\0"
            "cmpss\0"
            "cmpxchg\0"
            "cmpxchg16b\0"
            "cmpxchg8b\0"
            "comisd\0"
            "comiss\0"
            "cpuid\0"
            "crc32\0"
            "cvtdq2pd\0"
            "cvtdq2ps\0"
            "cvtpd2dq\0"
            "cvtpd2pi\0"
            "cvtpd2ps\0"
            "cvtpi2pd\0"
            "cvtpi2ps\0"
            "cvtps2dq\0"
            "cvtps2pd\0"
            "cvtps2pi\0"
            "cvtsd2si\0"
            "cvtsd2ss\0"
            "cvtsi2sd\0"
            "cvtsi2ss\0"
            "cvtss2sd\0"
            "cvtss2si\0"
            "cvttpd2dq\0"
            "cvttpd2pi\0"
            "cvttps2dq\0"
            "cvttps2pi\0"
            "cvttsd2si\0"
            "cvttss2si\0"
            "cwde\0"
            "daa\0"
            "das\0"
            "dec\0"
            "div\0"
            "divpd\0"
            "divps\0"
            "divsd\0"
            "divss\0"
            "dppd\0"
            "dpps\0"
            "emms\0"
            "enter\0"
            "extractps\0"
            "f2xm1\0"
            "fabs\0"
            "fadd\0"
            "faddp\0"
            "fbld\0"
            "fbstp\0"
            "fchs\0"
            "fclex\0"
            "fcmovb\0"
            "fcmovbe\0"
            "fcmove\0"
            "fcmovnb\0"
            "fcmovnbe\0"
            "fcmovne\0"
            "fcmovnu\0"
            "fcmovu\0"
            "fcom\0"
            "fcomi\0"
            "fcomip\0"
            "fcomp\0"
            "fcompp\0"
            "fcos\0"
            "fdecstp\0"
            "fdiv\0"
            "fdivp\0"
            "fdivr\0"
            "fdivrp\0"
            "femms\0"
            "ffree\0"
            "fiadd\0"
            "ficom\0"
            "ficomp\0"
            "fidiv\0"
            "fidivr\0"
            "fild\0"
            "fimul\0"
            "fincstp\0"
            "finit\0"
            "fist\0"
            "fistp\0"
            "fisttp\0"
            "fisub\0"
            "fisubr\0"
            "fld\0"
            "fld1\0"
            "fldcw\0"
            "fldenv\0"
            "fldl2e\0"
            "fldl2t\0"
            "fldlg2\0"
            "fldln2\0"
            "fldpi\0"
            "fldz\0"
            "fmul\0"
            "fmulp\0"
            "fnclex\0"
            "fninit\0"
            "fnop\0"
            "fnsave\0"
            "fnstcw\0"
            "fnstenv\0"
            "fnstsw\0"
            "fpatan\0"
            "fprem\0"
            "fprem1\0"
            "fptan\0"
            "frndint\0"
            "frstor\0"
            "fsave\0"
            "fscale\0"
            "fsin\0"
            "fsincos\0"
            "fsqrt\0"
            "fst\0"
            "fstcw\0"
            "fstenv\0"
            "fstp\0"
            "fstsw\0"
            "fsub\0"
            "fsubp\0"
            "fsubr\0"
            "fsubrp\0"
            "ftst\0"
            "fucom\0"
            "fucomi\0"
            "fucomip\0"
            "fucomp\0"
            "fucompp\0"
            "fwait\0"
            "fxam\0"
            "fxch\0"
            "fxrstor\0"
            "fxsave\0"
            "fxtract\0"
            "fyl2x\0"
            "fyl2xp1\0"
            "haddpd\0"
            "haddps\0"
            "hsubpd\0"
            "hsubps\0"
            "idiv\0"
            "imul\0"
            "inc\0"
            "int3\0"
            "ja\0"
            "jae\0"
            "jb\0"
            "jbe\0"
            "jc\0"
            "je\0"
            "jg\0"
            "jge\0"
            "jl\0"
            "jle\0"
            "jna\0"
            "jnae\0"
            "jnb\0"
            "jnbe\0"
            "jnc\0"
            "jne\0"
            "jng\0"
            "jnge\0"
            "jnl\0"
            "jnle\0"
            "jno\0"
            "jnp\0"
            "jns\0"
            "jnz\0"
            "jo\0"
            "jp\0"
            "jpe\0"
            "jpo\0"
            "js\0"
            "jz\0"
            "jmp\0"
            "lddqu\0"
            "ldmxcsr\0"
            "lahf\0"
            "lea\0"
            "leave\0"
            "lfence\0"
            "maskmovdqu\0"
            "maskmovq\0"
            "maxpd\0"
            "maxps\0"
            "maxsd\0"
            "maxss\0"
            "mfence\0"
            "minpd\0"
            "minps\0"
            "minsd\0"
            "minss\0"
            "monitor\0"
            "mov\0"
            "movapd\0"
            "movaps\0"
            "movbe\0"
            "movd\0"
            "movddup\0"
            "movdq2q\0"
            "movdqa\0"
            "movdqu\0"
            "movhlps\0"
            "movhpd\0"
            "movhps\0"
            "movlhps\0"
            "movlpd\0"
            "movlps\0"
            "movmskpd\0"
            "movmskps\0"
            "movntdq\0"
            "movntdqa\0"
            "movnti\0"
            "movntpd\0"
            "movntps\0"
            "movntq\0"
            "movq\0"
            "movq2dq\0"
            "movsd\0"
            "movshdup\0"
            "movsldup\0"
            "movss\0"
            "movsx\0"
            "movsxd\0"
            "movupd\0"
            "movups\0"
            "movzx\0"
            "mov_ptr\0"
            "mpsadbw\0"
            "mul\0"
            "mulpd\0"
            "mulps\0"
            "mulsd\0"
            "mulss\0"
            "mwait\0"
            "neg\0"
            "nop\0"
            "not\0"
            "or\0"
            "orpd\0"
            "orps\0"
            "pabsb\0"
            "pabsd\0"
            "pabsw\0"
            "packssdw\0"
            "packsswb\0"
            "packusdw\0"
            "packuswb\0"
            "paddb\0"
            "paddd\0"
            "paddq\0"
            "paddsb\0"
            "paddsw\0"
            "paddusb\0"
            "paddusw\0"
            "paddw\0"
            "palignr\0"
            "pand\0"
            "pandn\0"
            "pause\0"
            "pavgb\0"
            "pavgw\0"
            "pblendvb\0"
            "pblendw\0"
            "pcmpeqb\0"
            "pcmpeqd\0"
            "pcmpeqq\0"
            "pcmpeqw\0"
            "pcmpestri\0"
            "pcmpestrm\0"
            "pcmpgtb\0"
            "pcmpgtd\0"
            "pcmpgtq\0"
            "pcmpgtw\0"
            "pcmpistri\0"
            "pcmpistrm\0"
            "pextrb\0"
            "pextrd\0"
            "pextrq\0"
            "pextrw\0"
            "pf2id\0"
            "pf2iw\0"
            "pfacc\0"
            "pfadd\0"
            "pfcmpeq\0"
            "pfcmpge\0"
            "pfcmpgt\0"
            "pfmax\0"
            "pfmin\0"
            "pfmul\0"
            "pfnacc\0"
            "pfpnacc\0"
            "pfrcp\0"
            "pfrcpit1\0"
            "pfrcpit2\0"
            "pfrsqit1\0"
            "pfrsqrt\0"
            "pfsub\0"
            "pfsubr\0"
            "phaddd\0"
            "phaddsw\0"
            "phaddw\0"
            "phminposuw\0"
            "phsubd\0"
            "phsubsw\0"
            "phsubw\0"
            "pi2fd\0"
            "pi2fw\0"
            "pinsrb\0"
            "pinsrd\0"
            "pinsrq\0"
            "pinsrw\0"
            "pmaddubsw\0"
            "pmaddwd\0"
            "pmaxsb\0"
            "pmaxsd\0"
            "pmaxsw\0"
            "pmaxub\0"
            "pmaxud\0"
            "pmaxuw\0"
            "pminsb\0"
            "pminsd\0"
            "pminsw\0"
            "pminub\0"
            "pminud\0"
            "pminuw\0"
            "pmovmskb\0"
            "pmovsxbd\0"
            "pmovsxbq\0"
            "pmovsxbw\0"
            "pmovsxdq\0"
            "pmovsxwd\0"
            "pmovsxwq\0"
            "pmovzxbd\0"
            "pmovzxbq\0"
            "pmovzxbw\0"
            "pmovzxdq\0"
            "pmovzxwd\0"
            "pmovzxwq\0"
            "pmuldq\0"
            "pmulhrsw\0"
            "pmulhuw\0"
            "pmulhw\0"
            "pmulld\0"
            "pmullw\0"
            "pmuludq\0"
            "pop\0"
            "popad\0"
            "popcnt\0"
            "popfd\0"
            "popfq\0"
            "por\0"
            "prefetch\0"
            "psadbw\0"
            "pshufb\0"
            "pshufd\0"
            "pshufw\0"
            "pshufhw\0"
            "pshuflw\0"
            "psignb\0"
            "psignd\0"
            "psignw\0"
            "pslld\0"
            "pslldq\0"
            "psllq\0"
            "psllw\0"
            "psrad\0"
            "psraw\0"
            "psrld\0"
            "psrldq\0"
            "psrlq\0"
            "psrlw\0"
            "psubb\0"
            "psubd\0"
            "psubq\0"
            "psubsb\0"
            "psubsw\0"
            "psubusb\0"
            "psubusw\0"
            "psubw\0"
            "pswapd\0"
            "ptest\0"
            "punpckhbw\0"
            "punpckhdq\0"
            "punpckhqdq\0"
            "punpckhwd\0"
            "punpcklbw\0"
            "punpckldq\0"
            "punpcklqdq\0"
            "punpcklwd\0"
            "push\0"
            "pushad\0"
            "pushfd\0"
            "pushfq\0"
            "pxor\0"
            "rcl\0"
            "rcpps\0"
            "rcpss\0"
            "rcr\0"
            "rdtsc\0"
            "rdtscp\0"
            "rep lodsb\0"
            "rep lodsd\0"
            "rep lodsq\0"
            "rep lodsw\0"
            "rep movsb\0"
            "rep movsd\0"
            "rep movsq\0"
            "rep movsw\0"
            "rep stosb\0"
            "rep stosd\0"
            "rep stosq\0"
            "rep stosw\0"
            "repe cmpsb\0"
            "repe cmpsd\0"
            "repe cmpsq\0"
            "repe cmpsw\0"
            "repe scasb\0"
            "repe scasd\0"
            "repe scasq\0"
            "repe scasw\0"
            "repne cmpsb\0"
            "repne cmpsd\0"
            "repne cmpsq\0"
            "repne cmpsw\0"
            "repne scasb\0"
            "repne scasd\0"
            "repne scasq\0"
            "repne scasw\0"
            "ret\0"
            "rol\0"
            "ror\0"
            "roundpd\0"
            "roundps\0"
            "roundsd\0"
            "roundss\0"
            "rsqrtps\0"
            "rsqrtss\0"
            "sahf\0"
            "sal\0"
            "sar\0"
            "sbb\0"
            "seta\0"
            "setae\0"
            "setb\0"
            "setbe\0"
            "setc\0"
            "sete\0"
            "setg\0"
            "setge\0"
            "setl\0"
            "setle\0"
            "setna\0"
            "setnae\0"
            "setnb\0"
            "setnbe\0"
            "setnc\0"
            "setne\0"
            "setng\0"
            "setnge\0"
            "setnl\0"
            "setnle\0"
            "setno\0"
            "setnp\0"
            "setns\0"
            "setnz\0"
            "seto\0"
            "setp\0"
            "setpe\0"
            "setpo\0"
            "sets\0"
            "setz\0"
            "sfence\0"
            "shl\0"
            "shld\0"
            "shr\0"
            "shrd\0"
            "shufpd\0"
            "shufps\0"
            "sqrtpd\0"
            "sqrtps\0"
            "sqrtsd\0"
            "sqrtss\0"
            "stc\0"
            "std\0"
            "stmxcsr\0"
            "sub\0"
            "subpd\0"
            "subps\0"
            "subsd\0"
            "subss\0"
            "test\0"
            "ucomisd\0"
            "ucomiss\0"
            "ud2\0"
            "unpckhpd\0"
            "unpckhps\0"
            "unpcklpd\0"
            "unpcklps\0"
            "xadd\0"
            "xchg\0"
            "xor\0"
            "xorpd\0"
            "xorps\0"
            ;
      /// Instruction
      struct Instr
      {
         //static const size_t _maxOperandCount = 6;
         EInstrID _instrId; ///< Instruction ID
         boost::uint32_t _opcode; ///< Opcode
         boost::uint32_t _encodingFlags; ///< EncodingFlags
         //register_details::Operand _operand[_maxOperandCount]; ///< Operands

         Instr(const EInstrID id,const boost::uint32_t opcode, boost::uint32_t encodinkInsGrpflag
            /*            ,const register_details::Operand& opd1 = register_details::Operand(), const register_details::Operand& opd2 = register_details::Operand()
            ,const register_details::Operand& opd3 = register_details::Operand(), const register_details::Operand& opd4 = register_details::Operand()
            ,const register_details::Operand& opd5 = register_details::Operand(), const register_details::Operand& opd6 = register_details::Operand()*/)
            :_instrId(id), _opcode(opcode), _encodingFlags(encodinkInsGrpflag) 
         {
            //_operand[0] = opd1;
            //_operand[1] = opd2;
            //_operand[2] = opd3;
            //_operand[3] = opd4;
            //_operand[4] = opd5;
            //_operand[5] = opd6;
         }

         EInstrID instrId()const
         {
            return _instrId;
         }
         boost::uint32_t opcode()const
         {
            return _opcode;
         }

         boost::uint32_t encodingFlags()const
         {
            return _encodingFlags;
         }
         //const register_details::Operand& GetOpd(size_t index) const {return opd_[index];}
         //register_details::Operand& GetOpd(size_t index) {return opd_[index];}
      };

   }// namespace instruction_details

}  // namespace odra
}  // namespace blib

#endif  // #ifndef __BLIB_ODRA__
