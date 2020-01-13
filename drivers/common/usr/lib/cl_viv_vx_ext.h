


#ifndef _GC_VX_H
#define _GC_VX_H 1

#ifdef _VIV_VX_EXTENSION

#pragma OPENCL EXTENSION  CL_VIV_asm : enable

#ifndef VX_VERSION
#define VX_VERSION 1   
#endif

typedef enum _VXC_FilterMode
{
    VXC_FM_BOX      = 0,
    VXC_FM_Guassian = 1,
    VXC_FM_SobelX   = 2,
    VXC_FM_SobelY   = 3,
    VXC_FM_ScharrX  = 4,
    VXC_FM_ScharrY  = 5,
    VXC_FM_Max      = 8,
    VXC_FM_Min      = 9,
    VXC_FM_Median   = 10
} vxc_filter_mode;

typedef enum _VXC_RoundMode
{
    VXC_RM_Truncate      = 0,    
    VXC_RM_TowardZero    = 0,    
    VXC_RM_TowardInf     = 1,
    VXC_RM_ToNearestEven = 2
} vxc_round_mode;

typedef enum _VXC_ScatteredOffsetType
{
    VXC_OFFSET_UNSIGNED32   = 0,
    VXC_OFFSET_SIGNED32     = 1,
    VXC_OFFSET_UNSIGNED16   = 2,
    VXC_OFFSET_SIGNED16     = 3,
    VXC_OFFSET_UNSIGNED8    = 4,
    VXC_OFFSET_SIGNED8      = 5,
} VXC_ScatteredOffsetType;

typedef enum _VXC_AtomicOp
{
    VXC_ATOMIC_OP_ADD       = 0,
    VXC_ATOMIC_OP_MIN       = 1,
    VXC_ATOMIC_OP_MAX       = 2,
    VXC_ATOMIC_OP_OR        = 3,
    VXC_ATOMIC_OP_AND       = 4,
    VXC_ATOMIC_OP_XOR       = 5,
    VXC_ATOMIC_OP_XCHG      = 6,
}VXC_AtomicOpType;

#define VXC_CLAMP_BITMASK           0x00400000     
#define VXC_PREADJ_BITMASK          0x00200000     
#define VXC_RANGEPI_BITMASK         0x00100000     
#define VXC_FILTER_BITMASK          0x000F0000     
#define VXC_START_BIN_BITMASK       0x0000F000     
#define VXC_END_BIN_BITMASK         0x00000F00     
#define VXC_SOURCE_BIN_BITMASK      0x000000F0     
#define VXC_ROUNDING_MODE_BITMASK   0x0000000C     
#define VXC_ENABLEBOOL_BITMASK      0x00000002     
#define VXC_SIGNEXT_BITMASK         0x00000001     


#define VXC_OFFSET_TYPE_BITMASK     0x00070000     
#define VXC_OFFSET_TYPE_SHIFT       16             


#define VXC_ATOM_OP_BITMASK         0x00380000     
#define VXC_ATOM_OP_SHIFT           19             

#define VXC_MODIFIER(StartBin, EndBin, SourceBin, RoundingMode, Clamp)    \
         (                                                                \
          (((Clamp) << 22)&VXC_CLAMP_BITMASK)          |                  \
          (((StartBin) << 12)&VXC_START_BIN_BITMASK)   |                  \
          (((EndBin) << 8)&VXC_END_BIN_BITMASK)        |                  \
          (((SourceBin) << 4)&VXC_SOURCE_BIN_BITMASK)  |                  \
          ((RoundingMode << 2)&VXC_ROUNDING_MODE_BITMASK)                 \
         )

#define VXC_MODIFIER_SIGNEXT(StartBin, EndBin, SourceBin, SignExt, Clamp) \
         (                                                                \
          (((Clamp) << 22)&VXC_CLAMP_BITMASK)          |                  \
          ((SignExt)&VXC_SIGNEXT_BITMASK)              |                  \
          (((StartBin) << 12)&VXC_START_BIN_BITMASK)   |                  \
          (((EndBin) << 8)&VXC_END_BIN_BITMASK)        |                  \
          (((SourceBin) << 4)&VXC_SOURCE_BIN_BITMASK)                     \
         )

#define VXC_MODIFIER_MAGPHASE(StartBin, EndBin, SourceBin, NoPreAdjust, RangePi)  \
         (                                                                \
          (VXC_CLAMP_BITMASK)                          |                  \
          (((RangePi) << 20)&VXC_RANGEPI_BITMASK)      |                  \
          (((NoPreAdjust) << 21)&VXC_PREADJ_BITMASK)   |                  \
          (((StartBin) << 12)&VXC_START_BIN_BITMASK)   |                  \
          (((EndBin) << 8)&VXC_END_BIN_BITMASK)        |                  \
          (((SourceBin) << 4)&VXC_SOURCE_BIN_BITMASK)                     \
         )

#define VXC_MODIFIER_CLAMP(StartBin, EndBin, SourceBin, EnableBool)       \
         (                                                                \
          (((EnableBool) << 1)&VXC_ENABLEBOOL_BITMASK)   |                \
          (((StartBin) << 12)&VXC_START_BIN_BITMASK)     |                \
          (((EndBin) << 8)&VXC_END_BIN_BITMASK)          |                \
          (((SourceBin) << 4)&VXC_SOURCE_BIN_BITMASK)                     \
         )

#define VXC_MODIFIER_FILTER(StartBin, EndBin, SourceBin, Filter, Clamp)   \
         (                                                                \
          (((Clamp) << 22)&VXC_CLAMP_BITMASK)            |                \
          (((Filter) << 16)&VXC_FILTER_BITMASK)          |                \
          (((StartBin) << 12)&VXC_START_BIN_BITMASK)     |                \
          (((EndBin) << 8)&VXC_END_BIN_BITMASK)          |                \
          (((SourceBin) << 4)&VXC_SOURCE_BIN_BITMASK)                     \
         )

#define VXC_MODIFIER_BIN(StartBin, EndBin, Clamp)                         \
         (                                                                \
          (((Clamp) << 22)&VXC_CLAMP_BITMASK)          |                  \
          (((StartBin) << 12)&VXC_START_BIN_BITMASK)   |                  \
          (((EndBin) << 8)&VXC_END_BIN_BITMASK)                           \
         )

#define VXC_MODIFIER_GATHER(StartBin, EndBin, SourceBin, OffsetType)                \
         (                                                                          \
          (((OffsetType) << VXC_OFFSET_TYPE_SHIFT)&VXC_OFFSET_TYPE_BITMASK)  |      \
          (((StartBin) << 12)&VXC_START_BIN_BITMASK)     |                          \
          (((EndBin) << 8)&VXC_END_BIN_BITMASK)          |                          \
          (((SourceBin) << 4)&VXC_SOURCE_BIN_BITMASK)                               \
         )

#define VXC_MODIFIER_SCATTER(StartBin, EndBin, SourceBin, OffsetType)               \
         (                                                                          \
          (((OffsetType) << VXC_OFFSET_TYPE_SHIFT)&VXC_OFFSET_TYPE_BITMASK)  |      \
          (((StartBin) << 12)&VXC_START_BIN_BITMASK)     |                          \
          (((EndBin) << 8)&VXC_END_BIN_BITMASK)          |                          \
          (((SourceBin) << 4)&VXC_SOURCE_BIN_BITMASK)                               \
         )

#define VXC_MODIFIER_ATOMIC_S(StartBin, EndBin, SourceBin, OffsetType, AtomOp)      \
         (                                                                          \
          (((OffsetType) << VXC_OFFSET_TYPE_SHIFT)&VXC_OFFSET_TYPE_BITMASK)  |      \
          (((AtomOp) << VXC_ATOM_OP_SHIFT)&VXC_ATOM_OP_BITMASK)              |      \
          (((StartBin) << 12)&VXC_START_BIN_BITMASK)     |                          \
          (((EndBin) << 8)&VXC_END_BIN_BITMASK)          |                          \
          (((SourceBin) << 4)&VXC_SOURCE_BIN_BITMASK)                               \
         )


#define VXC_MODIFIER_SetDestClamp(VxModifier, Clamp)   ((VxModifier) | (((Clamp) << 22)&VXC_CLAMP_BITMASK))

#define VXC_DEFAULT_MODIFIER   (-1)

typedef unsigned int vxc_modifier;


typedef _viv_char2_packed     vxc_char2;
typedef _viv_char4_packed     vxc_char4;
typedef _viv_char8_packed     vxc_char8;
typedef _viv_char16_packed    vxc_char16;
typedef struct _vxc_char32
{
    vxc_char16  hi;
    vxc_char16  lo;
} vxc_char32;


typedef _viv_uchar2_packed    vxc_uchar2;
typedef _viv_uchar4_packed    vxc_uchar4;
typedef _viv_uchar8_packed    vxc_uchar8;
typedef _viv_uchar16_packed   vxc_uchar16;
typedef struct _vxc_uchar32
{
    vxc_uchar16  hi;
    vxc_uchar16  lo;
} vxc_uchar32;


typedef _viv_short2_packed    vxc_short2;
typedef _viv_short4_packed    vxc_short4;
typedef _viv_short8_packed    vxc_short8;
typedef struct _vxc_short16
{
    vxc_short8  hi;
    vxc_short8  lo;
} vxc_short16;


typedef _viv_ushort2_packed   vxc_ushort2;
typedef _viv_ushort4_packed   vxc_ushort4;
typedef _viv_ushort8_packed   vxc_ushort8;
typedef struct _vxc_ushort16
{
    vxc_ushort8  hi;
    vxc_ushort8  lo;
} vxc_ushort16;


typedef int      vxc_int;
typedef int2     vxc_int2;
typedef int4     vxc_int4;
typedef int8     vxc_int8;
typedef int16    vxc_int16;


typedef uint     vxc_uint;
typedef uint2    vxc_uint2;
typedef uint4    vxc_uint4;
typedef uint8    vxc_uint8;
typedef uint16   vxc_uint16;


typedef float     vxc_float;
typedef float2    vxc_float2;
typedef float4    vxc_float4;
typedef float8    vxc_float8;
typedef float16   vxc_float16;


typedef half                  vxc_half;
typedef _viv_half2_packed     vxc_half2;
typedef _viv_half4_packed     vxc_half4;
typedef _viv_half8_packed     vxc_half8;
typedef struct _vxc_half16
{
    vxc_half8  hi;
    vxc_half8  lo;
} vxc_half16;

typedef uint16 vxc_512bits;
typedef uint4  vxc_128bits;

typedef vxc_512bits VXC_512Bits;
typedef vxc_128bits VXC_128Bits;
typedef vxc_modifier VXC_Modifier_t ;
typedef vxc_round_mode VXC_RoundMode;
typedef vxc_filter_mode VXC_FilterMode;

#ifndef VX_USE_INTRINSIC
#define VX_USE_INTRINSIC 0    
#endif

enum VXC_OP {
    VXC_OP_abs_diff = 3, 
    VXC_OP_iadd,
    VXC_OP_iacc_sq,
    VXC_OP_lerp,
    VXC_OP_filter,
    VXC_OP_mag_phase,
    VXC_OP_mul_shift,
    VXC_OP_dp16x1,
    VXC_OP_dp8x2,
    VXC_OP_dp4x4,
    VXC_OP_dp2x8,
    VXC_OP_clamp,
    VXC_OP_bi_linear,
    VXC_OP_select_add,
    VXC_OP_atomic_add,
    VXC_OP_bit_extract,
    VXC_OP_bit_replace,
    VXC_OP_dp32x1,
    VXC_OP_dp16x2,
    VXC_OP_dp8x4,
    VXC_OP_dp4x8,
    VXC_OP_dp2x16,
    VXC_OP_dp32x1_b,
    VXC_OP_dp16x2_b,
    VXC_OP_dp8x4_b,
    VXC_OP_dp4x8_b,
    VXC_OP_dp2x16_b,
    VXC_OP_img_load,
    VXC_OP_img_load_3d,
    VXC_OP_img_store,
    VXC_OP_img_store_3d,
    VXC_OP_vload2,
    VXC_OP_vload3,
    VXC_OP_vload4,
    VXC_OP_vload8,
    VXC_OP_vload16,
    VXC_OP_vstore2,
    VXC_OP_vstore3,
    VXC_OP_vstore4,
    VXC_OP_vstore8,
    VXC_OP_vstore16,
    VXC_OP_index_add,
    VXC_OP_vert_min3,
    VXC_OP_vert_max3,
    VXC_OP_vert_med3,
    VXC_OP_horz_min3,
    VXC_OP_horz_max3,
    VXC_OP_horz_med3,
    VXC_OP_error,
    OP_bit_extract,
    VXC_OP_dp16x1_b,
    VXC_OP_dp8x2_b,
    VXC_OP_dp4x4_b,
    VXC_OP_dp2x8_b,
    VXC_OP_gather,
    VXC_OP_gather_b,
    VXC_OP_scatter,
    VXC_OP_scatter_b,
    VXC_OP_atomic_s,
    VXC_OP_atomic_s_b,
};

enum eVXC_ERROR
{
    ERROR_DP2x16_NOT_SUPPORTED,
    ERROR_IADD_NOT_SUPPORTED,
    ERROR_SELECTADD_NOT_SUPPORTED,
    ERROR_BITREPLACE_NOT_SUPPORTED
};

#define VXC_OP1(Op, Dest, Src0)   _viv_asm(INTRINSIC, Dest, VXC_OP_##Op, Src0)

#define VXC_OP2(Op, Dest, Src0, Src1)                  \
    do {                                               \
        int _t1;                                       \
        _viv_asm(PARAM_CHAIN, _t1, Src0, Src1);         \
        _viv_asm(INTRINSIC, Dest, VXC_OP_##Op, _t1);    \
    } while(0)

#define VXC_OP3(Op, Dest, Src0, Src1, Src2)            \
    do {                                               \
        int _t1, _t2;                                  \
        _viv_asm(PARAM_CHAIN, _t1, Src0, Src1);        \
        _viv_asm(PARAM_CHAIN, _t2, _t1, Src2);         \
        _viv_asm(INTRINSIC, Dest, VXC_OP_##Op, _t2);   \
    } while(0)

#define VXC_OP3_NoDest(Op, Src0, Src1, Src2)           \
    do {                                               \
        int _t1, _t2, _t3;                             \
        _viv_asm(PARAM_CHAIN, _t1, Src0, Src1);        \
        _viv_asm(PARAM_CHAIN, _t2, _t1, Src2);         \
        _viv_asm(INTRINSIC_ST, _t3, VXC_OP_##Op, _t2); \
    } while(0)


#define VXC_OP4(Op, Dest, Src0, Src1, Src2, Src3)      \
    do {                                               \
        int _t1, _t2, _t3;                             \
        _viv_asm(PARAM_CHAIN, _t1, Src0, Src1);        \
        _viv_asm(PARAM_CHAIN, _t2, _t1, Src2);         \
        _viv_asm(PARAM_CHAIN, _t3, _t2, Src3);         \
        _viv_asm(INTRINSIC, Dest, VXC_OP_##Op, _t3);   \
    } while(0)

#define VXC_OP4_NoDest(Op, Src0, Src1, Src2, Src3)     \
    do {                                               \
        int _t1, _t2, _t3, _t4;                        \
        _viv_asm(PARAM_CHAIN, _t1, Src0, Src1);        \
        _viv_asm(PARAM_CHAIN, _t2, _t1, Src2);         \
        _viv_asm(PARAM_CHAIN, _t3, _t2, Src3);         \
        _viv_asm(INTRINSIC_ST, _t4, VXC_OP_##Op, _t3); \
    } while(0)

#define VXC_OP4_ST(Op, Dest, Src0, Src1, Src2, Src3)   \
    do {                                               \
        int _t1, _t2, _t3;                             \
        _viv_asm(PARAM_CHAIN, _t1, Src0, Src1);        \
        _viv_asm(PARAM_CHAIN, _t2, _t1, Src2);         \
        _viv_asm(PARAM_CHAIN, _t3, _t2, Src3);         \
        _viv_asm(INTRINSIC_ST, Dest, VXC_OP_##Op, _t3);\
    } while(0)

#define VXC_OP5(Op, Dest, Src0, Src1, Src2, Src3, Src4)   \
    do {                                                  \
        int _t1, _t2, _t3, _t4;                           \
        _viv_asm(PARAM_CHAIN, _t1, Src0, Src1);           \
        _viv_asm(PARAM_CHAIN, _t2, _t1, Src2);            \
        _viv_asm(PARAM_CHAIN, _t3, _t2, Src3);            \
        _viv_asm(PARAM_CHAIN, _t4, _t3, Src4);            \
        _viv_asm(INTRINSIC, Dest, VXC_OP_##Op, _t4);      \
    } while(0)

#define VXC_OP5_NoDest(Op, Src0, Src1, Src2, Src3, Src4)  \
    do {                                                  \
        int _t1, _t2, _t3, _t4, _t5;                      \
        _viv_asm(PARAM_CHAIN, _t1, Src0, Src1);           \
        _viv_asm(PARAM_CHAIN, _t2, _t1, Src2);            \
        _viv_asm(PARAM_CHAIN, _t3, _t2, Src3);            \
        _viv_asm(PARAM_CHAIN, _t4, _t3, Src4);            \
        _viv_asm(INTRINSIC_ST, _t5, VXC_OP_##Op, _t4);    \
    } while(0)


#define VXC_5BITOFFSET_XY(offsetX, offsetY)  ((((offsetY) & 0x1F) << 5) | ((offsetX) & 0x1F))



#if !VX_USE_INTRINSIC
#define VXC_AbsDiff(Dest, Src0, Src1, Info)         VXC_OP3(abs_diff, Dest, Src0, Src1, Info)
#define VXC_IAccSq(Dest, Src0, Src1, Imm, Info)     VXC_OP4(iacc_sq, Dest, Src0, Src1, Imm, Info)
#define VXC_Lerp(Dest, Src0, Src1, Src2, Info)      VXC_OP4(lerp, Dest, Src0, Src1, Src2, Info)

#define VXC_MulShift(Dest, Src0, Src1, Imm, Info)      VXC_OP4(mul_shift, Dest, Src0, Src1, Imm, Info)
#define VXC_Clamp(Dest, Src0, Src1, Src2, Info)        VXC_OP4(clamp, Dest, Src0, Src1, Src2, Info)
#define VXC_AtomicAdd(Dest, Base, Offset, Data, Info)  VXC_OP4_ST(atomic_add, Dest, Base, Offset, Data, Info)
#define VXC_BitExtract(Dest, Src0, Src1, Src2, Info)   VXC_OP4(bit_extract, Dest, Src0, Src1, Src2, Info)

#define VXC_DP16x1(Dest, Src0, Src1, Info, U512)    VXC_OP4(dp16x1, Dest, Src0, Src1, Info, U512)
#define VXC_DP8x2(Dest, Src0, Src1, Info, U512)     VXC_OP4(dp8x2, Dest, Src0, Src1, Info, U512)
#define VXC_DP4x4(Dest, Src0, Src1, Info, U512)     VXC_OP4(dp4x4, Dest, Src0, Src1, Info, U512)
#define VXC_DP2x8(Dest, Src0, Src1, Info, U512)     VXC_OP4(dp2x8, Dest, Src0, Src1, Info, U512)

#define VXC_DP32x1(Dest, Src0, Src1, Info, U512)    VXC_OP4(dp32x1, Dest, Src0, Src1, Info, U512)
#define VXC_DP16x2(Dest, Src0, Src1, Info, U512)    VXC_OP4(dp16x2, Dest, Src0, Src1, Info, U512)
#define VXC_DP8x4(Dest, Src0, Src1, Info, U512)     VXC_OP4(dp8x4,  Dest, Src0, Src1, Info, U512)
#define VXC_DP4x8(Dest, Src0, Src1, Info, U512)     VXC_OP4(dp4x8,  Dest, Src0, Src1, Info, U512)
#if (VX_VERSION >= 2)
#define VXC_DP2x16(Dest, Src0, Src1, Info, U512)    VXC_OP1(error, ERROR_DP2x16_NOT_SUPPORTED)
#else
#define VXC_DP2x16(Dest, Src0, Src1, Info, U512)    VXC_OP4(dp2x16, Dest, Src0, Src1, Info, U512)
#endif

#if (VX_VERSION >= 2)

#define VXC_DP16x1_b(Dest, Src0, Src1, Src2, Info, U512)    VXC_OP5(dp16x1_b, Dest, Src0, Src1, Src2, Info, U512)
#define VXC_DP8x2_b(Dest, Src0, Src1, Src2, Info, U512)     VXC_OP5(dp8x2_b, Dest, Src0, Src1, Src2, Info, U512)
#define VXC_DP4x4_b(Dest, Src0, Src1, Src2, Info, U512)     VXC_OP5(dp4x4_b,  Dest, Src0, Src1, Src2, Info, U512)
#define VXC_DP2x8_b(Dest, Src0, Src1, Src2, Info, U512)     VXC_OP5(dp2x8_b,  Dest, Src0, Src1, Src2, Info, U512)
#endif


#define VXC_DP32x1_b(Dest, Src0, Src1, Src2, Info, U512)    VXC_OP5(dp32x1_b, Dest, Src0, Src1, Src2, Info, U512)
#define VXC_DP16x2_b(Dest, Src0, Src1, Src2, Info, U512)    VXC_OP5(dp16x2_b, Dest, Src0, Src1, Src2, Info, U512)
#define VXC_DP8x4_b(Dest, Src0, Src1, Src2, Info, U512)     VXC_OP5(dp8x4_b,  Dest, Src0, Src1, Src2, Info, U512)
#define VXC_DP4x8_b(Dest, Src0, Src1, Src2, Info, U512)     VXC_OP5(dp4x8_b,  Dest, Src0, Src1, Src2, Info, U512)
#define VXC_DP2x16_b(Dest, Src0, Src1, Src2, Info, U512)    VXC_OP5(dp2x16_b, Dest, Src0, Src1, Src2, Info, U512)

#define VXC_Gather(Dest, BaseAddr, Offsets, GatherInfo)                         VXC_OP3(gather, Dest, BaseAddr, Offsets, GatherInfo)
#define VXC_Gather_b(Dest, BaseAddr, Offsets, Offsets_b, GatherInfo)            VXC_OP4(gather_b, Dest, BaseAddr, Offsets, Offsets_b, GatherInfo)

#define VXC_Scatter(BaseAddr, Offsets, Data, ScatterInfo)                       VXC_OP4_NoDest(scatter, BaseAddr, Offsets, Data, ScatterInfo)
#define VXC_Scatter_b(BaseAddr, Offsets, Offsets_b, Data, ScatterInfo)          VXC_OP5_NoDest(scatter_b, BaseAddr, Offsets, Offsets_b, Data, ScatterInfo)

#define VXC_AtomicS(Dest, BaseAddr, Offsets, Data, AtomicSInfo)                 VXC_OP4(atomic_s, Dest, BaseAddr, Offsets, Data, AtomicSInfo)
#define VXC_AtomicS_b(Dest, BaseAddr, Offsets, Offsets_b, Data, AtomicSInfo)    VXC_OP5(atomic_s_b, Dest, BaseAddr, Offsets, Offsets_b, Data, AtomicSInfo)



#define VXC_ReadImage(Dest, Image, Coord, Offset, Info)   VXC_OP4(img_load, Dest, Image, Coord, Offset, Info)
#define VXC_WriteImage(Image, Coord, Color, Info)         VXC_OP4_NoDest(img_store, Image, Coord, Color, Info)


#define VXC_ReadImage2DArray(Dest, Image, Coord, Offset, Info)         \
    do {                                                               \
       int8 desc;                                                      \
       _viv_asm(COPY, desc, Image, sizeof(desc));                      \
       _viv_asm(CLAMP0MAX, (Coord).w, (Coord).z, desc.s5 - 1);         \
       int baseAddr =  (int)(Coord).w *desc.s4 + desc.s0;              \
       _viv_asm(MOV, (Coord).w, baseAddr);                             \
       VXC_OP4(img_load_3d, Dest, Image, (Coord).xyww, Offset, Info);  \
    } while (0)
#define VXC_WriteImage2DArray(Image, Coord, Color, Info)               \
    do {                                                               \
       int8 desc;                                                      \
       _viv_asm(COPY, desc, Image, sizeof(desc));                      \
       _viv_asm(CLAMP0MAX, (Coord).w, (Coord).z, desc.s5 - 1);         \
       int baseAddr =  (int)(Coord).w *(desc).s4 + desc.s0;            \
       _viv_asm(MOV, (Coord).w, baseAddr);                             \
       VXC_OP4_NoDest(img_store_3d, Image, (Coord).xyww, Color, Info); \
    } while (0)


#define VXC_ReadImage3D(Dest, Image, Coord, Offset, Info)       VXC_OP4(img_load_3d, Dest, Image, Coord, Offset, Info)
#define VXC_WriteImage3D(Image, Coord, Color, Info)             VXC_OP4_NoDest(img_store_3d, Image, Coord, Color, Info)

#define VXC_Vload2(Dest, Pointer, Offset)    do { int byteOffset = ((int)sizeof((Dest)))*(Offset); VXC_OP2(vload2, Dest, Pointer, byteOffset); } while(0)
#define VXC_Vload4(Dest, Pointer, Offset)    do { int byteOffset = ((int)sizeof((Dest)))*(Offset); VXC_OP2(vload4, Dest, Pointer,  byteOffset); } while(0)
#define VXC_Vload8(Dest, Pointer, Offset)    do { int byteOffset = ((int)sizeof((Dest)))*(Offset); VXC_OP2(vload8, Dest, Pointer,  byteOffset); } while(0)
#define VXC_Vload16(Dest, Pointer, Offset)   do { int byteOffset = ((int)sizeof((Dest)))*(Offset); VXC_OP2(vload16, Dest, Pointer,  byteOffset); } while(0)

#define VXC_Vstore2(Pointer, Offset, Data)   do { int byteOffset = ((int)sizeof((Data)))*(Offset); VXC_OP3_NoDest(vstore2, Pointer, byteOffset, Data); } while(0)
#define VXC_Vstore4(Pointer, Offset, Data)   do { int byteOffset = ((int)sizeof((Data)))*(Offset); VXC_OP3_NoDest(vstore4, Pointer, byteOffset, Data); } while(0)
#define VXC_Vstore8(Pointer, Offset, Data)   do { int byteOffset = ((int)sizeof((Data)))*(Offset); VXC_OP3_NoDest(vstore8, Pointer, byteOffset, Data); } while(0)
#define VXC_Vstore16(Pointer, Offset, Data)  do { int byteOffset = ((int)sizeof((Data)))*(Offset); VXC_OP3_NoDest(vstore16, Pointer, byteOffset, Data); } while(0)


#define VXC_IndexAdd(Dest, Src0, Src1, Src2, Info)        VXC_OP4(index_add, Dest, Src0, Src1, Src2, Info)
#define VXC_VertMin3(Dest, Src0, Src1, Src2, Info)        VXC_OP4(vert_min3, Dest, Src0, Src1, Src2, Info)
#define VXC_VertMax3(Dest, Src0, Src1, Src2, Info)        VXC_OP4(vert_max3, Dest, Src0, Src1, Src2, Info)
#define VXC_VertMed3(Dest, Src0, Src1, Src2, Info)        VXC_OP4(vert_med3, Dest, Src0, Src1, Src2, Info)
#define VXC_HorzMin3(Dest, Src0, Info)                    VXC_OP2(horz_min3, Dest, Src0, Info)
#define VXC_HorzMax3(Dest, Src0, Info)                    VXC_OP2(horz_max3, Dest, Src0, Info)
#define VXC_HorzMed3(Dest, Src0, Info)                    VXC_OP2(horz_med3, Dest, Src0, Info)

#if (VX_VERSION == 2)
#define VXC_BiLinear(Dest, Src0, Src1, Src2, Info)                                      \
    do {                                                                                \
        int endBin = ((Info) & VXC_END_BIN_BITMASK) >> 8;                               \
        int roundMode = ((Info) & VXC_ROUNDING_MODE_BITMASK) >> 2;                      \
        int clamp = ((Info) & VXC_CLAMP_BITMASK) >> 22;                                 \
        int mod1 = VXC_MODIFIER(0, endBin + 1, 0, roundMode, clamp);                    \
        int4 bitMask = { 0x00000000, 0x00000008, 0x00000010, 0x00000018};               \
        typeof (Dest) bi1;                                                              \
        uint4 bi2;                                                                      \
        int bi3, bi4;                                                                   \
        VXC_Lerp(bi1, Src0, Src1, (Src2).y, mod1);                                      \
        _viv_asm(PARAM_CHAIN, bi3, bi1.x!<f:UINT>, bitMask);                            \
        _viv_asm(PARAM_CHAIN, bi4, bi3, 8);                                             \
        _viv_asm(INTRINSIC, bi2, OP_bit_extract, bi4);                                  \
        VXC_Lerp(Dest, bi2!<f:UCHAR>, bi2.y!<f:UCHAR>, (Src2).x, Info);                 \
    }   while (0)

#define VXC_BitReplace(Dest, Src0, Src1, Src2, Info)   
#define VXC_IAdd(Dest, Src0, Src1, Src2, Info)         
#define VXC_MagPhase(Dest, Src0, Src1, Info)           
#define VXC_SelectAdd(Dest, Src0, Src1, U512, Info)    VXC_OP1(error, ERROR_SELECTADD_NOT_SUPPORTED)

#define VXC_Filter_Box(Dest, Src0, Src1, Src2, Info)        
#define VXC_Filter_Guassian(Dest, Src0, Src1, Src2, Info)   
#define VXC_Filter_SobelX(Dest, Src0, Src1, Src2, Info)     
#define VXC_Filter_SobelY(Dest, Src0, Src1, Src2, Info)     
#define VXC_Filter_ScharrX(Dest, Src0, Src1, Src2, Info)    
#define VXC_Filter_ScharrY(Dest, Src0, Src1, Src2, Info)    
#define VXC_Filter_Max(Dest, Src0, Src1, Src2, Info)        
#define VXC_Filter_Min(Dest, Src0, Src1, Src2, Info)        
#define VXC_Filter_Median(Dest, Src0, Src1, Src2, Info)     
#define VXC_Filter(Dest, Src0, Src1, Src2, Info)       do {                                    \
        int filter = (((Info) >> 16)&0x0F);                                                    \
        if (filter == VXC_FM_BOX)       { VXC_Filter_Box(Dest, Src0, Src1, Src2, Info); }      \
        if (filter == VXC_FM_Guassian)  { VXC_Filter_Guassian(Dest, Src0, Src1, Src2, Info); } \
        if (filter == VXC_FM_SobelX)    { VXC_Filter_SobelX(Dest, Src0, Src1, Src2, Info); }   \
        if (filter == VXC_FM_SobelY)    { VXC_Filter_SobelY(Dest, Src0, Src1, Src2, Info); }   \
        if (filter == VXC_FM_ScharrX)   { VXC_Filter_ScharrX(Dest, Src0, Src1, Src2, Info); }  \
        if (filter == VXC_FM_ScharrY)   { VXC_Filter_ScharrY(Dest, Src0, Src1, Src2, Info); }  \
        if (filter == VXC_FM_Max)       { VXC_Filter_Max(Dest, Src0, Src1, Src2, Info); }      \
        if (filter == VXC_FM_Min)       { VXC_Filter_Min(Dest, Src0, Src1, Src2, Info); }      \
        if (filter == VXC_FM_Median)    { VXC_Filter_Median(Dest, Src0, Src1, Src2, Info); }   \
    } while (0)

#else   

#define VXC_BiLinear(Dest, Src0, Src1, Src2, Info)     VXC_OP4(bi_linear, Dest, Src0, Src1, Src2, Info)
#define VXC_BitReplace(Dest, Src0, Src1, Src2, Info)   VXC_OP4(bit_replace, Dest, Src0, Src1, Src2, Info)
#define VXC_IAdd(Dest, Src0, Src1, Src2, Info)         VXC_OP4(iadd, Dest, Src0, Src1, Src2, Info)
#define VXC_MagPhase(Dest, Src0, Src1, Info)           VXC_OP3(mag_phase, Dest, Src0, Src1, Info)
#define VXC_SelectAdd(Dest, Src0, Src1, U512, Info)    VXC_OP4(select_add, Dest, Src0, Src1, U512, Info)
#define VXC_Filter(Dest, Src0, Src1, Src2, Info)       VXC_OP4(filter, Dest, Src0, Src1, Src2, Info)
#endif

#else

#ifdef __cplusplus
extern "c" {
#endif

#define viv_vx_api_only 0

#if viv_vx_api_only
#define _RET0_        ;
#define _RET_         ;
#define _EXT_         extern
#else
#define _RET0_        { return (0); }
#define _RET_         { return ; }
#define _EXT_
#endif



_EXT_ vxc_uchar16 viv_intrinsic_vx_icastP_uc(vxc_char16 a) _RET0_
_EXT_ vxc_uchar8  viv_intrinsic_vx_icastP_uc(vxc_char8 a)  _RET0_

_EXT_ vxc_uchar16  viv_intrinsic_vx_icastP_uc(vxc_short8 a)  _RET0_
_EXT_ vxc_uchar8  viv_intrinsic_vx_icastP_uc(vxc_short4 a)  _RET0_

_EXT_ vxc_uchar16  viv_intrinsic_vx_icastP_uc(vxc_ushort8 a)  _RET0_
_EXT_ vxc_uchar8  viv_intrinsic_vx_icastP_uc(vxc_ushort4 a)  _RET0_

_EXT_ vxc_uchar16  viv_intrinsic_vx_icastP_uc(vxc_half8 a)  _RET0_
_EXT_ vxc_uchar8  viv_intrinsic_vx_icastP_uc(vxc_half4 a)  _RET0_


_EXT_ vxc_char16 viv_intrinsic_vx_icastP_c(vxc_uchar16 a) _RET0_
_EXT_ vxc_char8  viv_intrinsic_vx_icastP_c(vxc_uchar8 a)  _RET0_

_EXT_ vxc_char16  viv_intrinsic_vx_icastP_c(vxc_short8 a)  _RET0_
_EXT_ vxc_char8  viv_intrinsic_vx_icastP_c(vxc_short4 a)  _RET0_

_EXT_ vxc_char16  viv_intrinsic_vx_icastP_c(vxc_ushort8 a)  _RET0_
_EXT_ vxc_char8  viv_intrinsic_vx_icastP_c(vxc_ushort4 a)  _RET0_

_EXT_ vxc_char16  viv_intrinsic_vx_icastP_c(vxc_half8 a)  _RET0_
_EXT_ vxc_char8  viv_intrinsic_vx_icastP_c(vxc_half4 a)  _RET0_


_EXT_ vxc_ushort8  viv_intrinsic_vx_icastP_us(vxc_uchar16 a) _RET0_
_EXT_ vxc_ushort4  viv_intrinsic_vx_icastP_us(vxc_uchar8 a)  _RET0_

_EXT_ vxc_ushort8  viv_intrinsic_vx_icastP_us(vxc_char16 a) _RET0_
_EXT_ vxc_ushort4  viv_intrinsic_vx_icastP_us(vxc_char8 a)  _RET0_

_EXT_ vxc_ushort8  viv_intrinsic_vx_icastP_us(vxc_short8 a)  _RET0_
_EXT_ vxc_ushort4  viv_intrinsic_vx_icastP_us(vxc_short4 a)  _RET0_

_EXT_ vxc_ushort8  viv_intrinsic_vx_icastP_us(vxc_half8 a)  _RET0_
_EXT_ vxc_ushort4  viv_intrinsic_vx_icastP_us(vxc_half4 a)  _RET0_


_EXT_ vxc_short8  viv_intrinsic_vx_icastP_s(vxc_uchar16 a) _RET0_
_EXT_ vxc_short4  viv_intrinsic_vx_icastP_s(vxc_uchar8 a)  _RET0_

_EXT_ vxc_short8  viv_intrinsic_vx_icastP_s(vxc_char16 a) _RET0_
_EXT_ vxc_short4  viv_intrinsic_vx_icastP_s(vxc_char8 a)  _RET0_

_EXT_ vxc_short8  viv_intrinsic_vx_icastP_s(vxc_ushort8 a)  _RET0_
_EXT_ vxc_short4  viv_intrinsic_vx_icastP_s(vxc_ushort4 a)  _RET0_

_EXT_ vxc_short8  viv_intrinsic_vx_icastP_s(vxc_half8 a)  _RET0_
_EXT_ vxc_short4  viv_intrinsic_vx_icastP_s(vxc_half4 a)  _RET0_


_EXT_ vxc_half8  viv_intrinsic_vx_icastP_h(vxc_uchar16 a) _RET0_
_EXT_ vxc_half4  viv_intrinsic_vx_icastP_h(vxc_uchar8 a)  _RET0_

_EXT_ vxc_half8  viv_intrinsic_vx_icastP_h(vxc_char16 a) _RET0_
_EXT_ vxc_half4  viv_intrinsic_vx_icastP_h(vxc_char8 a)  _RET0_

_EXT_ vxc_half8  viv_intrinsic_vx_icastP_h(vxc_ushort8 a)  _RET0_
_EXT_ vxc_half4  viv_intrinsic_vx_icastP_h(vxc_ushort4 a)  _RET0_

_EXT_ vxc_half8  viv_intrinsic_vx_icastP_h(vxc_short8 a)  _RET0_
_EXT_ vxc_half4  viv_intrinsic_vx_icastP_h(vxc_short4 a)  _RET0_




_EXT_ vxc_uchar16 viv_intrinsic_vx_icastD_uc(vxc_char16 a) _RET0_
_EXT_ vxc_uchar8  viv_intrinsic_vx_icastD_uc(vxc_char8 a)  _RET0_

_EXT_ vxc_uchar8 viv_intrinsic_vx_icastD_uc(vxc_short8 a)  _RET0_
_EXT_ vxc_uchar4  viv_intrinsic_vx_icastD_uc(vxc_short4 a)  _RET0_

_EXT_ vxc_uchar8 viv_intrinsic_vx_icastD_uc(vxc_ushort8 a)  _RET0_
_EXT_ vxc_uchar4  viv_intrinsic_vx_icastD_uc(vxc_ushort4 a)  _RET0_

_EXT_ vxc_uchar8 viv_intrinsic_vx_icastD_uc(vxc_half8 a)  _RET0_
_EXT_ vxc_uchar4  viv_intrinsic_vx_icastD_uc(vxc_half4 a)  _RET0_

_EXT_ vxc_uchar4 viv_intrinsic_vx_icastD_uc(vxc_int4 a)  _RET0_
_EXT_ vxc_uchar2  viv_intrinsic_vx_icastD_uc(vxc_int2 a)  _RET0_

_EXT_ vxc_uchar4 viv_intrinsic_vx_icastD_uc(vxc_uint4 a)  _RET0_
_EXT_ vxc_uchar2  viv_intrinsic_vx_icastD_uc(vxc_uint2 a)  _RET0_


_EXT_ vxc_char16 viv_intrinsic_vx_icastD_c(vxc_uchar16 a) _RET0_
_EXT_ vxc_char8  viv_intrinsic_vx_icastD_c(vxc_uchar8 a)  _RET0_

_EXT_ vxc_char8 viv_intrinsic_vx_icastD_c(vxc_short8 a)  _RET0_
_EXT_ vxc_char4  viv_intrinsic_vx_icastD_c(vxc_short4 a)  _RET0_

_EXT_ vxc_char8 viv_intrinsic_vx_icastD_c(vxc_ushort8 a)  _RET0_
_EXT_ vxc_char4  viv_intrinsic_vx_icastD_c(vxc_ushort4 a)  _RET0_

_EXT_ vxc_char8 viv_intrinsic_vx_icastD_c(vxc_half8 a)  _RET0_
_EXT_ vxc_char4  viv_intrinsic_vx_icastD_c(vxc_half4 a)  _RET0_

_EXT_ vxc_char4 viv_intrinsic_vx_icastD_c(vxc_int4 a)  _RET0_
_EXT_ vxc_char2  viv_intrinsic_vx_icastD_c(vxc_int2 a)  _RET0_

_EXT_ vxc_char4 viv_intrinsic_vx_icastD_c(vxc_uint4 a)  _RET0_
_EXT_ vxc_char2  viv_intrinsic_vx_icastD_c(vxc_uint2 a)  _RET0_


_EXT_ vxc_ushort8  viv_intrinsic_vx_icastD_us(vxc_uchar8 a) _RET0_
_EXT_ vxc_ushort4  viv_intrinsic_vx_icastD_us(vxc_uchar4 a)  _RET0_

_EXT_ vxc_ushort8  viv_intrinsic_vx_icastD_us(vxc_char8 a) _RET0_
_EXT_ vxc_ushort4  viv_intrinsic_vx_icastD_us(vxc_char4 a)  _RET0_

_EXT_ vxc_ushort8  viv_intrinsic_vx_icastD_us(vxc_short8 a)  _RET0_
_EXT_ vxc_ushort4  viv_intrinsic_vx_icastD_us(vxc_short4 a)  _RET0_

_EXT_ vxc_ushort8  viv_intrinsic_vx_icastD_us(vxc_half8 a)  _RET0_
_EXT_ vxc_ushort4  viv_intrinsic_vx_icastD_us(vxc_half4 a)  _RET0_

_EXT_ vxc_ushort4  viv_intrinsic_vx_icastD_us(vxc_int4 a)  _RET0_
_EXT_ vxc_ushort2  viv_intrinsic_vx_icastD_us(vxc_int2 a)  _RET0_

_EXT_ vxc_ushort4  viv_intrinsic_vx_icastD_us(vxc_uint4 a)  _RET0_
_EXT_ vxc_ushort2  viv_intrinsic_vx_icastD_us(vxc_uint2 a)  _RET0_


_EXT_ vxc_short8  viv_intrinsic_vx_icastD_s(vxc_uchar8 a) _RET0_
_EXT_ vxc_short4  viv_intrinsic_vx_icastD_s(vxc_uchar4 a)  _RET0_

_EXT_ vxc_short8  viv_intrinsic_vx_icastD_s(vxc_char8 a) _RET0_
_EXT_ vxc_short4  viv_intrinsic_vx_icastD_s(vxc_char4 a)  _RET0_

_EXT_ vxc_short8  viv_intrinsic_vx_icastD_s(vxc_ushort8 a)  _RET0_
_EXT_ vxc_short4  viv_intrinsic_vx_icastD_s(vxc_ushort4 a)  _RET0_

_EXT_ vxc_short8  viv_intrinsic_vx_icastD_s(vxc_half8 a)  _RET0_
_EXT_ vxc_short4  viv_intrinsic_vx_icastD_s(vxc_half4 a)  _RET0_

_EXT_ vxc_short4  viv_intrinsic_vx_icastD_s(vxc_int4 a)  _RET0_
_EXT_ vxc_short2  viv_intrinsic_vx_icastD_s(vxc_int2 a)  _RET0_

_EXT_ vxc_short4  viv_intrinsic_vx_icastD_s(vxc_uint4 a)  _RET0_
_EXT_ vxc_short2  viv_intrinsic_vx_icastD_s(vxc_uint2 a)  _RET0_


_EXT_ vxc_half8  viv_intrinsic_vx_icastD_h(vxc_uchar8 a) _RET0_
_EXT_ vxc_half4  viv_intrinsic_vx_icastD_h(vxc_uchar4 a)  _RET0_

_EXT_ vxc_half8  viv_intrinsic_vx_icastD_h(vxc_char8 a) _RET0_
_EXT_ vxc_half4  viv_intrinsic_vx_icastD_h(vxc_char4 a)  _RET0_

_EXT_ vxc_half8  viv_intrinsic_vx_icastD_h(vxc_ushort8 a)  _RET0_
_EXT_ vxc_half4  viv_intrinsic_vx_icastD_h(vxc_ushort4 a)  _RET0_

_EXT_ vxc_half8  viv_intrinsic_vx_icastD_h(vxc_short8 a)  _RET0_
_EXT_ vxc_half4  viv_intrinsic_vx_icastD_h(vxc_short4 a)  _RET0_

_EXT_ vxc_half4  viv_intrinsic_vx_icastD_h(vxc_int4 a)  _RET0_
_EXT_ vxc_half2  viv_intrinsic_vx_icastD_h(vxc_int2 a)  _RET0_

_EXT_ vxc_half4  viv_intrinsic_vx_icastD_h(vxc_uint4 a)  _RET0_
_EXT_ vxc_half2  viv_intrinsic_vx_icastD_h(vxc_uint2 a)  _RET0_


_EXT_ vxc_int4  viv_intrinsic_vx_icastD_i(vxc_char4 a)  _RET0_
_EXT_ vxc_int4  viv_intrinsic_vx_icastD_i(vxc_uchar4 a)  _RET0_
_EXT_ vxc_int4  viv_intrinsic_vx_icastD_i(vxc_short4 a)  _RET0_
_EXT_ vxc_int4  viv_intrinsic_vx_icastD_i(vxc_ushort4 a)  _RET0_
_EXT_ vxc_int4  viv_intrinsic_vx_icastD_i(vxc_half4 a)  _RET0_
_EXT_ vxc_int4  viv_intrinsic_vx_icastD_i(vxc_uint4 a)  _RET0_
_EXT_ vxc_int4  viv_intrinsic_vx_icastD_i(vxc_float4 a)  _RET0_

_EXT_ vxc_int2  viv_intrinsic_vx_icastD_i(vxc_char2 a)  _RET0_
_EXT_ vxc_int2  viv_intrinsic_vx_icastD_i(vxc_uchar2 a)  _RET0_
_EXT_ vxc_int2  viv_intrinsic_vx_icastD_i(vxc_short2 a)  _RET0_
_EXT_ vxc_int2  viv_intrinsic_vx_icastD_i(vxc_ushort2 a)  _RET0_
_EXT_ vxc_int2  viv_intrinsic_vx_icastD_i(vxc_half2 a)  _RET0_
_EXT_ vxc_int2  viv_intrinsic_vx_icastD_i(vxc_uint2 a)  _RET0_
_EXT_ vxc_int2  viv_intrinsic_vx_icastD_i(vxc_float2 a)  _RET0_


_EXT_ vxc_uint4  viv_intrinsic_vx_icastD_ui(vxc_char4 a)  _RET0_
_EXT_ vxc_uint4  viv_intrinsic_vx_icastD_ui(vxc_uchar4 a)  _RET0_
_EXT_ vxc_uint4  viv_intrinsic_vx_icastD_ui(vxc_short4 a)  _RET0_
_EXT_ vxc_uint4  viv_intrinsic_vx_icastD_ui(vxc_ushort4 a)  _RET0_
_EXT_ vxc_uint4  viv_intrinsic_vx_icastD_ui(vxc_half4 a)  _RET0_
_EXT_ vxc_uint4  viv_intrinsic_vx_icastD_ui(vxc_int4 a)  _RET0_
_EXT_ vxc_uint4  viv_intrinsic_vx_icastD_ui(vxc_float4 a)  _RET0_

_EXT_ vxc_uint2  viv_intrinsic_vx_icastD_ui(vxc_char2 a)  _RET0_
_EXT_ vxc_uint2  viv_intrinsic_vx_icastD_ui(vxc_uchar2 a)  _RET0_
_EXT_ vxc_uint2  viv_intrinsic_vx_icastD_ui(vxc_short2 a)  _RET0_
_EXT_ vxc_uint2  viv_intrinsic_vx_icastD_ui(vxc_ushort2 a)  _RET0_
_EXT_ vxc_uint2  viv_intrinsic_vx_icastD_ui(vxc_half2 a)  _RET0_
_EXT_ vxc_uint2  viv_intrinsic_vx_icastD_ui(vxc_int2 a)  _RET0_
_EXT_ vxc_uint2  viv_intrinsic_vx_icastD_ui(vxc_float2 a)  _RET0_


_EXT_ vxc_float4  viv_intrinsic_vx_icastD_f(vxc_char4 a)  _RET0_
_EXT_ vxc_float4  viv_intrinsic_vx_icastD_f(vxc_uchar4 a)  _RET0_
_EXT_ vxc_float4  viv_intrinsic_vx_icastD_f(vxc_short4 a)  _RET0_
_EXT_ vxc_float4  viv_intrinsic_vx_icastD_f(vxc_ushort4 a)  _RET0_
_EXT_ vxc_float4  viv_intrinsic_vx_icastD_f(vxc_half4 a)  _RET0_
_EXT_ vxc_float4  viv_intrinsic_vx_icastD_f(vxc_int4 a)  _RET0_

_EXT_ vxc_float2  viv_intrinsic_vx_icastD_f(vxc_char2 a)  _RET0_
_EXT_ vxc_float2  viv_intrinsic_vx_icastD_f(vxc_uchar2 a)  _RET0_
_EXT_ vxc_float2  viv_intrinsic_vx_icastD_f(vxc_short2 a)  _RET0_
_EXT_ vxc_float2  viv_intrinsic_vx_icastD_f(vxc_ushort2 a)  _RET0_
_EXT_ vxc_float2  viv_intrinsic_vx_icastD_f(vxc_half2 a)  _RET0_
_EXT_ vxc_float2  viv_intrinsic_vx_icastD_f(vxc_int2 a)  _RET0_



#define VXC_SWIZZLE_MASK8_ALL()   0xFFFF
#define VXC_SWIZZLE_MASK8(E0, E1, E2, E3, E4, E5, E6, E7)                       \
        (((E0) * 0x3)         | ((E1) * (0x3 << 2))  | ((E2) * (0x3 << 4))  |  \
         ((E3) * (0x3 << 6))  | ((E4) * (0x3 << 8))  | ((E5) * (0x3 << 10)) |  \
         ((E6) * (0x3 << 12)) | ((E7) * (0x3 << 14))  )

#define VXC_SWIZZLE_MASK16_ALL()   0xFFFF
#define VXC_SWIZZLE_MASK16(E0, E1, E2, E3, E4, E5, E6, E7, E9, E10, E11, E12, E13, E14, E15)  \
        (((E0) * 0x1)          | ((E1) * (0x1 << 1))   | ((E2) * (0x1 << 2))    |  \
         ((E3) * (0x1 << 1))   | ((E4) * (0x1 << 4))   | ((E5) * (0x1 << 5))    |  \
         ((E6) * (0x1 << 6))   | ((E7) * (0x1 << 7))   | ((E8) * (0x1 << 8))    |  \
         ((E9) * (0x1 << 9))   | ((E10) * (0x1 << 10)) | ((E11) * (0x1 << 11))  |  \
         ((E12) * (0x1 << 12)) | ((E13) * (0x1 << 13)) | ((E14) * (0x1 << 14))  |  \
         ((E15) * (0x1 << 15))  )

#define VXC_SWIZZLE8(S0, S1, S2, S3, S4, S5, S6, S7)           \
        (uint)((S0) << 0  | (S1) << 4  | (S2) << 8  | (S3) << 12 |  \
               (S4) << 16 | (S5) << 20 | (S6) << 24 | (S7) << 28  )


_EXT_ vxc_char16  viv_intrinsic_vx_read_imagec   (image2d_t image, int2 coord) _RET0_
_EXT_ vxc_uchar16 viv_intrinsic_vx_read_imageuc  (image2d_t image, int2 coord) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vx_read_images   (image2d_t image, int2 coord) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vx_read_imageus  (image2d_t image, int2 coord) _RET0_
_EXT_ vxc_half8   viv_intrinsic_vx_read_imageh   (image2d_t image, int2 coord) _RET0_

_EXT_ vxc_char16  viv_intrinsic_vx_read_imagec   (image1d_t image, int coord) _RET0_
_EXT_ vxc_uchar16 viv_intrinsic_vx_read_imageuc  (image1d_t image, int coord) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vx_read_images   (image1d_t image, int coord) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vx_read_imageus  (image1d_t image, int coord) _RET0_
_EXT_ vxc_half8   viv_intrinsic_vx_read_imageh   (image1d_t image, int coord) _RET0_

_EXT_ vxc_char16  viv_intrinsic_vx_read_imagec   (image1d_array_t image, int2 coord) _RET0_
_EXT_ vxc_uchar16 viv_intrinsic_vx_read_imageuc  (image1d_array_t image, int2 coord) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vx_read_images   (image1d_array_t image, int2 coord) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vx_read_imageus  (image1d_array_t image, int2 coord) _RET0_
_EXT_ vxc_half8   viv_intrinsic_vx_read_imageh   (image1d_array_t image, int2 coord) _RET0_


_EXT_ void viv_intrinsic_vx_write_imagec  (image2d_t image, int2 coord, vxc_char16 color) _RET_
_EXT_ void viv_intrinsic_vx_write_imageuc (image2d_t image, int2 coord, vxc_uchar16 color) _RET_
_EXT_ void viv_intrinsic_vx_write_images  (image2d_t image, int2 coord, vxc_short8 color) _RET_
_EXT_ void viv_intrinsic_vx_write_imageus (image2d_t image, int2 coord, vxc_ushort8 color) _RET_
_EXT_ void viv_intrinsic_vx_write_imageh  (image2d_t image, int2 coord, vxc_half8 color) _RET_

_EXT_ void viv_intrinsic_vx_write_imagec  (image1d_t image, int coord, vxc_char16 color) _RET_
_EXT_ void viv_intrinsic_vx_write_imageuc (image1d_t image, int coord, vxc_uchar16 color) _RET_
_EXT_ void viv_intrinsic_vx_write_images  (image1d_t image, int coord, vxc_short8 color) _RET_
_EXT_ void viv_intrinsic_vx_write_imageus (image1d_t image, int coord, vxc_ushort8 color) _RET_
_EXT_ void viv_intrinsic_vx_write_imageh  (image1d_t image, int coord, vxc_half8 color) _RET_

_EXT_ void viv_intrinsic_vx_write_imagec  (image1d_array_t image, int2 coord, vxc_char16 color) _RET_
_EXT_ void viv_intrinsic_vx_write_imageuc (image1d_array_t image, int2 coord, vxc_uchar16 color) _RET_
_EXT_ void viv_intrinsic_vx_write_images  (image1d_array_t image, int2 coord, vxc_short8 color) _RET_
_EXT_ void viv_intrinsic_vx_write_imageus (image1d_array_t image, int2 coord, vxc_ushort8 color) _RET_
_EXT_ void viv_intrinsic_vx_write_imageh  (image1d_array_t image, int2 coord, vxc_half8 color) _RET_


_EXT_ vxc_uchar16 viv_intrinsic_vx_AbsDiff_uc(vxc_uchar16 a, vxc_uchar16 b) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vx_AbsDiff_c(vxc_char16 a, vxc_char16 b) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vx_AbsDiff_s(vxc_short8 a, vxc_short8 b) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vx_AbsDiff_us(vxc_ushort8 a, vxc_ushort8 b) _RET0_


_EXT_ vxc_uchar16 viv_intrinsic_vx_IAdd_uc(vxc_uchar16 a, vxc_uchar16 b, vxc_uchar16 c) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vx_IAdd_c(vxc_char16 a, vxc_char16 b, vxc_char16 c) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vx_IAdd_s(vxc_short8 a, vxc_short8 b, vxc_short8 c) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vx_IAdd_us(vxc_ushort8 a, vxc_ushort8 b, vxc_ushort8 c) _RET0_


_EXT_ vxc_uchar16 viv_intrinsic_vx_IAccSq_uc(vxc_uchar16 a, vxc_uchar16 b, uint Imm) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vx_IAccSq_c(vxc_char16 a, vxc_char16 b, uint Imm) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vx_IAccSq_s(vxc_short8 a, vxc_short8 b, uint Imm) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vx_IAccSq_us(vxc_ushort8 a, vxc_ushort8 b, uint Imm) _RET0_


_EXT_ vxc_uchar16 viv_intrinsic_vx_Lerp_uc(vxc_uchar16 a, vxc_uchar16 b, vxc_float c) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vx_Lerp_c(vxc_char16 a, vxc_char16 b, vxc_float c) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vx_Lerp_s(vxc_short8 a, vxc_short8 b, vxc_float c) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vx_Lerp_us(vxc_ushort8 a, vxc_ushort8 b, vxc_float c) _RET0_


_EXT_ vxc_uchar16 viv_intrinsic_vx_Filter_uc(vxc_uchar16 a, vxc_uchar16 b, vxc_uchar16 c, vxc_filter_mode f) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vx_Filter_c(vxc_char16 a, vxc_char16 b, vxc_char16 c, vxc_filter_mode f) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vx_Filter_s(vxc_short8 a, vxc_short8 b, vxc_short8 c, vxc_filter_mode f) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vx_Filter_us(vxc_ushort8 a, vxc_ushort8 b, vxc_ushort8 c, vxc_filter_mode f) _RET0_


_EXT_ vxc_uchar16 viv_intrinsic_vx_MagPhase_uc(vxc_uchar16 a, vxc_uchar16 b) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vx_MagPhase_c(vxc_char16 a, vxc_char16 b) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vx_MagPhase_s(vxc_short8 a, vxc_short8 b) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vx_MagPhase_us(vxc_ushort8 a, vxc_ushort8 b) _RET0_


_EXT_ vxc_uchar16 viv_intrinsic_vx_MulShift_uc(vxc_uchar16 a, vxc_uchar16 b, uint Imm) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vx_MulShift_c(vxc_char16 a, vxc_char16 b, uint Imm) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vx_MulShift_s(vxc_short8 a, vxc_short8 b, uint Imm) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vx_MulShift_us(vxc_ushort8 a, vxc_ushort8 b, uint Imm) _RET0_

/* Clamp: clamps up to 16 values to a min and.or max value
 *
 * Syntax:
 *      r = Clamp(a, b, c) ;
 *      r = ClampBoolean(a, b, c) ; // boolean mode
 * Semantics:
 *      r[i] = clamp(a[i], b[i], c[i]) ;  i E [0, elem(r) )
 *
 *  In boolean mode it will write a 0 in the result if the value
 *  is inside the specified min/max range, otherwise all 1’s will
 *  be written to the result.
 */
_EXT_ vxc_uchar16 viv_intrinsic_vx_Clamp_uc(vxc_uchar16 a, vxc_uchar16 b, vxc_uchar16 c) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vx_Clamp_c(vxc_char16 a, vxc_char16 b, vxc_char16 c) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vx_Clamp_s(vxc_short8 a, vxc_short8 b, vxc_short8 c) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vx_Clamp_us(vxc_ushort8 a, vxc_ushort8 b, vxc_ushort8 c) _RET0_

_EXT_ vxc_uchar16 viv_intrinsic_vx_ClampBoolean_uc(vxc_uchar16 a, vxc_uchar16 b, vxc_uchar16 c) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vx_ClampBoolean_c(vxc_char16 a, vxc_char16 b, vxc_char16 c) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vx_ClampBoolean_s(vxc_short8 a, vxc_short8 b, vxc_short8 c) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vx_ClampBoolean_us(vxc_ushort8 a, vxc_ushort8 b, vxc_ushort8 c) _RET0_


_EXT_ vxc_uchar16 viv_intrinsic_vx_BiLinear_uc(vxc_uchar16 a, vxc_uchar16 b, float2 c) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vx_BiLinear_c(vxc_char16 a, vxc_char16 b, float2 c) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vx_BiLinear_s(vxc_short8 a, vxc_short8 b, float2 c) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vx_BiLinear_us(vxc_ushort8 a, vxc_ushort8 b, float2 c) _RET0_



_EXT_ vxc_uchar16 viv_intrinsic_vx_SelectAdd_uc(vxc_uchar16 a, vxc_uchar16 b, vxc_512bits c) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vx_SelectAdd_c(vxc_char16 a, vxc_char16 b, vxc_512bits c) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vx_SelectAdd_s(vxc_short8 a, vxc_short8 b, vxc_512bits c) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vx_SelectAdd_us(vxc_ushort8 a, vxc_ushort8 b, vxc_512bits c) _RET0_


_EXT_ vxc_uchar16 viv_intrinsic_vx_AtomicAdd_uc(vxc_uchar16 * a, vxc_int offset, vxc_uchar16 c) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vx_AtomicAdd_c(vxc_char16 * a,  vxc_int offset, vxc_char16 c) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vx_AtomicAdd_s(vxc_short8 * a,  vxc_int offset, vxc_short8 c) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vx_AtomicAdd_us(vxc_ushort8 * a, vxc_int offset, vxc_ushort8 c) _RET0_


_EXT_ vxc_ushort8 viv_intrinsic_vx_BitExtract_us(vxc_ushort8 a, vxc_ushort8 b, vxc_uchar16 c) _RET0_
_EXT_ vxc_uchar8  viv_intrinsic_vx_BitExtract_uc(vxc_uchar16 a, vxc_uchar16 b, vxc_uchar16 c) _RET0_


_EXT_ vxc_ushort8 viv_intrinsic_vx_BitReplace_us(vxc_ushort8 a, vxc_ushort8 b, vxc_uchar16 c) _RET0_
_EXT_ vxc_uchar16 viv_intrinsic_vx_BitReplace_uc(vxc_uchar16 a, vxc_uchar16 b, vxc_uchar16 c) _RET0_






_EXT_ vxc_char16  viv_intrinsic_vxmc_read_imagec   (image2d_t image, int2 coord, int offsetXY, vxc_modifier modifier) _RET0_
_EXT_ vxc_uchar16 viv_intrinsic_vxmc_read_imageuc  (image2d_t image, int2 coord, int offsetXY, vxc_modifier modifier) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vxmc_read_images   (image2d_t image, int2 coord, int offsetXY, vxc_modifier modifier) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vxmc_read_imageus  (image2d_t image, int2 coord, int offsetXY, vxc_modifier modifier) _RET0_
_EXT_ vxc_half8   viv_intrinsic_vxmc_read_imageh   (image2d_t image, int2 coord, int offsetXY, vxc_modifier modifier) _RET0_

_EXT_ vxc_char16  viv_intrinsic_vxmc_read_imagec   (image1d_t image, int coord, int offsetX, vxc_modifier modifier) _RET0_
_EXT_ vxc_uchar16 viv_intrinsic_vxmc_read_imageuc  (image1d_t image, int coord, int offsetX, vxc_modifier modifier) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vxmc_read_images   (image1d_t image, int coord, int offsetX, vxc_modifier modifier) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vxmc_read_imageus  (image1d_t image, int coord, int offsetX, vxc_modifier modifier) _RET0_
_EXT_ vxc_half8   viv_intrinsic_vxmc_read_imageh   (image1d_t image, int coord, int offsetX, vxc_modifier modifier) _RET0_

_EXT_ vxc_char16  viv_intrinsic_vxmc_read_imagec   (image1d_array_t image, int2 coord, int offsetXY, vxc_modifier modifier) _RET0_
_EXT_ vxc_uchar16 viv_intrinsic_vxmc_read_imageuc  (image1d_array_t image, int2 coord, int offsetXY, vxc_modifier modifier) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vxmc_read_images   (image1d_array_t image, int2 coord, int offsetXY, vxc_modifier modifier) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vxmc_read_imageus  (image1d_array_t image, int2 coord, int offsetXY, vxc_modifier modifier) _RET0_
_EXT_ vxc_half8   viv_intrinsic_vxmc_read_imageh   (image1d_array_t image, int2 coord, int offsetXY, vxc_modifier modifier) _RET0_


_EXT_ void viv_intrinsic_vxmc_write_imagec  (image2d_t image, int2 coord, vxc_char16 color, vxc_modifier modifier) _RET_
_EXT_ void viv_intrinsic_vxmc_write_imageuc (image2d_t image, int2 coord, vxc_uchar16 color, vxc_modifier modifier) _RET_
_EXT_ void viv_intrinsic_vxmc_write_images  (image2d_t image, int2 coord, vxc_short8 color, vxc_modifier modifier) _RET_
_EXT_ void viv_intrinsic_vxmc_write_imageus (image2d_t image, int2 coord, vxc_ushort8 color, vxc_modifier modifier) _RET_
_EXT_ void viv_intrinsic_vxmc_write_imageh  (image2d_t image, int2 coord, vxc_half8 color, vxc_modifier modifier) _RET_

_EXT_ void viv_intrinsic_vxmc_write_imagec  (image1d_t image, int coord, vxc_char16 color, vxc_modifier modifier) _RET_
_EXT_ void viv_intrinsic_vxmc_write_imageuc (image1d_t image, int coord, vxc_uchar16 color, vxc_modifier modifier) _RET_
_EXT_ void viv_intrinsic_vxmc_write_images  (image1d_t image, int coord, vxc_short8 color, vxc_modifier modifier) _RET_
_EXT_ void viv_intrinsic_vxmc_write_imageus (image1d_t image, int coord, vxc_ushort8 color, vxc_modifier modifier) _RET_
_EXT_ void viv_intrinsic_vxmc_write_imageh  (image1d_t image, int coord, vxc_half8 color, vxc_modifier modifier) _RET_

_EXT_ void viv_intrinsic_vxmc_write_imagec  (image1d_array_t image, int2 coord, vxc_char16 color, vxc_modifier modifier) _RET_
_EXT_ void viv_intrinsic_vxmc_write_imageuc (image1d_array_t image, int2 coord, vxc_uchar16 color, vxc_modifier modifier) _RET_
_EXT_ void viv_intrinsic_vxmc_write_images  (image1d_array_t image, int2 coord, vxc_short8 color, vxc_modifier modifier) _RET_
_EXT_ void viv_intrinsic_vxmc_write_imageus (image1d_array_t image, int2 coord, vxc_ushort8 color, vxc_modifier modifier) _RET_
_EXT_ void viv_intrinsic_vxmc_write_imageh  (image1d_array_t image, int2 coord, vxc_half8 color, vxc_modifier modifier) _RET_


_EXT_ vxc_uchar16 viv_intrinsic_vxmc_AbsDiff_uc(vxc_uchar16 a, vxc_uchar16 b, vxc_modifier modifier) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vxmc_AbsDiff_c(vxc_char16 a, vxc_char16 b,   vxc_modifier modifier) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vxmc_AbsDiff_s(vxc_short8 a, vxc_short8 b,   vxc_modifier modifier) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vxmc_AbsDiff_us(vxc_ushort8 a, vxc_ushort8 b, vxc_modifier modifier) _RET0_


_EXT_ vxc_uchar16 viv_intrinsic_vxmc_IAdd_uc(vxc_uchar16 a, vxc_uchar16 b, vxc_uchar16 c, vxc_modifier modifier) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vxmc_IAdd_c(vxc_char16 a, vxc_char16 b, vxc_char16 c, vxc_modifier modifier) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vxmc_IAdd_s(vxc_short8 a, vxc_short8 b, vxc_short8 c, vxc_modifier modifier) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vxmc_IAdd_us(vxc_ushort8 a, vxc_ushort8 b, vxc_ushort8 c, vxc_modifier modifier) _RET0_


_EXT_ vxc_uchar16 viv_intrinsic_vxmc_IAccSq_uc(vxc_uchar16 a, vxc_uchar16 b, uint Imm, vxc_modifier modifier) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vxmc_IAccSq_c(vxc_char16 a, vxc_char16 b, uint Imm, vxc_modifier modifier) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vxmc_IAccSq_s(vxc_short8 a, vxc_short8 b, uint Imm, vxc_modifier modifier) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vxmc_IAccSq_us(vxc_ushort8 a, vxc_ushort8 b, uint Imm, vxc_modifier modifier) _RET0_


_EXT_ vxc_uchar16 viv_intrinsic_vxmc_Lerp_uc(vxc_uchar16 a, vxc_uchar16 b, vxc_float c, vxc_modifier modifier) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vxmc_Lerp_c(vxc_char16 a, vxc_char16 b, vxc_float c, vxc_modifier modifier) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vxmc_Lerp_s(vxc_short8 a, vxc_short8 b, vxc_float c, vxc_modifier modifier) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vxmc_Lerp_us(vxc_ushort8 a, vxc_ushort8 b, vxc_float c, vxc_modifier modifier) _RET0_


_EXT_ vxc_uchar16 viv_intrinsic_vxmc_Filter_uc(vxc_uchar16 a, vxc_uchar16 b, vxc_uchar16 c, vxc_modifier modifier) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vxmc_Filter_c(vxc_char16 a, vxc_char16 b, vxc_char16 c, vxc_modifier modifier) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vxmc_Filter_s(vxc_short8 a, vxc_short8 b, vxc_short8 c, vxc_modifier modifier) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vxmc_Filter_us(vxc_ushort8 a, vxc_ushort8 b, vxc_ushort8 c, vxc_modifier modifier) _RET0_


_EXT_ vxc_uchar16 viv_intrinsic_vxmc_MagPhase_uc(vxc_uchar16 a, vxc_uchar16 b, vxc_modifier modifier) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vxmc_MagPhase_c(vxc_char16 a, vxc_char16 b, vxc_modifier modifier) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vxmc_MagPhase_s(vxc_short8 a, vxc_short8 b, vxc_modifier modifier) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vxmc_MagPhase_us(vxc_ushort8 a, vxc_ushort8 b, vxc_modifier modifier) _RET0_


_EXT_ vxc_uchar16 viv_intrinsic_vxmc_MulShift_uc(vxc_uchar16 a, vxc_uchar16 b, uint Imm, vxc_modifier modifier) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vxmc_MulShift_c(vxc_char16 a, vxc_char16 b, uint Imm, vxc_modifier modifier) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vxmc_MulShift_s(vxc_short8 a, vxc_short8 b, uint Imm, vxc_modifier modifier) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vxmc_MulShift_us(vxc_ushort8 a, vxc_ushort8 b, uint Imm, vxc_modifier modifier) _RET0_


_EXT_ vxc_uint   viv_intrinsic_vxmc_DP16x1(vxc_uchar16 a,  vxc_uchar16 b, vxc_modifier modifier, vxc_512bits u) _RET0_
_EXT_ vxc_int    viv_intrinsic_vxmc_DP16x1(vxc_char16 a,   vxc_char16 b, vxc_modifier modifier, vxc_512bits u) _RET0_
_EXT_ vxc_float  viv_intrinsic_vxmc_DP16x1(vxc_half8 a,    vxc_half8 b, vxc_modifier modifier, vxc_512bits u) _RET0_


_EXT_ vxc_uint2   viv_intrinsic_vxmc_DP8x2(vxc_uchar16 a,  vxc_uchar16 b, vxc_modifier modifier, vxc_512bits u) _RET0_
_EXT_ vxc_int2    viv_intrinsic_vxmc_DP8x2(vxc_char16 a,   vxc_char16 b, vxc_modifier modifier, vxc_512bits u) _RET0_
_EXT_ vxc_float2  viv_intrinsic_vxmc_DP8x2(vxc_half8 a,    vxc_half8 b, vxc_modifier modifier, vxc_512bits u) _RET0_


_EXT_ vxc_uint4   viv_intrinsic_vxmc_DP4x4(vxc_uchar16 a,  vxc_uchar16 b, vxc_modifier modifier, vxc_512bits u) _RET0_
_EXT_ vxc_int4    viv_intrinsic_vxmc_DP4x4(vxc_char16 a,   vxc_char16 b, vxc_modifier modifier, vxc_512bits u) _RET0_
_EXT_ vxc_float4  viv_intrinsic_vxmc_DP4x4(vxc_half8 a,    vxc_half8 b, vxc_modifier modifier, vxc_512bits u) _RET0_


_EXT_ vxc_uchar8  viv_intrinsic_vxmc_DP2x8(vxc_uchar16 a,  vxc_uchar16 b, vxc_modifier modifier, vxc_512bits u) _RET0_
_EXT_ vxc_char8   viv_intrinsic_vxmc_DP2x8(vxc_char16 a,   vxc_char16 b, vxc_modifier modifier, vxc_512bits u) _RET0_
_EXT_ vxc_half8   viv_intrinsic_vxmc_DP2x8(vxc_half8 a,    vxc_half8 b, vxc_modifier modifier, vxc_512bits u) _RET0_


_EXT_ vxc_uint    viv_intrinsic_vxmc_DP32x1(vxc_uchar16 a,  vxc_uchar16 b, vxc_modifier modifier, vxc_512bits u) _RET0_
_EXT_ vxc_int     viv_intrinsic_vxmc_DP32x1(vxc_char16 a,   vxc_char16 b,  vxc_modifier modifier, vxc_512bits u) _RET0_

_EXT_ vxc_uint2   viv_intrinsic_vxmc_DP16x2(vxc_uchar16 a,  vxc_uchar16 b, vxc_modifier modifier, vxc_512bits u) _RET0_
_EXT_ vxc_int2    viv_intrinsic_vxmc_DP16x2(vxc_char16 a,   vxc_char16 b,  vxc_modifier modifier, vxc_512bits u) _RET0_

_EXT_ vxc_uint4   viv_intrinsic_vxmc_DP8x4(vxc_uchar16 a,  vxc_uchar16 b, vxc_modifier modifier, vxc_512bits u) _RET0_
_EXT_ vxc_int4    viv_intrinsic_vxmc_DP8x4(vxc_char16 a,   vxc_char16 b,  vxc_modifier modifier, vxc_512bits u) _RET0_

_EXT_ vxc_ushort8 viv_intrinsic_vxmc_DP4x8(vxc_uchar16 a,  vxc_uchar16 b, vxc_modifier modifier, vxc_512bits u) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vxmc_DP4x8(vxc_char16 a,   vxc_char16 b,  vxc_modifier modifier, vxc_512bits u) _RET0_

_EXT_ vxc_uchar16 viv_intrinsic_vxmc_DP2x16(vxc_uchar16 a,  vxc_uchar16 b, vxc_modifier modifier, vxc_512bits u) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vxmc_DP2x16(vxc_char16 a,   vxc_char16 b,  vxc_modifier modifier, vxc_512bits u) _RET0_


_EXT_ vxc_uint    viv_intrinsic_vxmc_DP32x1_b(vxc_uchar16 a_hi,  vxc_uchar16 a_lo, vxc_uchar16 b, vxc_modifier modifier, vxc_512bits u) _RET0_
_EXT_ vxc_int     viv_intrinsic_vxmc_DP32x1_b(vxc_char16 a_hi,   vxc_char16 a_lo,  vxc_char16 b,  vxc_modifier modifier, vxc_512bits u) _RET0_

_EXT_ vxc_uint2   viv_intrinsic_vxmc_DP16x2_b(vxc_uchar16 a_hi,  vxc_uchar16 a_lo, vxc_uchar16 b, vxc_modifier modifier, vxc_512bits u) _RET0_
_EXT_ vxc_int2    viv_intrinsic_vxmc_DP16x2_b(vxc_char16 a_hi,   vxc_char16 a_lo,  vxc_char16 b,  vxc_modifier modifier, vxc_512bits u) _RET0_

_EXT_ vxc_uint4   viv_intrinsic_vxmc_DP8x4_b(vxc_uchar16 a_hi,  vxc_uchar16 a_lo, vxc_uchar16 b, vxc_modifier modifier, vxc_512bits u) _RET0_
_EXT_ vxc_int4    viv_intrinsic_vxmc_DP8x4_b(vxc_char16 a_hi,   vxc_char16 a_lo,  vxc_char16 b,  vxc_modifier modifier, vxc_512bits u) _RET0_

_EXT_ vxc_ushort8 viv_intrinsic_vxmc_DP4x8_b(vxc_uchar16 a_hi,  vxc_uchar16 a_lo, vxc_uchar16 b, vxc_modifier modifier, vxc_512bits u) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vxmc_DP4x8_b(vxc_char16 a_hi,   vxc_char16 a_lo,  vxc_char16 b,  vxc_modifier modifier, vxc_512bits u) _RET0_

_EXT_ vxc_uchar16 viv_intrinsic_vxmc_DP2x16_b(vxc_uchar16 a_hi,  vxc_uchar16 a_lo, vxc_uchar16 b, vxc_modifier modifier, vxc_512bits u) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vxmc_DP2x16_b(vxc_char16 a_hi,   vxc_char16 a_lo,  vxc_char16 b,  vxc_modifier modifier, vxc_512bits u) _RET0_


_EXT_ vxc_uchar16 viv_intrinsic_vxmc_Clamp_uc(vxc_uchar16 a, vxc_uchar16 b, vxc_uchar16 c, vxc_modifier modifier) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vxmc_Clamp_c(vxc_char16 a, vxc_char16 b, vxc_char16 c, vxc_modifier modifier) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vxmc_Clamp_s(vxc_short8 a, vxc_short8 b, vxc_short8 c, vxc_modifier modifier) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vxmc_Clamp_us(vxc_ushort8 a, vxc_ushort8 b, vxc_ushort8 c, vxc_modifier modifier) _RET0_


_EXT_ vxc_uchar16 viv_intrinsic_vxmc_BiLinear_uc(vxc_uchar16 a, vxc_uchar16 b, float2 c, vxc_modifier modifier) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vxmc_BiLinear_c(vxc_char16 a, vxc_char16 b, float2 c, vxc_modifier modifier) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vxmc_BiLinear_s(vxc_short8 a, vxc_short8 b, float2 c, vxc_modifier modifier) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vxmc_BiLinear_us(vxc_ushort8 a, vxc_ushort8 b, float2 c, vxc_modifier modifier) _RET0_


_EXT_ vxc_uchar16 viv_intrinsic_vxmc_SelectAdd_uc(vxc_uchar16 a, vxc_uchar16 b, vxc_512bits c, vxc_modifier modifier) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vxmc_SelectAdd_c(vxc_char16 a,  vxc_char16 b,  vxc_512bits c, vxc_modifier modifier) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vxmc_SelectAdd_s(vxc_short8 a,  vxc_short8 b,  vxc_512bits c, vxc_modifier modifier) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vxmc_SelectAdd_us(vxc_ushort8 a, vxc_ushort8 b, vxc_512bits c, vxc_modifier modifier) _RET0_


_EXT_ vxc_uchar16 viv_intrinsic_vxmc_AtomicAdd_uc(vxc_uchar16 * a, vxc_int offset, vxc_uchar16 c, vxc_modifier modifier) _RET0_
_EXT_ vxc_char16  viv_intrinsic_vxmc_AtomicAdd_c(vxc_char16 * a,  vxc_int offset, vxc_char16 c, vxc_modifier modifier) _RET0_
_EXT_ vxc_short8  viv_intrinsic_vxmc_AtomicAdd_s(vxc_short8 * a,  vxc_int offset,  vxc_short8 c, vxc_modifier modifier) _RET0_
_EXT_ vxc_ushort8 viv_intrinsic_vxmc_AtomicAdd_us(vxc_ushort8 * a, vxc_int offset, vxc_ushort8 c, vxc_modifier modifier) _RET0_


_EXT_ vxc_ushort8 viv_intrinsic_vxmc_BitExtract_us(vxc_ushort8 a, vxc_ushort8 b, vxc_uchar16 c, vxc_modifier modifier) _RET0_
_EXT_ vxc_uchar8  viv_intrinsic_vxmc_BitExtract_uc(vxc_uchar16 a, vxc_uchar16 b, vxc_uchar16 c, vxc_modifier modifier) _RET0_


_EXT_ vxc_ushort8 viv_intrinsic_vxmc_BitReplace_us(vxc_ushort8 a, vxc_ushort8 b, vxc_uchar16 c, vxc_modifier modifier) _RET0_
_EXT_ vxc_uchar16 viv_intrinsic_vxmc_BitReplace_uc(vxc_uchar16 a, vxc_uchar16 b, vxc_uchar16 c, vxc_modifier modifier) _RET0_


vxc_char2 viv_intrinsic_vx_vload2(size_t Offset, char *Pointer)  {
    vxc_char2 dest;
    VXC_OP2(vload2, dest, Pointer, Offset * sizeof(vxc_char2));
    return dest;
}

vxc_char4 viv_intrinsic_vx_vload4(size_t Offset, char *Pointer)  {
    vxc_char4 dest;
    VXC_OP2(vload4, dest, Pointer, Offset * sizeof(vxc_char4));
    return dest;
}

vxc_char8 viv_intrinsic_vx_vload8(size_t Offset, char *Pointer)  {
    vxc_char8 dest;
    VXC_OP2(vload8, dest, Pointer, Offset * sizeof(vxc_char8));
    return dest;
}

vxc_char16 viv_intrinsic_vx_vload16(size_t Offset, char *Pointer)  {
    vxc_char16 dest;
    VXC_OP2(vload16, dest, Pointer, Offset * sizeof(vxc_char16));
    return dest;
}

vxc_uchar2 viv_intrinsic_vx_vload2(size_t Offset, uchar *Pointer)  {
    vxc_uchar2 dest;
    VXC_OP2(vload2, dest, Pointer, Offset * sizeof(vxc_uchar2));
    return dest;
}

vxc_uchar4 viv_intrinsic_vx_vload4(size_t Offset, uchar *Pointer)  {
    vxc_uchar4 dest;
    VXC_OP2(vload4, dest, Pointer, Offset * sizeof(vxc_uchar4));
    return dest;
}

vxc_uchar8 viv_intrinsic_vx_vload8(size_t Offset, uchar *Pointer)  {
    vxc_uchar8 dest;
    VXC_OP2(vload8, dest, Pointer, Offset * sizeof(vxc_uchar8));
    return dest;
}

vxc_uchar16 viv_intrinsic_vx_vload16(size_t Offset, uchar *Pointer)  {
    vxc_uchar16 dest;
    VXC_OP2(vload16, dest, Pointer, Offset * sizeof(vxc_uchar16));
    return dest;
}

vxc_short2 viv_intrinsic_vx_vload2(size_t Offset, short *Pointer)  {
    vxc_short2 dest;
    VXC_OP2(vload2, dest, Pointer, Offset * sizeof(vxc_short2));
    return dest;
}

vxc_short4 viv_intrinsic_vx_vload4(size_t Offset, short *Pointer)  {
    vxc_short4 dest;
    VXC_OP2(vload4, dest, Pointer, Offset * sizeof(vxc_short4));
    return dest;
}

vxc_short8 viv_intrinsic_vx_vload8(size_t Offset, short *Pointer)  {
    vxc_short8 dest;
    VXC_OP2(vload8, dest, Pointer, Offset * sizeof(vxc_short8));
    return dest;
}

vxc_short16 viv_intrinsic_vx_vload16(size_t Offset, short *Pointer)  {
    vxc_short16 dest;
    VXC_OP2(vload16, dest, Pointer, Offset * sizeof(vxc_short16));
    return dest;
}

vxc_ushort2 viv_intrinsic_vx_vload2(size_t Offset, ushort *Pointer)  {
    vxc_ushort2 dest;
    VXC_OP2(vload2, dest, Pointer, Offset * sizeof(vxc_ushort2));
    return dest;
}

vxc_ushort4 viv_intrinsic_vx_vload4(size_t Offset, ushort *Pointer)  {
    vxc_ushort4 dest;
    VXC_OP2(vload4, dest, Pointer, Offset * sizeof(vxc_ushort4));
    return dest;
}

vxc_ushort8 viv_intrinsic_vx_vload8(size_t Offset, ushort *Pointer)  {
    vxc_ushort8 dest;
    VXC_OP2(vload8, dest, Pointer, Offset * sizeof(vxc_ushort8));
    return dest;
}

vxc_ushort16 viv_intrinsic_vx_vload16(size_t Offset, ushort *Pointer)  {
    vxc_ushort16 dest;
    VXC_OP2(vload16, dest, Pointer, Offset * sizeof(vxc_ushort16));
    return dest;
}

vxc_half2 viv_intrinsic_vx_vload2(size_t Offset, half *Pointer)  {
    vxc_half2 dest;
    VXC_OP2(vload2, dest, Pointer, Offset * sizeof(vxc_half2));
    return dest;
}

vxc_half4 viv_intrinsic_vx_vload4(size_t Offset, half *Pointer)  {
    vxc_half4 dest;
    VXC_OP2(vload4, dest, Pointer, Offset * sizeof(vxc_half4));
    return dest;
}

vxc_half8 viv_intrinsic_vx_vload8(size_t Offset, half *Pointer)  {
    vxc_half8 dest;
    VXC_OP2(vload8, dest, Pointer, Offset * sizeof(vxc_half8));
    return dest;
}

vxc_half16 viv_intrinsic_vx_vload16(size_t Offset, half *Pointer)  {
    vxc_half16 dest;
    VXC_OP2(vload16, dest, Pointer, Offset * sizeof(vxc_half16));
    return dest;
}


void viv_intrinsic_vx_vstore2(vxc_char2 Data, size_t Offset, char * Pointer)  {
   VXC_OP3_NoDest(vstore2, Pointer, Offset * sizeof(vxc_char2), Data);
}

void viv_intrinsic_vx_vstore4(vxc_char4 Data, size_t Offset, char * Pointer)  {
   VXC_OP3_NoDest(vstore4, Pointer, Offset * sizeof(vxc_char4), Data);
}

void viv_intrinsic_vx_vstore8(vxc_char8 Data, size_t Offset, char * Pointer)  {
   VXC_OP3_NoDest(vstore8, Pointer, Offset * sizeof(vxc_char8), Data);
}

void viv_intrinsic_vx_vstore16(vxc_char16 Data, size_t Offset, char * Pointer)  {
   VXC_OP3_NoDest(vstore16, Pointer, Offset * sizeof(vxc_char16), Data);
}

void viv_intrinsic_vx_vstore2(vxc_uchar2 Data, size_t Offset, uchar * Pointer)  {
   VXC_OP3_NoDest(vstore2, Pointer, Offset * sizeof(vxc_uchar2), Data);
}

void viv_intrinsic_vx_vstore4(vxc_uchar4 Data, size_t Offset, uchar * Pointer)  {
   VXC_OP3_NoDest(vstore4, Pointer, Offset * sizeof(vxc_uchar4), Data);
}

void viv_intrinsic_vx_vstore8(vxc_uchar8 Data, size_t Offset, uchar * Pointer)  {
   VXC_OP3_NoDest(vstore8, Pointer, Offset * sizeof(vxc_uchar8), Data);
}

void viv_intrinsic_vx_vstore16(vxc_uchar16 Data, size_t Offset, uchar * Pointer)  {
   VXC_OP3_NoDest(vstore16, Pointer, Offset * sizeof(vxc_uchar16), Data);
}

void viv_intrinsic_vx_vstore2(vxc_short2 Data, size_t Offset, short * Pointer)  {
   VXC_OP3_NoDest(vstore2, Pointer, Offset * sizeof(vxc_short2), Data);
}

void viv_intrinsic_vx_vstore4(vxc_short4 Data, size_t Offset, short * Pointer)  {
   VXC_OP3_NoDest(vstore4, Pointer, Offset * sizeof(vxc_short4), Data);
}

void viv_intrinsic_vx_vstore8(vxc_short8 Data, size_t Offset, short * Pointer)  {
   VXC_OP3_NoDest(vstore8, Pointer, Offset * sizeof(vxc_short8), Data);
}

void viv_intrinsic_vx_vstore16(vxc_short16 Data, size_t Offset, short * Pointer)  {
   VXC_OP3_NoDest(vstore16, Pointer, Offset * sizeof(vxc_short16), Data);
}

void viv_intrinsic_vx_vstore2(vxc_ushort2 Data, size_t Offset, ushort * Pointer)  {
   VXC_OP3_NoDest(vstore2, Pointer, Offset * sizeof(vxc_ushort2), Data);
}

void viv_intrinsic_vx_vstore4(vxc_ushort4 Data, size_t Offset, ushort * Pointer)  {
   VXC_OP3_NoDest(vstore4, Pointer, Offset * sizeof(vxc_ushort4), Data);
}

void viv_intrinsic_vx_vstore8(vxc_ushort8 Data, size_t Offset, ushort * Pointer)  {
   VXC_OP3_NoDest(vstore8, Pointer, Offset * sizeof(vxc_ushort8), Data);
}

void viv_intrinsic_vx_vstore16(vxc_ushort16 Data, size_t Offset, ushort * Pointer)  {
   VXC_OP3_NoDest(vstore16, Pointer, Offset * sizeof(vxc_ushort16), Data);
}

void viv_intrinsic_vx_vstore2(vxc_half2 Data, size_t Offset, half * Pointer)  {
   VXC_OP3_NoDest(vstore2, Pointer, Offset * sizeof(vxc_half2), Data);
}

void viv_intrinsic_vx_vstore4(vxc_half4 Data, size_t Offset, half * Pointer)  {
   VXC_OP3_NoDest(vstore4, Pointer, Offset * sizeof(vxc_half4), Data);
}

void viv_intrinsic_vx_vstore8(vxc_half8 Data, size_t Offset, half * Pointer)  {
   VXC_OP3_NoDest(vstore8, Pointer, Offset * sizeof(vxc_half8), Data);
}

void viv_intrinsic_vx_vstore16(vxc_half16 Data, size_t Offset, half * Pointer)  {
   VXC_OP3_NoDest(vstore16, Pointer, Offset * sizeof(vxc_half16), Data);
}

#undef _RET0_
#undef _RET_
#undef _EXT_

#ifdef __cplusplus
}
#endif

#endif
typedef struct
{
    size_t size;
    global char* item;
} vx_array_char;

typedef struct
{
    size_t size;
    global unsigned char* item;
} vx_array_uchar;

typedef struct
{
    size_t size;
    global short* item;
} vx_array_short;

typedef struct
{
    size_t size;
    global unsigned short* item;
} vx_array_ushort;

typedef struct
{
    size_t size;
    global int* item;
} vx_array_int;

typedef struct
{
    size_t size;
    global unsigned int* item;
} vx_array_uint;


typedef struct
{
    size_t size;
    global float * item;
} vx_array_float;

typedef struct
{
    size_t size;
    global unsigned char* item;
} vx_lut_uchar;

typedef struct
{
    size_t size;
    global unsigned short* item;
} vx_lut_ushort;

typedef struct
{
    size_t columns;
    size_t rows;
    global short* matrix;
    uint scale;
} vx_convolution;

typedef struct
{
    size_t columns;
    size_t rows;
    global char* matrix;
} vx_matrix_char;

typedef struct
{
    size_t columns;
    size_t rows;
    global unsigned char* matrix;
} vx_matrix_uchar;

typedef struct
{
    size_t columns;
    size_t rows;
    global short* matrix;
} vx_matrix_short;

typedef struct
{
    size_t columns;
    size_t rows;
    global unsigned short* matrix;
} vx_matrix_ushort;

typedef struct
{
    size_t columns;
    size_t rows;
    global int* matrix;
} vx_matrix_int;

typedef struct
{
    size_t columns;
    size_t rows;
    global unsigned int* matrix;
} vx_matrix_uint;

typedef struct
{
    size_t columns;
    size_t rows;
    global float* matrix;
} vx_matrix_float;

typedef struct
{
    int type;
    uint value;
    uint lower;
    uint upper;
    uint trueValue;
    uint falseValue;
} vx_threshold;

typedef struct  {
    int dst_width;
    int dst_height;
    global float* ptr;
} vx_remap;

typedef struct
{
    int bins;
    int rang;
    int offset;
    float window_r;
    global int* ptr;
} vx_distribution;

typedef struct _vxc_pyramid
{
    float scale;
    uint width;
    uint height;
    uint format;
    uint levelCount;
    _viv_image2d_array_t  imageArray;
} vxc_pyramid;

#endif 

#endif
