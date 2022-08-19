/*
 * Copyright 2022 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Authors: AMD
 *
 */

#ifndef __DML_DCN32_DISPLAY_MODE_VBA_UTIL_32_H__
#define __DML_DCN32_DISPLAY_MODE_VBA_UTIL_32_H__

#include "../display_mode_enums.h"
#include "os_types.h"
#include "../dc_features.h"
#include "../display_mode_structs.h"
#include "dml/display_mode_vba.h"

unsigned int dml32_dscceComputeDelay(
		unsigned int bpc,
		double BPP,
		unsigned int sliceWidth,
		unsigned int numSlices,
		enum output_format_class pixelFormat,
		enum output_encoder_class Output);

unsigned int dml32_dscComputeDelay(enum output_format_class pixelFormat, enum output_encoder_class Output);

bool IsVertical(enum dm_rotation_angle Scan);

void dml32_CalculateBytePerPixelAndBlockSizes(
		enum source_format_class SourcePixelFormat,
		enum dm_swizzle_mode SurfaceTiling,

		/*Output*/
		unsigned int *BytePerPixelY,
		unsigned int *BytePerPixelC,
		double           *BytePerPixelDETY,
		double           *BytePerPixelDETC,
		unsigned int *BlockHeight256BytesY,
		unsigned int *BlockHeight256BytesC,
		unsigned int *BlockWidth256BytesY,
		unsigned int *BlockWidth256BytesC,
		unsigned int *MacroTileHeightY,
		unsigned int *MacroTileHeightC,
		unsigned int *MacroTileWidthY,
		unsigned int *MacroTileWidthC);

void dml32_CalculateSinglePipeDPPCLKAndSCLThroughput(
		double HRatio,
		double HRatioChroma,
		double VRatio,
		double VRatioChroma,
		double MaxDCHUBToPSCLThroughput,
		double MaxPSCLToLBThroughput,
		double PixelClock,
		enum source_format_class SourcePixelFormat,
		unsigned int HTaps,
		unsigned int HTapsChroma,
		unsigned int VTaps,
		unsigned int VTapsChroma,

		/* output */
		double *PSCL_THROUGHPUT,
		double *PSCL_THROUGHPUT_CHROMA,
		double *DPPCLKUsingSingleDPP);

void dml32_CalculateSwathAndDETConfiguration(
		struct dml32_CalculateSwathAndDETConfiguration *st_vars,
		unsigned int DETSizeOverride[],
		enum dm_use_mall_for_pstate_change_mode UseMALLForPStateChange[],
		unsigned int ConfigReturnBufferSizeInKByte,
		unsigned int MaxTotalDETInKByte,
		unsigned int MinCompressedBufferSizeInKByte,
		double ForceSingleDPP,
		unsigned int NumberOfActiveSurfaces,
		unsigned int nomDETInKByte,
		enum unbounded_requesting_policy UseUnboundedRequestingFinal,
		bool DisableUnboundRequestIfCompBufReservedSpaceNeedAdjustment,
		unsigned int PixelChunkSizeKBytes,
		unsigned int ROBSizeKBytes,
		unsigned int CompressedBufferSegmentSizeInkByteFinal,
		enum output_encoder_class Output[],
		double ReadBandwidthLuma[],
		double ReadBandwidthChroma[],
		double MaximumSwathWidthLuma[],
		double MaximumSwathWidthChroma[],
		enum dm_rotation_angle SourceRotation[],
		bool ViewportStationary[],
		enum source_format_class SourcePixelFormat[],
		enum dm_swizzle_mode SurfaceTiling[],
		unsigned int ViewportWidth[],
		unsigned int ViewportHeight[],
		unsigned int ViewportXStart[],
		unsigned int ViewportYStart[],
		unsigned int ViewportXStartC[],
		unsigned int ViewportYStartC[],
		unsigned int SurfaceWidthY[],
		unsigned int SurfaceWidthC[],
		unsigned int SurfaceHeightY[],
		unsigned int SurfaceHeightC[],
		unsigned int Read256BytesBlockHeightY[],
		unsigned int Read256BytesBlockHeightC[],
		unsigned int Read256BytesBlockWidthY[],
		unsigned int Read256BytesBlockWidthC[],
		enum odm_combine_mode ODMMode[],
		unsigned int BlendingAndTiming[],
		unsigned int BytePerPixY[],
		unsigned int BytePerPixC[],
		double BytePerPixDETY[],
		double BytePerPixDETC[],
		unsigned int HActive[],
		double HRatio[],
		double HRatioChroma[],
		unsigned int DPPPerSurface[],

		/* Output */
		unsigned int swath_width_luma_ub[],
		unsigned int swath_width_chroma_ub[],
		double SwathWidth[],
		double SwathWidthChroma[],
		unsigned int SwathHeightY[],
		unsigned int SwathHeightC[],
		unsigned int DETBufferSizeInKByte[],
		unsigned int DETBufferSizeY[],
		unsigned int DETBufferSizeC[],
		bool *UnboundedRequestEnabled,
		unsigned int *CompressedBufferSizeInkByte,
		unsigned int *CompBufReservedSpaceKBytes,
		bool *CompBufReservedSpaceNeedAdjustment,
		bool ViewportSizeSupportPerSurface[],
		bool *ViewportSizeSupport);

void dml32_CalculateSwathWidth(
		bool ForceSingleDPP,
		unsigned int NumberOfActiveSurfaces,
		enum source_format_class SourcePixelFormat[],
		enum dm_rotation_angle SourceScan[],
		bool ViewportStationary[],
		unsigned int ViewportWidth[],
		unsigned int ViewportHeight[],
		unsigned int ViewportXStart[],
		unsigned int ViewportYStart[],
		unsigned int ViewportXStartC[],
		unsigned int ViewportYStartC[],
		unsigned int SurfaceWidthY[],
		unsigned int SurfaceWidthC[],
		unsigned int SurfaceHeightY[],
		unsigned int SurfaceHeightC[],
		enum odm_combine_mode ODMMode[],
		unsigned int BytePerPixY[],
		unsigned int BytePerPixC[],
		unsigned int Read256BytesBlockHeightY[],
		unsigned int Read256BytesBlockHeightC[],
		unsigned int Read256BytesBlockWidthY[],
		unsigned int Read256BytesBlockWidthC[],
		unsigned int BlendingAndTiming[],
		unsigned int HActive[],
		double HRatio[],
		unsigned int DPPPerSurface[],

		/* Output */
		double SwathWidthdoubleDPPY[],
		double SwathWidthdoubleDPPC[],
		double SwathWidthY[], // per-pipe
		double SwathWidthC[], // per-pipe
		unsigned int MaximumSwathHeightY[],
		unsigned int MaximumSwathHeightC[],
		unsigned int swath_width_luma_ub[], // per-pipe
		unsigned int swath_width_chroma_ub[]);

bool dml32_UnboundedRequest(enum unbounded_requesting_policy UseUnboundedRequestingFinal,
		unsigned int TotalNumberOfActiveDPP,
		bool NoChroma,
		enum output_encoder_class Output,
		enum dm_swizzle_mode SurfaceTiling,
		bool CompBufReservedSpaceNeedAdjustment,
		bool DisableUnboundRequestIfCompBufReservedSpaceNeedAdjustment);

void dml32_CalculateDETBufferSize(
		unsigned int DETSizeOverride[],
		enum dm_use_mall_for_pstate_change_mode UseMALLForPStateChange[],
		bool ForceSingleDPP,
		unsigned int NumberOfActiveSurfaces,
		bool UnboundedRequestEnabled,
		unsigned int nomDETInKByte,
		unsigned int MaxTotalDETInKByte,
		unsigned int ConfigReturnBufferSizeInKByte,
		unsigned int MinCompressedBufferSizeInKByte,
		unsigned int CompressedBufferSegmentSizeInkByteFinal,
		enum source_format_class SourcePixelFormat[],
		double ReadBandwidthLuma[],
		double ReadBandwidthChroma[],
		unsigned int RoundedUpMaxSwathSizeBytesY[],
		unsigned int RoundedUpMaxSwathSizeBytesC[],
		unsigned int DPPPerSurface[],
		/* Output */
		unsigned int DETBufferSizeInKByte[],
		unsigned int *CompressedBufferSizeInkByte);

void dml32_CalculateODMMode(
		unsigned int MaximumPixelsPerLinePerDSCUnit,
		unsigned int HActive,
		enum output_encoder_class Output,
		enum odm_combine_policy ODMUse,
		double StateDispclk,
		double MaxDispclk,
		bool DSCEnable,
		unsigned int TotalNumberOfActiveDPP,
		unsigned int MaxNumDPP,
		double PixelClock,
		double DISPCLKDPPCLKDSCCLKDownSpreading,
		double DISPCLKRampingMargin,
		double DISPCLKDPPCLKVCOSpeed,

		/* Output */
		bool *TotalAvailablePipesSupport,
		unsigned int *NumberOfDPP,
		enum odm_combine_mode *ODMMode,
		double *RequiredDISPCLKPerSurface);

double dml32_CalculateRequiredDispclk(
		enum odm_combine_mode ODMMode,
		double PixelClock,
		double DISPCLKDPPCLKDSCCLKDownSpreading,
		double DISPCLKRampingMargin,
		double DISPCLKDPPCLKVCOSpeed,
		double MaxDispclk);

double dml32_RoundToDFSGranularity(double Clock, bool round_up, double VCOSpeed);

void dml32_CalculateOutputLink(
		double PHYCLKPerState,
		double PHYCLKD18PerState,
		double PHYCLKD32PerState,
		double Downspreading,
		bool IsMainSurfaceUsingTheIndicatedTiming,
		enum output_encoder_class Output,
		enum output_format_class OutputFormat,
		unsigned int HTotal,
		unsigned int HActive,
		double PixelClockBackEnd,
		double ForcedOutputLinkBPP,
		unsigned int DSCInputBitPerComponent,
		unsigned int NumberOfDSCSlices,
		double AudioSampleRate,
		unsigned int AudioSampleLayout,
		enum odm_combine_mode ODMModeNoDSC,
		enum odm_combine_mode ODMModeDSC,
		bool DSCEnable,
		unsigned int OutputLinkDPLanes,
		enum dm_output_link_dp_rate OutputLinkDPRate,

		/* Output */
		bool *RequiresDSC,
		double *RequiresFEC,
		double  *OutBpp,
		enum dm_output_type *OutputType,
		enum dm_output_rate *OutputRate,
		unsigned int *RequiredSlots);

void dml32_CalculateDPPCLK(
		unsigned int NumberOfActiveSurfaces,
		double DISPCLKDPPCLKDSCCLKDownSpreading,
		double DISPCLKDPPCLKVCOSpeed,
		double DPPCLKUsingSingleDPP[],
		unsigned int DPPPerSurface[],

		/* output */
		double *GlobalDPPCLK,
		double Dppclk[]);

double dml32_TruncToValidBPP(
		double LinkBitRate,
		unsigned int Lanes,
		unsigned int HTotal,
		unsigned int HActive,
		double PixelClock,
		double DesiredBPP,
		bool DSCEnable,
		enum output_encoder_class Output,
		enum output_format_class Format,
		unsigned int DSCInputBitPerComponent,
		unsigned int DSCSlices,
		unsigned int AudioRate,
		unsigned int AudioLayout,
		enum odm_combine_mode ODMModeNoDSC,
		enum odm_combine_mode ODMModeDSC,
		/* Output */
		unsigned int *RequiredSlots);

double dml32_RequiredDTBCLK(
		bool              DSCEnable,
		double               PixelClock,
		enum output_format_class  OutputFormat,
		double               OutputBpp,
		unsigned int              DSCSlices,
		unsigned int                 HTotal,
		unsigned int                 HActive,
		unsigned int              AudioRate,
		unsigned int              AudioLayout);

unsigned int dml32_DSCDelayRequirement(bool DSCEnabled,
		enum odm_combine_mode ODMMode,
		unsigned int DSCInputBitPerComponent,
		double OutputBpp,
		unsigned int HActive,
		unsigned int HTotal,
		unsigned int NumberOfDSCSlices,
		enum output_format_class  OutputFormat,
		enum output_encoder_class Output,
		double PixelClock,
		double PixelClockBackEnd);

void dml32_CalculateSurfaceSizeInMall(
		unsigned int NumberOfActiveSurfaces,
		unsigned int MALLAllocatedForDCN,
		enum dm_use_mall_for_static_screen_mode UseMALLForStaticScreen[],
		bool DCCEnable[],
		bool ViewportStationary[],
		unsigned int ViewportXStartY[],
		unsigned int ViewportYStartY[],
		unsigned int ViewportXStartC[],
		unsigned int ViewportYStartC[],
		unsigned int ViewportWidthY[],
		unsigned int ViewportHeightY[],
		unsigned int BytesPerPixelY[],
		unsigned int ViewportWidthC[],
		unsigned int ViewportHeightC[],
		unsigned int BytesPerPixelC[],
		unsigned int SurfaceWidthY[],
		unsigned int SurfaceWidthC[],
		unsigned int SurfaceHeightY[],
		unsigned int SurfaceHeightC[],
		unsigned int Read256BytesBlockWidthY[],
		unsigned int Read256BytesBlockWidthC[],
		unsigned int Read256BytesBlockHeightY[],
		unsigned int Read256BytesBlockHeightC[],
		unsigned int ReadBlockWidthY[],
		unsigned int ReadBlockWidthC[],
		unsigned int ReadBlockHeightY[],
		unsigned int ReadBlockHeightC[],

		/* Output */
		unsigned int    SurfaceSizeInMALL[],
		bool *ExceededMALLSize);

void dml32_CalculateVMRowAndSwath(
		struct dml32_CalculateVMRowAndSwath *st_vars,
		unsigned int NumberOfActiveSurfaces,
		DmlPipe myPipe[],
		unsigned int SurfaceSizeInMALL[],
		unsigned int PTEBufferSizeInRequestsLuma,
		unsigned int PTEBufferSizeInRequestsChroma,
		unsigned int DCCMetaBufferSizeBytes,
		enum dm_use_mall_for_static_screen_mode UseMALLForStaticScreen[],
		enum dm_use_mall_for_pstate_change_mode UseMALLForPStateChange[],
		unsigned int MALLAllocatedForDCN,
		double SwathWidthY[],
		double SwathWidthC[],
		bool GPUVMEnable,
		bool HostVMEnable,
		unsigned int HostVMMaxNonCachedPageTableLevels,
		unsigned int GPUVMMaxPageTableLevels,
		unsigned int GPUVMMinPageSizeKBytes[],
		unsigned int HostVMMinPageSize,

		/* Output */
		bool PTEBufferSizeNotExceeded[],
		bool DCCMetaBufferSizeNotExceeded[],
		unsigned int dpte_row_width_luma_ub[],
		unsigned int dpte_row_width_chroma_ub[],
		unsigned int dpte_row_height_luma[],
		unsigned int dpte_row_height_chroma[],
		unsigned int dpte_row_height_linear_luma[],     // VBA_DELTA
		unsigned int dpte_row_height_linear_chroma[],   // VBA_DELTA
		unsigned int meta_req_width[],
		unsigned int meta_req_width_chroma[],
		unsigned int meta_req_height[],
		unsigned int meta_req_height_chroma[],
		unsigned int meta_row_width[],
		unsigned int meta_row_width_chroma[],
		unsigned int meta_row_height[],
		unsigned int meta_row_height_chroma[],
		unsigned int vm_group_bytes[],
		unsigned int dpte_group_bytes[],
		unsigned int PixelPTEReqWidthY[],
		unsigned int PixelPTEReqHeightY[],
		unsigned int PTERequestSizeY[],
		unsigned int PixelPTEReqWidthC[],
		unsigned int PixelPTEReqHeightC[],
		unsigned int PTERequestSizeC[],
		unsigned int dpde0_bytes_per_frame_ub_l[],
		unsigned int meta_pte_bytes_per_frame_ub_l[],
		unsigned int dpde0_bytes_per_frame_ub_c[],
		unsigned int meta_pte_bytes_per_frame_ub_c[],
		double PrefetchSourceLinesY[],
		double PrefetchSourceLinesC[],
		double VInitPreFillY[],
		double VInitPreFillC[],
		unsigned int MaxNumSwathY[],
		unsigned int MaxNumSwathC[],
		double meta_row_bw[],
		double dpte_row_bw[],
		double PixelPTEBytesPerRow[],
		double PDEAndMetaPTEBytesFrame[],
		double MetaRowByte[],
		bool use_one_row_for_frame[],
		bool use_one_row_for_frame_flip[],
		bool UsesMALLForStaticScreen[],
		bool PTE_BUFFER_MODE[],
		unsigned int BIGK_FRAGMENT_SIZE[]);

unsigned int dml32_CalculateVMAndRowBytes(
		bool ViewportStationary,
		bool DCCEnable,
		unsigned int NumberOfDPPs,
		unsigned int BlockHeight256Bytes,
		unsigned int BlockWidth256Bytes,
		enum source_format_class SourcePixelFormat,
		unsigned int SurfaceTiling,
		unsigned int BytePerPixel,
		enum dm_rotation_angle SourceScan,
		double SwathWidth,
		unsigned int ViewportHeight,
		unsigned int    ViewportXStart,
		unsigned int    ViewportYStart,
		bool GPUVMEnable,
		bool HostVMEnable,
		unsigned int HostVMMaxNonCachedPageTableLevels,
		unsigned int GPUVMMaxPageTableLevels,
		unsigned int GPUVMMinPageSizeKBytes,
		unsigned int HostVMMinPageSize,
		unsigned int PTEBufferSizeInRequests,
		unsigned int Pitch,
		unsigned int DCCMetaPitch,
		unsigned int MacroTileWidth,
		unsigned int MacroTileHeight,

		/* Output */
		unsigned int *MetaRowByte,
		unsigned int *PixelPTEBytesPerRow,
		unsigned int    *dpte_row_width_ub,
		unsigned int *dpte_row_height,
		unsigned int *dpte_row_height_linear,
		unsigned int    *PixelPTEBytesPerRow_one_row_per_frame,
		unsigned int    *dpte_row_width_ub_one_row_per_frame,
		unsigned int    *dpte_row_height_one_row_per_frame,
		unsigned int *MetaRequestWidth,
		unsigned int *MetaRequestHeight,
		unsigned int *meta_row_width,
		unsigned int *meta_row_height,
		unsigned int *PixelPTEReqWidth,
		unsigned int *PixelPTEReqHeight,
		unsigned int *PTERequestSize,
		unsigned int    *DPDE0BytesFrame,
		unsigned int    *MetaPTEBytesFrame);

double dml32_CalculatePrefetchSourceLines(
		double VRatio,
		unsigned int VTaps,
		bool Interlace,
		bool ProgressiveToInterlaceUnitInOPP,
		unsigned int SwathHeight,
		enum dm_rotation_angle SourceRotation,
		bool ViewportStationary,
		double SwathWidth,
		unsigned int ViewportHeight,
		unsigned int ViewportXStart,
		unsigned int ViewportYStart,

		/* Output */
		double *VInitPreFill,
		unsigned int *MaxNumSwath);

void dml32_CalculateMALLUseForStaticScreen(
		unsigned int NumberOfActiveSurfaces,
		unsigned int MALLAllocatedForDCNFinal,
		enum dm_use_mall_for_static_screen_mode *UseMALLForStaticScreen,
		unsigned int SurfaceSizeInMALL[],
		bool one_row_per_frame_fits_in_buffer[],

		/* output */
		bool UsesMALLForStaticScreen[]);

void dml32_CalculateRowBandwidth(
		bool GPUVMEnable,
		enum source_format_class SourcePixelFormat,
		double VRatio,
		double VRatioChroma,
		bool DCCEnable,
		double LineTime,
		unsigned int MetaRowByteLuma,
		unsigned int MetaRowByteChroma,
		unsigned int meta_row_height_luma,
		unsigned int meta_row_height_chroma,
		unsigned int PixelPTEBytesPerRowLuma,
		unsigned int PixelPTEBytesPerRowChroma,
		unsigned int dpte_row_height_luma,
		unsigned int dpte_row_height_chroma,
		/* Output */
		double *meta_row_bw,
		double *dpte_row_bw);

double dml32_CalculateUrgentLatency(
		double UrgentLatencyPixelDataOnly,
		double UrgentLatencyPixelMixedWithVMData,
		double UrgentLatencyVMDataOnly,
		bool   DoUrgentLatencyAdjustment,
		double UrgentLatencyAdjustmentFabricClockComponent,
		double UrgentLatencyAdjustmentFabricClockReference,
		double FabricClock);

void dml32_CalculateUrgentBurstFactor(
		enum dm_use_mall_for_pstate_change_mode UseMALLForPStateChange,
		unsigned int    swath_width_luma_ub,
		unsigned int    swath_width_chroma_ub,
		unsigned int SwathHeightY,
		unsigned int SwathHeightC,
		double  LineTime,
		double  UrgentLatency,
		double  CursorBufferSize,
		unsigned int CursorWidth,
		unsigned int CursorBPP,
		double  VRatio,
		double  VRatioC,
		double  BytePerPixelInDETY,
		double  BytePerPixelInDETC,
		unsigned int    DETBufferSizeY,
		unsigned int    DETBufferSizeC,
		/* Output */
		double *UrgentBurstFactorCursor,
		double *UrgentBurstFactorLuma,
		double *UrgentBurstFactorChroma,
		bool   *NotEnoughUrgentLatencyHiding);

void dml32_CalculateDCFCLKDeepSleep(
		unsigned int NumberOfActiveSurfaces,
		unsigned int BytePerPixelY[],
		unsigned int BytePerPixelC[],
		double VRatio[],
		double VRatioChroma[],
		double SwathWidthY[],
		double SwathWidthC[],
		unsigned int DPPPerSurface[],
		double HRatio[],
		double HRatioChroma[],
		double PixelClock[],
		double PSCL_THROUGHPUT[],
		double PSCL_THROUGHPUT_CHROMA[],
		double Dppclk[],
		double ReadBandwidthLuma[],
		double ReadBandwidthChroma[],
		unsigned int ReturnBusWidth,

		/* Output */
		double *DCFClkDeepSleep);

double dml32_CalculateWriteBackDelay(
		enum source_format_class WritebackPixelFormat,
		double WritebackHRatio,
		double WritebackVRatio,
		unsigned int WritebackVTaps,
		unsigned int         WritebackDestinationWidth,
		unsigned int         WritebackDestinationHeight,
		unsigned int         WritebackSourceHeight,
		unsigned int HTotal);

void dml32_UseMinimumDCFCLK(
		enum dm_use_mall_for_pstate_change_mode UseMALLForPStateChange[],
		bool DRRDisplay[],
		bool SynchronizeDRRDisplaysForUCLKPStateChangeFinal,
		unsigned int MaxInterDCNTileRepeaters,
		unsigned int MaxPrefetchMode,
		double DRAMClockChangeLatencyFinal,
		double FCLKChangeLatency,
		double SREnterPlusExitTime,
		unsigned int ReturnBusWidth,
		unsigned int RoundTripPingLatencyCycles,
		unsigned int ReorderingBytes,
		unsigned int PixelChunkSizeInKByte,
		unsigned int MetaChunkSize,
		bool GPUVMEnable,
		unsigned int GPUVMMaxPageTableLevels,
		bool HostVMEnable,
		unsigned int NumberOfActiveSurfaces,
		double HostVMMinPageSize,
		unsigned int HostVMMaxNonCachedPageTableLevels,
		bool DynamicMetadataVMEnabled,
		bool ImmediateFlipRequirement,
		bool ProgressiveToInterlaceUnitInOPP,
		double MaxAveragePercentOfIdealSDPPortBWDisplayCanUseInNormalSystemOperation,
		double PercentOfIdealSDPPortBWReceivedAfterUrgLatency,
		unsigned int VTotal[],
		unsigned int VActive[],
		unsigned int DynamicMetadataTransmittedBytes[],
		unsigned int DynamicMetadataLinesBeforeActiveRequired[],
		bool Interlace[],
		double RequiredDPPCLKPerSurface[][2][DC__NUM_DPP__MAX],
		double RequiredDISPCLK[][2],
		double UrgLatency[],
		unsigned int NoOfDPP[][2][DC__NUM_DPP__MAX],
		double ProjectedDCFClkDeepSleep[][2],
		double MaximumVStartup[][2][DC__NUM_DPP__MAX],
		unsigned int TotalNumberOfActiveDPP[][2],
		unsigned int TotalNumberOfDCCActiveDPP[][2],
		unsigned int dpte_group_bytes[],
		double PrefetchLinesY[][2][DC__NUM_DPP__MAX],
		double PrefetchLinesC[][2][DC__NUM_DPP__MAX],
		unsigned int swath_width_luma_ub_all_states[][2][DC__NUM_DPP__MAX],
		unsigned int swath_width_chroma_ub_all_states[][2][DC__NUM_DPP__MAX],
		unsigned int BytePerPixelY[],
		unsigned int BytePerPixelC[],
		unsigned int HTotal[],
		double PixelClock[],
		double PDEAndMetaPTEBytesPerFrame[][2][DC__NUM_DPP__MAX],
		double DPTEBytesPerRow[][2][DC__NUM_DPP__MAX],
		double MetaRowBytes[][2][DC__NUM_DPP__MAX],
		bool DynamicMetadataEnable[],
		double ReadBandwidthLuma[],
		double ReadBandwidthChroma[],
		double DCFCLKPerState[],
		/* Output */
		double DCFCLKState[][2]);

unsigned int dml32_CalculateExtraLatencyBytes(unsigned int ReorderingBytes,
		unsigned int TotalNumberOfActiveDPP,
		unsigned int PixelChunkSizeInKByte,
		unsigned int TotalNumberOfDCCActiveDPP,
		unsigned int MetaChunkSize,
		bool GPUVMEnable,
		bool HostVMEnable,
		unsigned int NumberOfActiveSurfaces,
		unsigned int NumberOfDPP[],
		unsigned int dpte_group_bytes[],
		double HostVMInefficiencyFactor,
		double HostVMMinPageSize,
		unsigned int HostVMMaxNonCachedPageTableLevels);

void dml32_CalculateVUpdateAndDynamicMetadataParameters(
		unsigned int MaxInterDCNTileRepeaters,
		double Dppclk,
		double Dispclk,
		double DCFClkDeepSleep,
		double PixelClock,
		unsigned int HTotal,
		unsigned int VBlank,
		unsigned int DynamicMetadataTransmittedBytes,
		unsigned int DynamicMetadataLinesBeforeActiveRequired,
		unsigned int InterlaceEnable,
		bool ProgressiveToInterlaceUnitInOPP,
		double *TSetup,
		double *Tdmbf,
		double *Tdmec,
		double *Tdmsks,
		unsigned int *VUpdateOffsetPix,
		double *VUpdateWidthPix,
		double *VReadyOffsetPix);

double dml32_CalculateTWait(
		unsigned int PrefetchMode,
		enum dm_use_mall_for_pstate_change_mode UseMALLForPStateChange,
		bool SynchronizeDRRDisplaysForUCLKPStateChangeFinal,
		bool DRRDisplay,
		double DRAMClockChangeLatency,
		double FCLKChangeLatency,
		double UrgentLatency,
		double SREnterPlusExitTime);

double dml32_get_return_bw_mbps(const soc_bounding_box_st *soc,
		const int VoltageLevel,
		const bool HostVMEnable,
		const double DCFCLK,
		const double FabricClock,
		const double DRAMSpeed);

double dml32_get_return_bw_mbps_vm_only(const soc_bounding_box_st *soc,
		const int VoltageLevel,
		const double DCFCLK,
		const double FabricClock,
		const double DRAMSpeed);

double dml32_CalculateExtraLatency(
		unsigned int RoundTripPingLatencyCycles,
		unsigned int ReorderingBytes,
		double DCFCLK,
		unsigned int TotalNumberOfActiveDPP,
		unsigned int PixelChunkSizeInKByte,
		unsigned int TotalNumberOfDCCActiveDPP,
		unsigned int MetaChunkSize,
		double ReturnBW,
		bool GPUVMEnable,
		bool HostVMEnable,
		unsigned int NumberOfActiveSurfaces,
		unsigned int NumberOfDPP[],
		unsigned int dpte_group_bytes[],
		double HostVMInefficiencyFactor,
		double HostVMMinPageSize,
		unsigned int HostVMMaxNonCachedPageTableLevels);

bool dml32_CalculatePrefetchSchedule(
		struct dml32_CalculatePrefetchSchedule *st_vars,
		double HostVMInefficiencyFactor,
		DmlPipe *myPipe,
		unsigned int DSCDelay,
		double DPPCLKDelaySubtotalPlusCNVCFormater,
		double DPPCLKDelaySCL,
		double DPPCLKDelaySCLLBOnly,
		double DPPCLKDelayCNVCCursor,
		double DISPCLKDelaySubtotal,
		unsigned int DPP_RECOUT_WIDTH,
		enum output_format_class OutputFormat,
		unsigned int MaxInterDCNTileRepeaters,
		unsigned int VStartup,
		unsigned int MaxVStartup,
		unsigned int GPUVMPageTableLevels,
		bool GPUVMEnable,
		bool HostVMEnable,
		unsigned int HostVMMaxNonCachedPageTableLevels,
		double HostVMMinPageSize,
		bool DynamicMetadataEnable,
		bool DynamicMetadataVMEnabled,
		int DynamicMetadataLinesBeforeActiveRequired,
		unsigned int DynamicMetadataTransmittedBytes,
		double UrgentLatency,
		double UrgentExtraLatency,
		double TCalc,
		unsigned int PDEAndMetaPTEBytesFrame,
		unsigned int MetaRowByte,
		unsigned int PixelPTEBytesPerRow,
		double PrefetchSourceLinesY,
		unsigned int SwathWidthY,
		unsigned int VInitPreFillY,
		unsigned int MaxNumSwathY,
		double PrefetchSourceLinesC,
		unsigned int SwathWidthC,
		unsigned int VInitPreFillC,
		unsigned int MaxNumSwathC,
		unsigned int swath_width_luma_ub,
		unsigned int swath_width_chroma_ub,
		unsigned int SwathHeightY,
		unsigned int SwathHeightC,
		double TWait,
		/* Output */
		double   *DSTXAfterScaler,
		double   *DSTYAfterScaler,
		double *DestinationLinesForPrefetch,
		double *PrefetchBandwidth,
		double *DestinationLinesToRequestVMInVBlank,
		double *DestinationLinesToRequestRowInVBlank,
		double *VRatioPrefetchY,
		double *VRatioPrefetchC,
		double *RequiredPrefetchPixDataBWLuma,
		double *RequiredPrefetchPixDataBWChroma,
		bool   *NotEnoughTimeForDynamicMetadata,
		double *Tno_bw,
		double *prefetch_vmrow_bw,
		double *Tdmdl_vm,
		double *Tdmdl,
		double *TSetup,
		unsigned int   *VUpdateOffsetPix,
		double   *VUpdateWidthPix,
		double   *VReadyOffsetPix);

void dml32_CalculateFlipSchedule(
		double HostVMInefficiencyFactor,
		double UrgentExtraLatency,
		double UrgentLatency,
		unsigned int GPUVMMaxPageTableLevels,
		bool HostVMEnable,
		unsigned int HostVMMaxNonCachedPageTableLevels,
		bool GPUVMEnable,
		double HostVMMinPageSize,
		double PDEAndMetaPTEBytesPerFrame,
		double MetaRowBytes,
		double DPTEBytesPerRow,
		double BandwidthAvailableForImmediateFlip,
		unsigned int TotImmediateFlipBytes,
		enum source_format_class SourcePixelFormat,
		double LineTime,
		double VRatio,
		double VRatioChroma,
		double Tno_bw,
		bool DCCEnable,
		unsigned int dpte_row_height,
		unsigned int meta_row_height,
		unsigned int dpte_row_height_chroma,
		unsigned int meta_row_height_chroma,
		bool    use_one_row_for_frame_flip,

		/* Output */
		double *DestinationLinesToRequestVMInImmediateFlip,
		double *DestinationLinesToRequestRowInImmediateFlip,
		double *final_flip_bw,
		bool *ImmediateFlipSupportedForPipe);

void dml32_CalculateWatermarksMALLUseAndDRAMSpeedChangeSupport(
		struct dml32_CalculateWatermarksMALLUseAndDRAMSpeedChangeSupport *st_vars,
		bool USRRetrainingRequiredFinal,
		enum dm_use_mall_for_pstate_change_mode UseMALLForPStateChange[],
		unsigned int PrefetchMode,
		unsigned int NumberOfActiveSurfaces,
		unsigned int MaxLineBufferLines,
		unsigned int LineBufferSize,
		unsigned int WritebackInterfaceBufferSize,
		double DCFCLK,
		double ReturnBW,
		bool SynchronizeTimingsFinal,
		bool SynchronizeDRRDisplaysForUCLKPStateChangeFinal,
		bool DRRDisplay[],
		unsigned int dpte_group_bytes[],
		unsigned int meta_row_height[],
		unsigned int meta_row_height_chroma[],
		SOCParametersList mmSOCParameters,
		unsigned int WritebackChunkSize,
		double SOCCLK,
		double DCFClkDeepSleep,
		unsigned int DETBufferSizeY[],
		unsigned int DETBufferSizeC[],
		unsigned int SwathHeightY[],
		unsigned int SwathHeightC[],
		unsigned int LBBitPerPixel[],
		double SwathWidthY[],
		double SwathWidthC[],
		double HRatio[],
		double HRatioChroma[],
		unsigned int VTaps[],
		unsigned int VTapsChroma[],
		double VRatio[],
		double VRatioChroma[],
		unsigned int HTotal[],
		unsigned int VTotal[],
		unsigned int VActive[],
		double PixelClock[],
		unsigned int BlendingAndTiming[],
		unsigned int DPPPerSurface[],
		double BytePerPixelDETY[],
		double BytePerPixelDETC[],
		double DSTXAfterScaler[],
		double DSTYAfterScaler[],
		bool WritebackEnable[],
		enum source_format_class WritebackPixelFormat[],
		double WritebackDestinationWidth[],
		double WritebackDestinationHeight[],
		double WritebackSourceHeight[],
		bool UnboundedRequestEnabled,
		unsigned int CompressedBufferSizeInkByte,

		/* Output */
		Watermarks *Watermark,
		enum clock_change_support *DRAMClockChangeSupport,
		double MaxActiveDRAMClockChangeLatencySupported[],
		unsigned int SubViewportLinesNeededInMALL[],
		enum dm_fclock_change_support *FCLKChangeSupport,
		double *MinActiveFCLKChangeLatencySupported,
		bool *USRRetrainingSupport,
		double ActiveDRAMClockChangeLatencyMargin[]);

double dml32_CalculateWriteBackDISPCLK(
		enum source_format_class WritebackPixelFormat,
		double PixelClock,
		double WritebackHRatio,
		double WritebackVRatio,
		unsigned int WritebackHTaps,
		unsigned int WritebackVTaps,
		unsigned int   WritebackSourceWidth,
		unsigned int   WritebackDestinationWidth,
		unsigned int HTotal,
		unsigned int WritebackLineBufferSize,
		double DISPCLKDPPCLKVCOSpeed);

void dml32_CalculateMinAndMaxPrefetchMode(
		enum dm_prefetch_modes   AllowForPStateChangeOrStutterInVBlankFinal,
		unsigned int             *MinPrefetchMode,
		unsigned int             *MaxPrefetchMode);

void dml32_CalculatePixelDeliveryTimes(
		unsigned int             NumberOfActiveSurfaces,
		double              VRatio[],
		double              VRatioChroma[],
		double              VRatioPrefetchY[],
		double              VRatioPrefetchC[],
		unsigned int             swath_width_luma_ub[],
		unsigned int             swath_width_chroma_ub[],
		unsigned int             DPPPerSurface[],
		double              HRatio[],
		double              HRatioChroma[],
		double              PixelClock[],
		double              PSCL_THROUGHPUT[],
		double              PSCL_THROUGHPUT_CHROMA[],
		double              Dppclk[],
		unsigned int             BytePerPixelC[],
		enum dm_rotation_angle   SourceRotation[],
		unsigned int             NumberOfCursors[],
		unsigned int             CursorWidth[][DC__NUM_CURSOR__MAX],
		unsigned int             CursorBPP[][DC__NUM_CURSOR__MAX],
		unsigned int             BlockWidth256BytesY[],
		unsigned int             BlockHeight256BytesY[],
		unsigned int             BlockWidth256BytesC[],
		unsigned int             BlockHeight256BytesC[],

		/* Output */
		double              DisplayPipeLineDeliveryTimeLuma[],
		double              DisplayPipeLineDeliveryTimeChroma[],
		double              DisplayPipeLineDeliveryTimeLumaPrefetch[],
		double              DisplayPipeLineDeliveryTimeChromaPrefetch[],
		double              DisplayPipeRequestDeliveryTimeLuma[],
		double              DisplayPipeRequestDeliveryTimeChroma[],
		double              DisplayPipeRequestDeliveryTimeLumaPrefetch[],
		double              DisplayPipeRequestDeliveryTimeChromaPrefetch[],
		double              CursorRequestDeliveryTime[],
		double              CursorRequestDeliveryTimePrefetch[]);

void dml32_CalculateMetaAndPTETimes(
		bool use_one_row_for_frame[],
		unsigned int NumberOfActiveSurfaces,
		bool GPUVMEnable,
		unsigned int MetaChunkSize,
		unsigned int MinMetaChunkSizeBytes,
		unsigned int    HTotal[],
		double  VRatio[],
		double  VRatioChroma[],
		double  DestinationLinesToRequestRowInVBlank[],
		double  DestinationLinesToRequestRowInImmediateFlip[],
		bool DCCEnable[],
		double  PixelClock[],
		unsigned int BytePerPixelY[],
		unsigned int BytePerPixelC[],
		enum dm_rotation_angle SourceRotation[],
		unsigned int dpte_row_height[],
		unsigned int dpte_row_height_chroma[],
		unsigned int meta_row_width[],
		unsigned int meta_row_width_chroma[],
		unsigned int meta_row_height[],
		unsigned int meta_row_height_chroma[],
		unsigned int meta_req_width[],
		unsigned int meta_req_width_chroma[],
		unsigned int meta_req_height[],
		unsigned int meta_req_height_chroma[],
		unsigned int dpte_group_bytes[],
		unsigned int    PTERequestSizeY[],
		unsigned int    PTERequestSizeC[],
		unsigned int    PixelPTEReqWidthY[],
		unsigned int    PixelPTEReqHeightY[],
		unsigned int    PixelPTEReqWidthC[],
		unsigned int    PixelPTEReqHeightC[],
		unsigned int    dpte_row_width_luma_ub[],
		unsigned int    dpte_row_width_chroma_ub[],

		/* Output */
		double DST_Y_PER_PTE_ROW_NOM_L[],
		double DST_Y_PER_PTE_ROW_NOM_C[],
		double DST_Y_PER_META_ROW_NOM_L[],
		double DST_Y_PER_META_ROW_NOM_C[],
		double TimePerMetaChunkNominal[],
		double TimePerChromaMetaChunkNominal[],
		double TimePerMetaChunkVBlank[],
		double TimePerChromaMetaChunkVBlank[],
		double TimePerMetaChunkFlip[],
		double TimePerChromaMetaChunkFlip[],
		double time_per_pte_group_nom_luma[],
		double time_per_pte_group_vblank_luma[],
		double time_per_pte_group_flip_luma[],
		double time_per_pte_group_nom_chroma[],
		double time_per_pte_group_vblank_chroma[],
		double time_per_pte_group_flip_chroma[]);

void dml32_CalculateVMGroupAndRequestTimes(
		unsigned int     NumberOfActiveSurfaces,
		bool     GPUVMEnable,
		unsigned int     GPUVMMaxPageTableLevels,
		unsigned int     HTotal[],
		unsigned int     BytePerPixelC[],
		double      DestinationLinesToRequestVMInVBlank[],
		double      DestinationLinesToRequestVMInImmediateFlip[],
		bool     DCCEnable[],
		double      PixelClock[],
		unsigned int        dpte_row_width_luma_ub[],
		unsigned int        dpte_row_width_chroma_ub[],
		unsigned int     vm_group_bytes[],
		unsigned int     dpde0_bytes_per_frame_ub_l[],
		unsigned int     dpde0_bytes_per_frame_ub_c[],
		unsigned int        meta_pte_bytes_per_frame_ub_l[],
		unsigned int        meta_pte_bytes_per_frame_ub_c[],

		/* Output */
		double      TimePerVMGroupVBlank[],
		double      TimePerVMGroupFlip[],
		double      TimePerVMRequestVBlank[],
		double      TimePerVMRequestFlip[]);

void dml32_CalculateDCCConfiguration(
		bool             DCCEnabled,
		bool             DCCProgrammingAssumesScanDirectionUnknown,
		enum source_format_class SourcePixelFormat,
		unsigned int             SurfaceWidthLuma,
		unsigned int             SurfaceWidthChroma,
		unsigned int             SurfaceHeightLuma,
		unsigned int             SurfaceHeightChroma,
		unsigned int                nomDETInKByte,
		unsigned int             RequestHeight256ByteLuma,
		unsigned int             RequestHeight256ByteChroma,
		enum dm_swizzle_mode     TilingFormat,
		unsigned int             BytePerPixelY,
		unsigned int             BytePerPixelC,
		double              BytePerPixelDETY,
		double              BytePerPixelDETC,
		enum dm_rotation_angle   SourceRotation,
		/* Output */
		unsigned int        *MaxUncompressedBlockLuma,
		unsigned int        *MaxUncompressedBlockChroma,
		unsigned int        *MaxCompressedBlockLuma,
		unsigned int        *MaxCompressedBlockChroma,
		unsigned int        *IndependentBlockLuma,
		unsigned int        *IndependentBlockChroma);

void dml32_CalculateStutterEfficiency(
		unsigned int      CompressedBufferSizeInkByte,
		enum dm_use_mall_for_pstate_change_mode UseMALLForPStateChange[],
		bool   UnboundedRequestEnabled,
		unsigned int      MetaFIFOSizeInKEntries,
		unsigned int      ZeroSizeBufferEntries,
		unsigned int      PixelChunkSizeInKByte,
		unsigned int   NumberOfActiveSurfaces,
		unsigned int      ROBBufferSizeInKByte,
		double    TotalDataReadBandwidth,
		double    DCFCLK,
		double    ReturnBW,
		unsigned int      CompbufReservedSpace64B,
		unsigned int      CompbufReservedSpaceZs,
		double    SRExitTime,
		double    SRExitZ8Time,
		bool   SynchronizeTimingsFinal,
		unsigned int   BlendingAndTiming[],
		double    StutterEnterPlusExitWatermark,
		double    Z8StutterEnterPlusExitWatermark,
		bool   ProgressiveToInterlaceUnitInOPP,
		bool   Interlace[],
		double    MinTTUVBlank[],
		unsigned int   DPPPerSurface[],
		unsigned int      DETBufferSizeY[],
		unsigned int   BytePerPixelY[],
		double    BytePerPixelDETY[],
		double      SwathWidthY[],
		unsigned int   SwathHeightY[],
		unsigned int   SwathHeightC[],
		double    NetDCCRateLuma[],
		double    NetDCCRateChroma[],
		double    DCCFractionOfZeroSizeRequestsLuma[],
		double    DCCFractionOfZeroSizeRequestsChroma[],
		unsigned int      HTotal[],
		unsigned int      VTotal[],
		double    PixelClock[],
		double    VRatio[],
		enum dm_rotation_angle SourceRotation[],
		unsigned int   BlockHeight256BytesY[],
		unsigned int   BlockWidth256BytesY[],
		unsigned int   BlockHeight256BytesC[],
		unsigned int   BlockWidth256BytesC[],
		unsigned int   DCCYMaxUncompressedBlock[],
		unsigned int   DCCCMaxUncompressedBlock[],
		unsigned int      VActive[],
		bool   DCCEnable[],
		bool   WritebackEnable[],
		double    ReadBandwidthSurfaceLuma[],
		double    ReadBandwidthSurfaceChroma[],
		double    meta_row_bw[],
		double    dpte_row_bw[],

		/* Output */
		double   *StutterEfficiencyNotIncludingVBlank,
		double   *StutterEfficiency,
		unsigned int     *NumberOfStutterBurstsPerFrame,
		double   *Z8StutterEfficiencyNotIncludingVBlank,
		double   *Z8StutterEfficiency,
		unsigned int     *Z8NumberOfStutterBurstsPerFrame,
		double   *StutterPeriod,
		bool  *DCHUBBUB_ARB_CSTATE_MAX_CAP_MODE);

void dml32_CalculateMaxDETAndMinCompressedBufferSize(
		unsigned int    ConfigReturnBufferSizeInKByte,
		unsigned int    ROBBufferSizeInKByte,
		unsigned int MaxNumDPP,
		bool nomDETInKByteOverrideEnable, // VBA_DELTA, allow DV to override default DET size
		unsigned int nomDETInKByteOverrideValue,  // VBA_DELTA

		/* Output */
		unsigned int *MaxTotalDETInKByte,
		unsigned int *nomDETInKByte,
		unsigned int *MinCompressedBufferSizeInKByte);

bool dml32_CalculateVActiveBandwithSupport(unsigned int NumberOfActiveSurfaces,
		double ReturnBW,
		bool NotUrgentLatencyHiding[],
		double ReadBandwidthLuma[],
		double ReadBandwidthChroma[],
		double cursor_bw[],
		double meta_row_bandwidth[],
		double dpte_row_bandwidth[],
		unsigned int NumberOfDPP[],
		double UrgentBurstFactorLuma[],
		double UrgentBurstFactorChroma[],
		double UrgentBurstFactorCursor[]);

void dml32_CalculatePrefetchBandwithSupport(unsigned int NumberOfActiveSurfaces,
		double ReturnBW,
		bool NotUrgentLatencyHiding[],
		double ReadBandwidthLuma[],
		double ReadBandwidthChroma[],
		double PrefetchBandwidthLuma[],
		double PrefetchBandwidthChroma[],
		double cursor_bw[],
		double meta_row_bandwidth[],
		double dpte_row_bandwidth[],
		double cursor_bw_pre[],
		double prefetch_vmrow_bw[],
		unsigned int NumberOfDPP[],
		double UrgentBurstFactorLuma[],
		double UrgentBurstFactorChroma[],
		double UrgentBurstFactorCursor[],
		double UrgentBurstFactorLumaPre[],
		double UrgentBurstFactorChromaPre[],
		double UrgentBurstFactorCursorPre[],

		/* output */
		double  *PrefetchBandwidth,
		double  *FractionOfUrgentBandwidth,
		bool *PrefetchBandwidthSupport);

double dml32_CalculateBandwidthAvailableForImmediateFlip(unsigned int NumberOfActiveSurfaces,
		double ReturnBW,
		double ReadBandwidthLuma[],
		double ReadBandwidthChroma[],
		double PrefetchBandwidthLuma[],
		double PrefetchBandwidthChroma[],
		double cursor_bw[],
		double cursor_bw_pre[],
		unsigned int NumberOfDPP[],
		double UrgentBurstFactorLuma[],
		double UrgentBurstFactorChroma[],
		double UrgentBurstFactorCursor[],
		double UrgentBurstFactorLumaPre[],
		double UrgentBurstFactorChromaPre[],
		double UrgentBurstFactorCursorPre[]);

void dml32_CalculateImmediateFlipBandwithSupport(unsigned int NumberOfActiveSurfaces,
		double ReturnBW,
		enum immediate_flip_requirement ImmediateFlipRequirement[],
		double final_flip_bw[],
		double ReadBandwidthLuma[],
		double ReadBandwidthChroma[],
		double PrefetchBandwidthLuma[],
		double PrefetchBandwidthChroma[],
		double cursor_bw[],
		double meta_row_bandwidth[],
		double dpte_row_bandwidth[],
		double cursor_bw_pre[],
		double prefetch_vmrow_bw[],
		unsigned int NumberOfDPP[],
		double UrgentBurstFactorLuma[],
		double UrgentBurstFactorChroma[],
		double UrgentBurstFactorCursor[],
		double UrgentBurstFactorLumaPre[],
		double UrgentBurstFactorChromaPre[],
		double UrgentBurstFactorCursorPre[],

		/* output */
		double  *TotalBandwidth,
		double  *FractionOfUrgentBandwidth,
		bool *ImmediateFlipBandwidthSupport);

#endif
