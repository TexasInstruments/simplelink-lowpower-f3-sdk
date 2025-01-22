/******************************************************************************
*  Filename:       hw_vce_h
******************************************************************************
*  Copyright (c) 2024 Texas Instruments Incorporated. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*
*  1) Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
*
*  2) Redistributions in binary form must reproduce the above copyright notice,
*     this list of conditions and the following disclaimer in the documentation
*     and/or other materials provided with the distribution.
*
*  3) Neither the name of the copyright holder nor the names of its contributors
*     may be used to endorse or promote products derived from this software
*     without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
*  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
*  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
*  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
*  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
*  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
*  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
*  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
*  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

#ifndef __HW_VCE_H__
#define __HW_VCE_H__

//*****************************************************************************
//
// This section defines the register offsets of
// VCE component
//
//*****************************************************************************
// IP Description
#define VCE_O_DESC                                                  0x00000000U

// Interrupt mask register
#define VCE_O_IMASK                                                 0x00000044U

// Raw interrupt flag register
#define VCE_O_RIS                                                   0x00000048U

// Masked interrupt flag register
#define VCE_O_MIS                                                   0x0000004CU

// Interrupt flag set register
#define VCE_O_ISET                                                  0x00000050U

// Interrupt flag clear register
#define VCE_O_ICLR                                                  0x00000054U

// Interrupt mask set register
#define VCE_O_IMSET                                                 0x00000058U

// Interrupt mask clear register
#define VCE_O_IMCLR                                                 0x0000005CU

// Internal. Only to be used through TI provided API.
#define VCE_O_ENABLE                                                0x00000800U

// Internal. Only to be used through TI provided API.
#define VCE_O_FWSRC                                                 0x00000804U

// Internal. Only to be used through TI provided API.
#define VCE_O_INIT                                                  0x00000808U

// Internal. Only to be used through TI provided API.
#define VCE_O_PDREQ                                                 0x0000080CU

// Internal. Only to be used through TI provided API.
#define VCE_O_API                                                   0x00000810U

// Internal. Only to be used through TI provided API.
#define VCE_O_MSGBOX                                                0x00000814U

// Internal. Only to be used through TI provided API.
#define VCE_O_CMDPAR0                                               0x00000818U

// Internal. Only to be used through TI provided API.
#define VCE_O_CMDPAR1                                               0x0000081CU

// Internal. Only to be used through TI provided API.
#define VCE_O_CMDPAR2                                               0x00000820U

// Internal. Only to be used through TI provided API.
#define VCE_O_CMDPAR3                                               0x00000824U

// Internal. Only to be used through TI provided API.
#define VCE_O_CMDPAR4                                               0x00000828U

// Internal. Only to be used through TI provided API.
#define VCE_O_CMDPAR5                                               0x0000082CU

// Internal. Only to be used through TI provided API.
#define VCE_O_STROBES                                               0x00000830U

// Internal. Only to be used through TI provided API.
#define VCE_O_IRQ                                                   0x00000834U

// Internal. Only to be used through TI provided API.
#define VCE_O_EVT                                                   0x00000838U

// Internal. Only to be used through TI provided API.
#define VCE_O_EVTMSK                                                0x0000083CU

// Internal. Only to be used through TI provided API.
#define VCE_O_EVTCLR                                                0x00000840U

// Internal. Only to be used through TI provided API.
#define VCE_O_GPO                                                   0x00000844U

// Internal. Only to be used through TI provided API.
#define VCE_O_GPOE                                                  0x00000848U

// Internal. Only to be used through TI provided API.
#define VCE_O_GPI                                                   0x0000084CU

// Internal. Only to be used through TI provided API.
#define VCE_O_TRCCTL                                                0x00000850U

// Internal. Only to be used through TI provided API.
#define VCE_O_TRCSTAT                                               0x00000854U

// Internal. Only to be used through TI provided API.
#define VCE_O_TRCCMD                                                0x00000858U

// Internal. Only to be used through TI provided API.
#define VCE_O_TRCPAR0                                               0x0000085CU

// Internal. Only to be used through TI provided API.
#define VCE_O_TRCPAR1                                               0x00000860U

// Internal. Only to be used through TI provided API.
#define VCE_O_TIMCTL                                                0x00000864U

// Internal. Only to be used through TI provided API.
#define VCE_O_TIMINC                                                0x00000868U

// Internal. Only to be used through TI provided API.
#define VCE_O_TIMPER                                                0x0000086CU

// Internal. Only to be used through TI provided API.
#define VCE_O_TIMCNT                                                0x00000870U

// Internal. Only to be used through TI provided API.
#define VCE_O_TIMCAPT                                               0x00000874U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSECTL                                                0x00000878U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSESTART                                              0x0000087CU

// Internal. Only to be used through TI provided API.
#define VCE_O_LSEBASESRCA                                           0x00000880U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSEMODESRCA                                           0x00000884U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSESUBMODESRCA                                        0x00000888U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSENSRCA                                              0x0000088CU

// Internal. Only to be used through TI provided API.
#define VCE_O_LSEMSRCA                                              0x00000890U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSEELEMENTSRCA                                        0x00000894U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSESTRIDESRCA                                         0x00000898U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSEBASEDSTA                                           0x0000089CU

// Internal. Only to be used through TI provided API.
#define VCE_O_LSEMODEDSTA                                           0x000008A0U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSESUBMODEDSTA                                        0x000008A4U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSENDSTA                                              0x000008A8U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSEMDSTA                                              0x000008ACU

// Internal. Only to be used through TI provided API.
#define VCE_O_LSEELEMENTDSTA                                        0x000008B0U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSESTRIDEDSTA                                         0x000008B4U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSEBASESRCB                                           0x000008B8U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSEMODESRCB                                           0x000008BCU

// Internal. Only to be used through TI provided API.
#define VCE_O_LSESUBMODESRCB                                        0x000008C0U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSENSRCB                                              0x000008C4U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSEMSRCB                                              0x000008C8U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSEELEMENTSRCB                                        0x000008CCU

// Internal. Only to be used through TI provided API.
#define VCE_O_LSESTRIDESRCB                                         0x000008D0U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSEBASEDSTB                                           0x000008D4U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSEMODEDSTB                                           0x000008D8U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSESUBMODEDSTB                                        0x000008DCU

// Internal. Only to be used through TI provided API.
#define VCE_O_LSENDSTB                                              0x000008E0U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSEMDSTB                                              0x000008E4U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSEELEMENTDSTB                                        0x000008E8U

// Internal. Only to be used through TI provided API.
#define VCE_O_LSESTRIDEDSTB                                         0x000008ECU

// Internal. Only to be used through TI provided API.
#define VCE_O_XBAR0                                                 0x000008F0U

// Internal. Only to be used through TI provided API.
#define VCE_O_XBAR1                                                 0x000008F4U

// Internal. Only to be used through TI provided API.
#define VCE_O_XBAR2                                                 0x000008F8U

// Internal. Only to be used through TI provided API.
#define VCE_O_R2C                                                   0x000008FCU

// Internal. Only to be used through TI provided API.
#define VCE_O_FMUL0                                                 0x00000900U

// Internal. Only to be used through TI provided API.
#define VCE_O_FMUL1                                                 0x00000904U

// Internal. Only to be used through TI provided API.
#define VCE_O_UCRD                                                  0x00000908U

// Internal. Only to be used through TI provided API.
#define VCE_O_ADDSUB                                                0x0000090CU

// Internal. Only to be used through TI provided API.
#define VCE_O_ADDSUBDECACC                                          0x00000910U

// Internal. Only to be used through TI provided API.
#define VCE_O_ADDSUBSTAT                                            0x00000914U

// Internal. Only to be used through TI provided API.
#define VCE_O_MAXMIN                                                0x00000918U

// Internal. Only to be used through TI provided API.
#define VCE_O_MAXMINDECACC                                          0x0000091CU

// Internal. Only to be used through TI provided API.
#define VCE_O_MAXMININDEX                                           0x00000920U

// Internal. Only to be used through TI provided API.
#define VCE_O_FX2FP                                                 0x00000924U

// Internal. Only to be used through TI provided API.
#define VCE_O_FX2FPR                                                0x00000928U

// Internal. Only to be used through TI provided API.
#define VCE_O_FX2FPCONVR                                            0x0000092CU

// Internal. Only to be used through TI provided API.
#define VCE_O_FDIV                                                  0x00000930U

// Internal. Only to be used through TI provided API.
#define VCE_O_FDIVSTAT                                              0x00000934U

// Internal. Only to be used through TI provided API.
#define VCE_O_CFG                                                   0x00000C00U

// Internal. Only to be used through TI provided API.
#define VCE_O_CH1CMD                                                0x00000C04U

// Internal. Only to be used through TI provided API.
#define VCE_O_CH2CMD                                                0x00000C08U

// Internal. Only to be used through TI provided API.
#define VCE_O_CH3CMD                                                0x00000C0CU

// Internal. Only to be used through TI provided API.
#define VCE_O_CH1PAR01                                              0x00000C14U

// Internal. Only to be used through TI provided API.
#define VCE_O_CH2PAR01                                              0x00000C18U

// Internal. Only to be used through TI provided API.
#define VCE_O_CH3PAR01                                              0x00000C1CU

// Internal. Only to be used through TI provided API.
#define VCE_O_CH1PAR23                                              0x00000C24U

// Internal. Only to be used through TI provided API.
#define VCE_O_CH2PAR23                                              0x00000C28U

// Internal. Only to be used through TI provided API.
#define VCE_O_CH3PAR23                                              0x00000C2CU

//*****************************************************************************
//
// Register: VCE_O_DESC
//
//*****************************************************************************
// Field: [31:16] MODID
//
// Module identifier
#define VCE_DESC_MODID_W                                                    16U
#define VCE_DESC_MODID_M                                            0xFFFF0000U
#define VCE_DESC_MODID_S                                                    16U

// Field: [15:12] STDIPOFF
//
// Standard IP MMR block offset
#define VCE_DESC_STDIPOFF_W                                                  4U
#define VCE_DESC_STDIPOFF_M                                         0x0000F000U
#define VCE_DESC_STDIPOFF_S                                                 12U

// Field:   [7:4] MAJREV
//
// Major revision
#define VCE_DESC_MAJREV_W                                                    4U
#define VCE_DESC_MAJREV_M                                           0x000000F0U
#define VCE_DESC_MAJREV_S                                                    4U

// Field:   [3:0] MINREV
//
// Minor revision
#define VCE_DESC_MINREV_W                                                    4U
#define VCE_DESC_MINREV_M                                           0x0000000FU
#define VCE_DESC_MINREV_S                                                    0U

//*****************************************************************************
//
// Register: VCE_O_IMASK
//
//*****************************************************************************
// Field:     [3] MSGBOX
//
// Hardware defined interrupt triggered on msgbox write.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_IMASK_MSGBOX                                            0x00000008U
#define VCE_IMASK_MSGBOX_M                                          0x00000008U
#define VCE_IMASK_MSGBOX_S                                                   3U
#define VCE_IMASK_MSGBOX_YES                                        0x00000008U
#define VCE_IMASK_MSGBOX_ZERO                                       0x00000000U

// Field:     [2] SOFT2
//
// Software defined interrupt. Not in use.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_IMASK_SOFT2                                             0x00000004U
#define VCE_IMASK_SOFT2_M                                           0x00000004U
#define VCE_IMASK_SOFT2_S                                                    2U
#define VCE_IMASK_SOFT2_YES                                         0x00000004U
#define VCE_IMASK_SOFT2_ZERO                                        0x00000000U

// Field:     [1] SOFT1
//
// Software defined interrupt. Not in use.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_IMASK_SOFT1                                             0x00000002U
#define VCE_IMASK_SOFT1_M                                           0x00000002U
#define VCE_IMASK_SOFT1_S                                                    1U
#define VCE_IMASK_SOFT1_YES                                         0x00000002U
#define VCE_IMASK_SOFT1_ZERO                                        0x00000000U

// Field:     [0] API
//
// VCE API finished mask.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_IMASK_API                                               0x00000001U
#define VCE_IMASK_API_M                                             0x00000001U
#define VCE_IMASK_API_S                                                      0U
#define VCE_IMASK_API_YES                                           0x00000001U
#define VCE_IMASK_API_ZERO                                          0x00000000U

//*****************************************************************************
//
// Register: VCE_O_RIS
//
//*****************************************************************************
// Field:     [3] MSGBOX
//
// Hardware defined interrupt triggered on msgbox write.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_RIS_MSGBOX                                              0x00000008U
#define VCE_RIS_MSGBOX_M                                            0x00000008U
#define VCE_RIS_MSGBOX_S                                                     3U
#define VCE_RIS_MSGBOX_YES                                          0x00000008U
#define VCE_RIS_MSGBOX_ZERO                                         0x00000000U

// Field:     [2] SOFT2
//
// Software defined interrupt. Not in use.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_RIS_SOFT2                                               0x00000004U
#define VCE_RIS_SOFT2_M                                             0x00000004U
#define VCE_RIS_SOFT2_S                                                      2U
#define VCE_RIS_SOFT2_YES                                           0x00000004U
#define VCE_RIS_SOFT2_ZERO                                          0x00000000U

// Field:     [1] SOFT1
//
// Software defined interrupt. Not in use.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_RIS_SOFT1                                               0x00000002U
#define VCE_RIS_SOFT1_M                                             0x00000002U
#define VCE_RIS_SOFT1_S                                                      1U
#define VCE_RIS_SOFT1_YES                                           0x00000002U
#define VCE_RIS_SOFT1_ZERO                                          0x00000000U

// Field:     [0] API
//
// VCE API finished. This flag is set when an API call is finished.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_RIS_API                                                 0x00000001U
#define VCE_RIS_API_M                                               0x00000001U
#define VCE_RIS_API_S                                                        0U
#define VCE_RIS_API_YES                                             0x00000001U
#define VCE_RIS_API_ZERO                                            0x00000000U

//*****************************************************************************
//
// Register: VCE_O_MIS
//
//*****************************************************************************
// Field:     [3] MSGBOX
//
// Hardware defined interrupt triggered on msgbox write.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_MIS_MSGBOX                                              0x00000008U
#define VCE_MIS_MSGBOX_M                                            0x00000008U
#define VCE_MIS_MSGBOX_S                                                     3U
#define VCE_MIS_MSGBOX_YES                                          0x00000008U
#define VCE_MIS_MSGBOX_ZERO                                         0x00000000U

// Field:     [2] SOFT2
//
// Software defined interrupt. Not in use.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_MIS_SOFT2                                               0x00000004U
#define VCE_MIS_SOFT2_M                                             0x00000004U
#define VCE_MIS_SOFT2_S                                                      2U
#define VCE_MIS_SOFT2_YES                                           0x00000004U
#define VCE_MIS_SOFT2_ZERO                                          0x00000000U

// Field:     [1] SOFT1
//
// Software defined interrupt. Not in use.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_MIS_SOFT1                                               0x00000002U
#define VCE_MIS_SOFT1_M                                             0x00000002U
#define VCE_MIS_SOFT1_S                                                      1U
#define VCE_MIS_SOFT1_YES                                           0x00000002U
#define VCE_MIS_SOFT1_ZERO                                          0x00000000U

// Field:     [0] API
//
// VCE API finished masked interrupt status.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_MIS_API                                                 0x00000001U
#define VCE_MIS_API_M                                               0x00000001U
#define VCE_MIS_API_S                                                        0U
#define VCE_MIS_API_YES                                             0x00000001U
#define VCE_MIS_API_ZERO                                            0x00000000U

//*****************************************************************************
//
// Register: VCE_O_ISET
//
//*****************************************************************************
// Field:     [3] MSGBOX
//
// Hardware defined interrupt triggered on msgbox write.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_ISET_MSGBOX                                             0x00000008U
#define VCE_ISET_MSGBOX_M                                           0x00000008U
#define VCE_ISET_MSGBOX_S                                                    3U
#define VCE_ISET_MSGBOX_YES                                         0x00000008U
#define VCE_ISET_MSGBOX_ZERO                                        0x00000000U

// Field:     [2] SOFT2
//
// Software defined interrupt. Not in use.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_ISET_SOFT2                                              0x00000004U
#define VCE_ISET_SOFT2_M                                            0x00000004U
#define VCE_ISET_SOFT2_S                                                     2U
#define VCE_ISET_SOFT2_YES                                          0x00000004U
#define VCE_ISET_SOFT2_ZERO                                         0x00000000U

// Field:     [1] SOFT1
//
// Software defined interrupt. Not in use.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_ISET_SOFT1                                              0x00000002U
#define VCE_ISET_SOFT1_M                                            0x00000002U
#define VCE_ISET_SOFT1_S                                                     1U
#define VCE_ISET_SOFT1_YES                                          0x00000002U
#define VCE_ISET_SOFT1_ZERO                                         0x00000000U

// Field:     [0] API
//
// Set the VCE API finish interrupt.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_ISET_API                                                0x00000001U
#define VCE_ISET_API_M                                              0x00000001U
#define VCE_ISET_API_S                                                       0U
#define VCE_ISET_API_YES                                            0x00000001U
#define VCE_ISET_API_ZERO                                           0x00000000U

//*****************************************************************************
//
// Register: VCE_O_ICLR
//
//*****************************************************************************
// Field:     [3] MSGBOX
//
// Hardware defined interrupt triggered on msgbox write.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_ICLR_MSGBOX                                             0x00000008U
#define VCE_ICLR_MSGBOX_M                                           0x00000008U
#define VCE_ICLR_MSGBOX_S                                                    3U
#define VCE_ICLR_MSGBOX_YES                                         0x00000008U
#define VCE_ICLR_MSGBOX_ZERO                                        0x00000000U

// Field:     [2] SOFT2
//
// Software defined interrupt. Not in use.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_ICLR_SOFT2                                              0x00000004U
#define VCE_ICLR_SOFT2_M                                            0x00000004U
#define VCE_ICLR_SOFT2_S                                                     2U
#define VCE_ICLR_SOFT2_YES                                          0x00000004U
#define VCE_ICLR_SOFT2_ZERO                                         0x00000000U

// Field:     [1] SOFT1
//
// Software defined interrupt. Not in use.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_ICLR_SOFT1                                              0x00000002U
#define VCE_ICLR_SOFT1_M                                            0x00000002U
#define VCE_ICLR_SOFT1_S                                                     1U
#define VCE_ICLR_SOFT1_YES                                          0x00000002U
#define VCE_ICLR_SOFT1_ZERO                                         0x00000000U

// Field:     [0] API
//
// Clear the VCE API finish interrupt.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_ICLR_API                                                0x00000001U
#define VCE_ICLR_API_M                                              0x00000001U
#define VCE_ICLR_API_S                                                       0U
#define VCE_ICLR_API_YES                                            0x00000001U
#define VCE_ICLR_API_ZERO                                           0x00000000U

//*****************************************************************************
//
// Register: VCE_O_IMSET
//
//*****************************************************************************
// Field:     [3] MSGBOX
//
// Hardware defined interrupt triggered on msgbox write.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_IMSET_MSGBOX                                            0x00000008U
#define VCE_IMSET_MSGBOX_M                                          0x00000008U
#define VCE_IMSET_MSGBOX_S                                                   3U
#define VCE_IMSET_MSGBOX_YES                                        0x00000008U
#define VCE_IMSET_MSGBOX_ZERO                                       0x00000000U

// Field:     [2] SOFT2
//
// Software defined interrupt. Not in use.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_IMSET_SOFT2                                             0x00000004U
#define VCE_IMSET_SOFT2_M                                           0x00000004U
#define VCE_IMSET_SOFT2_S                                                    2U
#define VCE_IMSET_SOFT2_YES                                         0x00000004U
#define VCE_IMSET_SOFT2_ZERO                                        0x00000000U

// Field:     [1] SOFT1
//
// Software defined interrupt. Not in use.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_IMSET_SOFT1                                             0x00000002U
#define VCE_IMSET_SOFT1_M                                           0x00000002U
#define VCE_IMSET_SOFT1_S                                                    1U
#define VCE_IMSET_SOFT1_YES                                         0x00000002U
#define VCE_IMSET_SOFT1_ZERO                                        0x00000000U

// Field:     [0] API
//
// Set the VCE API finish mask.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_IMSET_API                                               0x00000001U
#define VCE_IMSET_API_M                                             0x00000001U
#define VCE_IMSET_API_S                                                      0U
#define VCE_IMSET_API_YES                                           0x00000001U
#define VCE_IMSET_API_ZERO                                          0x00000000U

//*****************************************************************************
//
// Register: VCE_O_IMCLR
//
//*****************************************************************************
// Field:     [3] MSGBOX
//
// Hardware defined interrupt triggered on msgbox write.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_IMCLR_MSGBOX                                            0x00000008U
#define VCE_IMCLR_MSGBOX_M                                          0x00000008U
#define VCE_IMCLR_MSGBOX_S                                                   3U
#define VCE_IMCLR_MSGBOX_YES                                        0x00000008U
#define VCE_IMCLR_MSGBOX_ZERO                                       0x00000000U

// Field:     [2] SOFT2
//
// Software defined interrupt. Not in use.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_IMCLR_SOFT2                                             0x00000004U
#define VCE_IMCLR_SOFT2_M                                           0x00000004U
#define VCE_IMCLR_SOFT2_S                                                    2U
#define VCE_IMCLR_SOFT2_YES                                         0x00000004U
#define VCE_IMCLR_SOFT2_ZERO                                        0x00000000U

// Field:     [1] SOFT1
//
// Software defined interrupt. Not in use.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_IMCLR_SOFT1                                             0x00000002U
#define VCE_IMCLR_SOFT1_M                                           0x00000002U
#define VCE_IMCLR_SOFT1_S                                                    1U
#define VCE_IMCLR_SOFT1_YES                                         0x00000002U
#define VCE_IMCLR_SOFT1_ZERO                                        0x00000000U

// Field:     [0] API
//
// Clear the VCE API finish mask.
// ENUMs:
// YES                      The bit is 1
// ZERO                     The bit is 0
#define VCE_IMCLR_API                                               0x00000001U
#define VCE_IMCLR_API_M                                             0x00000001U
#define VCE_IMCLR_API_S                                                      0U
#define VCE_IMCLR_API_YES                                           0x00000001U
#define VCE_IMCLR_API_ZERO                                          0x00000000U

//*****************************************************************************
//
// Register: VCE_O_ENABLE
//
//*****************************************************************************
// Field:     [1] LSE
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_ENABLE_LSE                                              0x00000002U
#define VCE_ENABLE_LSE_M                                            0x00000002U
#define VCE_ENABLE_LSE_S                                                     1U
#define VCE_ENABLE_LSE_ONE                                          0x00000002U
#define VCE_ENABLE_LSE_ZERO                                         0x00000000U

// Field:     [0] TOPSM
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_ENABLE_TOPSM                                            0x00000001U
#define VCE_ENABLE_TOPSM_M                                          0x00000001U
#define VCE_ENABLE_TOPSM_S                                                   0U
#define VCE_ENABLE_TOPSM_ONE                                        0x00000001U
#define VCE_ENABLE_TOPSM_ZERO                                       0x00000000U

//*****************************************************************************
//
// Register: VCE_O_FWSRC
//
//*****************************************************************************
// Field:     [0] BANK
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_FWSRC_BANK                                              0x00000001U
#define VCE_FWSRC_BANK_M                                            0x00000001U
#define VCE_FWSRC_BANK_S                                                     0U
#define VCE_FWSRC_BANK_ONE                                          0x00000001U
#define VCE_FWSRC_BANK_ZERO                                         0x00000000U

//*****************************************************************************
//
// Register: VCE_O_INIT
//
//*****************************************************************************
// Field:     [8] FDIV
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_INIT_FDIV                                               0x00000100U
#define VCE_INIT_FDIV_M                                             0x00000100U
#define VCE_INIT_FDIV_S                                                      8U
#define VCE_INIT_FDIV_ONE                                           0x00000100U
#define VCE_INIT_FDIV_ZERO                                          0x00000000U

// Field:     [7] MAXMIN
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_INIT_MAXMIN                                             0x00000080U
#define VCE_INIT_MAXMIN_M                                           0x00000080U
#define VCE_INIT_MAXMIN_S                                                    7U
#define VCE_INIT_MAXMIN_ONE                                         0x00000080U
#define VCE_INIT_MAXMIN_ZERO                                        0x00000000U

// Field:     [6] ADDSUB
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_INIT_ADDSUB                                             0x00000040U
#define VCE_INIT_ADDSUB_M                                           0x00000040U
#define VCE_INIT_ADDSUB_S                                                    6U
#define VCE_INIT_ADDSUB_ONE                                         0x00000040U
#define VCE_INIT_ADDSUB_ZERO                                        0x00000000U

// Field:     [5] UCRD
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_INIT_UCRD                                               0x00000020U
#define VCE_INIT_UCRD_M                                             0x00000020U
#define VCE_INIT_UCRD_S                                                      5U
#define VCE_INIT_UCRD_ONE                                           0x00000020U
#define VCE_INIT_UCRD_ZERO                                          0x00000000U

// Field:     [4] FMUL1
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_INIT_FMUL1                                              0x00000010U
#define VCE_INIT_FMUL1_M                                            0x00000010U
#define VCE_INIT_FMUL1_S                                                     4U
#define VCE_INIT_FMUL1_ONE                                          0x00000010U
#define VCE_INIT_FMUL1_ZERO                                         0x00000000U

// Field:     [3] FMUL0
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_INIT_FMUL0                                              0x00000008U
#define VCE_INIT_FMUL0_M                                            0x00000008U
#define VCE_INIT_FMUL0_S                                                     3U
#define VCE_INIT_FMUL0_ONE                                          0x00000008U
#define VCE_INIT_FMUL0_ZERO                                         0x00000000U

// Field:     [2] ARBITER
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_INIT_ARBITER                                            0x00000004U
#define VCE_INIT_ARBITER_M                                          0x00000004U
#define VCE_INIT_ARBITER_S                                                   2U
#define VCE_INIT_ARBITER_ONE                                        0x00000004U
#define VCE_INIT_ARBITER_ZERO                                       0x00000000U

// Field:     [1] LSE
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_INIT_LSE                                                0x00000002U
#define VCE_INIT_LSE_M                                              0x00000002U
#define VCE_INIT_LSE_S                                                       1U
#define VCE_INIT_LSE_ONE                                            0x00000002U
#define VCE_INIT_LSE_ZERO                                           0x00000000U

// Field:     [0] TOPSM
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_INIT_TOPSM                                              0x00000001U
#define VCE_INIT_TOPSM_M                                            0x00000001U
#define VCE_INIT_TOPSM_S                                                     0U
#define VCE_INIT_TOPSM_ONE                                          0x00000001U
#define VCE_INIT_TOPSM_ZERO                                         0x00000000U

//*****************************************************************************
//
// Register: VCE_O_PDREQ
//
//*****************************************************************************
// Field:     [0] TOPSMPDREQ
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_PDREQ_TOPSMPDREQ                                        0x00000001U
#define VCE_PDREQ_TOPSMPDREQ_M                                      0x00000001U
#define VCE_PDREQ_TOPSMPDREQ_S                                               0U
#define VCE_PDREQ_TOPSMPDREQ_ONE                                    0x00000001U
#define VCE_PDREQ_TOPSMPDREQ_ZERO                                   0x00000000U

//*****************************************************************************
//
// Register: VCE_O_API
//
//*****************************************************************************
// Field:   [4:0] CMD
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_API_CMD_W                                                        5U
#define VCE_API_CMD_M                                               0x0000001FU
#define VCE_API_CMD_S                                                        0U
#define VCE_API_CMD_ALLONES                                         0x0000001FU
#define VCE_API_CMD_ALLZEROS                                        0x00000000U

//*****************************************************************************
//
// Register: VCE_O_MSGBOX
//
//*****************************************************************************
// Field:   [7:0] VAL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_MSGBOX_VAL_W                                                     8U
#define VCE_MSGBOX_VAL_M                                            0x000000FFU
#define VCE_MSGBOX_VAL_S                                                     0U
#define VCE_MSGBOX_VAL_ALLONES                                      0x000000FFU
#define VCE_MSGBOX_VAL_ALLZEROS                                     0x00000000U

//*****************************************************************************
//
// Register: VCE_O_CMDPAR0
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CMDPAR0_VAL_W                                                   11U
#define VCE_CMDPAR0_VAL_M                                           0x000007FFU
#define VCE_CMDPAR0_VAL_S                                                    0U
#define VCE_CMDPAR0_VAL_ALLONES                                     0x000000FFU
#define VCE_CMDPAR0_VAL_ALLZEROS                                    0x00000000U

//*****************************************************************************
//
// Register: VCE_O_CMDPAR1
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_CMDPAR1_VAL_W                                                   11U
#define VCE_CMDPAR1_VAL_M                                           0x000007FFU
#define VCE_CMDPAR1_VAL_S                                                    0U

//*****************************************************************************
//
// Register: VCE_O_CMDPAR2
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_CMDPAR2_VAL_W                                                   11U
#define VCE_CMDPAR2_VAL_M                                           0x000007FFU
#define VCE_CMDPAR2_VAL_S                                                    0U

//*****************************************************************************
//
// Register: VCE_O_CMDPAR3
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_CMDPAR3_VAL_W                                                   11U
#define VCE_CMDPAR3_VAL_M                                           0x000007FFU
#define VCE_CMDPAR3_VAL_S                                                    0U

//*****************************************************************************
//
// Register: VCE_O_CMDPAR4
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_CMDPAR4_VAL_W                                                   11U
#define VCE_CMDPAR4_VAL_M                                           0x000007FFU
#define VCE_CMDPAR4_VAL_S                                                    0U

//*****************************************************************************
//
// Register: VCE_O_CMDPAR5
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_CMDPAR5_VAL_W                                                   11U
#define VCE_CMDPAR5_VAL_M                                           0x000007FFU
#define VCE_CMDPAR5_VAL_S                                                    0U

//*****************************************************************************
//
// Register: VCE_O_STROBES
//
//*****************************************************************************
// Field:     [0] S0
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_STROBES_S0                                              0x00000001U
#define VCE_STROBES_S0_M                                            0x00000001U
#define VCE_STROBES_S0_S                                                     0U
#define VCE_STROBES_S0_ONE                                          0x00000001U
#define VCE_STROBES_S0_ZERO                                         0x00000000U

//*****************************************************************************
//
// Register: VCE_O_IRQ
//
//*****************************************************************************
// Field:     [2] SOFT2
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// YES                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_IRQ_SOFT2                                               0x00000004U
#define VCE_IRQ_SOFT2_M                                             0x00000004U
#define VCE_IRQ_SOFT2_S                                                      2U
#define VCE_IRQ_SOFT2_YES                                           0x00000004U
#define VCE_IRQ_SOFT2_ZERO                                          0x00000000U

// Field:     [1] SOFT1
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// YES                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_IRQ_SOFT1                                               0x00000002U
#define VCE_IRQ_SOFT1_M                                             0x00000002U
#define VCE_IRQ_SOFT1_S                                                      1U
#define VCE_IRQ_SOFT1_YES                                           0x00000002U
#define VCE_IRQ_SOFT1_ZERO                                          0x00000000U

// Field:     [0] SOFT0
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// YES                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_IRQ_SOFT0                                               0x00000001U
#define VCE_IRQ_SOFT0_M                                             0x00000001U
#define VCE_IRQ_SOFT0_S                                                      0U
#define VCE_IRQ_SOFT0_YES                                           0x00000001U
#define VCE_IRQ_SOFT0_ZERO                                          0x00000000U

//*****************************************************************************
//
// Register: VCE_O_EVT
//
//*****************************************************************************
// Field:     [4] LSEPUSHFINISH
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_EVT_LSEPUSHFINISH                                       0x00000010U
#define VCE_EVT_LSEPUSHFINISH_M                                     0x00000010U
#define VCE_EVT_LSEPUSHFINISH_S                                              4U
#define VCE_EVT_LSEPUSHFINISH_ONE                                   0x00000010U
#define VCE_EVT_LSEPUSHFINISH_ZERO                                  0x00000000U

// Field:     [3] LSEPULLFINISH
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_EVT_LSEPULLFINISH                                       0x00000008U
#define VCE_EVT_LSEPULLFINISH_M                                     0x00000008U
#define VCE_EVT_LSEPULLFINISH_S                                              3U
#define VCE_EVT_LSEPULLFINISH_ONE                                   0x00000008U
#define VCE_EVT_LSEPULLFINISH_ZERO                                  0x00000000U

// Field:     [2] COUNTER
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_EVT_COUNTER                                             0x00000004U
#define VCE_EVT_COUNTER_M                                           0x00000004U
#define VCE_EVT_COUNTER_S                                                    2U
#define VCE_EVT_COUNTER_ONE                                         0x00000004U
#define VCE_EVT_COUNTER_ZERO                                        0x00000000U

// Field:     [1] TIMER
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_EVT_TIMER                                               0x00000002U
#define VCE_EVT_TIMER_M                                             0x00000002U
#define VCE_EVT_TIMER_S                                                      1U
#define VCE_EVT_TIMER_ONE                                           0x00000002U
#define VCE_EVT_TIMER_ZERO                                          0x00000000U

// Field:     [0] API
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_EVT_API                                                 0x00000001U
#define VCE_EVT_API_M                                               0x00000001U
#define VCE_EVT_API_S                                                        0U
#define VCE_EVT_API_ONE                                             0x00000001U
#define VCE_EVT_API_ZERO                                            0x00000000U

//*****************************************************************************
//
// Register: VCE_O_EVTMSK
//
//*****************************************************************************
// Field:     [4] LSEPUSHFINISH
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_EVTMSK_LSEPUSHFINISH                                    0x00000010U
#define VCE_EVTMSK_LSEPUSHFINISH_M                                  0x00000010U
#define VCE_EVTMSK_LSEPUSHFINISH_S                                           4U
#define VCE_EVTMSK_LSEPUSHFINISH_ONE                                0x00000010U
#define VCE_EVTMSK_LSEPUSHFINISH_ZERO                               0x00000000U

// Field:     [3] LSEPULLFINISH
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_EVTMSK_LSEPULLFINISH                                    0x00000008U
#define VCE_EVTMSK_LSEPULLFINISH_M                                  0x00000008U
#define VCE_EVTMSK_LSEPULLFINISH_S                                           3U
#define VCE_EVTMSK_LSEPULLFINISH_ONE                                0x00000008U
#define VCE_EVTMSK_LSEPULLFINISH_ZERO                               0x00000000U

// Field:     [2] COUNTER
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_EVTMSK_COUNTER                                          0x00000004U
#define VCE_EVTMSK_COUNTER_M                                        0x00000004U
#define VCE_EVTMSK_COUNTER_S                                                 2U
#define VCE_EVTMSK_COUNTER_ONE                                      0x00000004U
#define VCE_EVTMSK_COUNTER_ZERO                                     0x00000000U

// Field:     [1] TIMER
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_EVTMSK_TIMER                                            0x00000002U
#define VCE_EVTMSK_TIMER_M                                          0x00000002U
#define VCE_EVTMSK_TIMER_S                                                   1U
#define VCE_EVTMSK_TIMER_ONE                                        0x00000002U
#define VCE_EVTMSK_TIMER_ZERO                                       0x00000000U

// Field:     [0] API
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_EVTMSK_API                                              0x00000001U
#define VCE_EVTMSK_API_M                                            0x00000001U
#define VCE_EVTMSK_API_S                                                     0U
#define VCE_EVTMSK_API_ONE                                          0x00000001U
#define VCE_EVTMSK_API_ZERO                                         0x00000000U

//*****************************************************************************
//
// Register: VCE_O_EVTCLR
//
//*****************************************************************************
// Field:     [4] LSEPUSHFINISH
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_EVTCLR_LSEPUSHFINISH                                    0x00000010U
#define VCE_EVTCLR_LSEPUSHFINISH_M                                  0x00000010U
#define VCE_EVTCLR_LSEPUSHFINISH_S                                           4U
#define VCE_EVTCLR_LSEPUSHFINISH_ONE                                0x00000010U
#define VCE_EVTCLR_LSEPUSHFINISH_ZERO                               0x00000000U

// Field:     [3] LSEPULLFINISH
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_EVTCLR_LSEPULLFINISH                                    0x00000008U
#define VCE_EVTCLR_LSEPULLFINISH_M                                  0x00000008U
#define VCE_EVTCLR_LSEPULLFINISH_S                                           3U
#define VCE_EVTCLR_LSEPULLFINISH_ONE                                0x00000008U
#define VCE_EVTCLR_LSEPULLFINISH_ZERO                               0x00000000U

// Field:     [2] COUNTER
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_EVTCLR_COUNTER                                          0x00000004U
#define VCE_EVTCLR_COUNTER_M                                        0x00000004U
#define VCE_EVTCLR_COUNTER_S                                                 2U
#define VCE_EVTCLR_COUNTER_ONE                                      0x00000004U
#define VCE_EVTCLR_COUNTER_ZERO                                     0x00000000U

// Field:     [1] TIMER
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_EVTCLR_TIMER                                            0x00000002U
#define VCE_EVTCLR_TIMER_M                                          0x00000002U
#define VCE_EVTCLR_TIMER_S                                                   1U
#define VCE_EVTCLR_TIMER_ONE                                        0x00000002U
#define VCE_EVTCLR_TIMER_ZERO                                       0x00000000U

// Field:     [0] API
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_EVTCLR_API                                              0x00000001U
#define VCE_EVTCLR_API_M                                            0x00000001U
#define VCE_EVTCLR_API_S                                                     0U
#define VCE_EVTCLR_API_ONE                                          0x00000001U
#define VCE_EVTCLR_API_ZERO                                         0x00000000U

//*****************************************************************************
//
// Register: VCE_O_GPO
//
//*****************************************************************************
// Field:     [7] GPO7
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPO_GPO7                                                0x00000080U
#define VCE_GPO_GPO7_M                                              0x00000080U
#define VCE_GPO_GPO7_S                                                       7U
#define VCE_GPO_GPO7_ONE                                            0x00000080U
#define VCE_GPO_GPO7_ZERO                                           0x00000000U

// Field:     [6] GPO6
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPO_GPO6                                                0x00000040U
#define VCE_GPO_GPO6_M                                              0x00000040U
#define VCE_GPO_GPO6_S                                                       6U
#define VCE_GPO_GPO6_ONE                                            0x00000040U
#define VCE_GPO_GPO6_ZERO                                           0x00000000U

// Field:     [5] GPO5
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPO_GPO5                                                0x00000020U
#define VCE_GPO_GPO5_M                                              0x00000020U
#define VCE_GPO_GPO5_S                                                       5U
#define VCE_GPO_GPO5_ONE                                            0x00000020U
#define VCE_GPO_GPO5_ZERO                                           0x00000000U

// Field:     [4] GPO4
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPO_GPO4                                                0x00000010U
#define VCE_GPO_GPO4_M                                              0x00000010U
#define VCE_GPO_GPO4_S                                                       4U
#define VCE_GPO_GPO4_ONE                                            0x00000010U
#define VCE_GPO_GPO4_ZERO                                           0x00000000U

// Field:     [3] GPO3
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPO_GPO3                                                0x00000008U
#define VCE_GPO_GPO3_M                                              0x00000008U
#define VCE_GPO_GPO3_S                                                       3U
#define VCE_GPO_GPO3_ONE                                            0x00000008U
#define VCE_GPO_GPO3_ZERO                                           0x00000000U

// Field:     [2] GPO2
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPO_GPO2                                                0x00000004U
#define VCE_GPO_GPO2_M                                              0x00000004U
#define VCE_GPO_GPO2_S                                                       2U
#define VCE_GPO_GPO2_ONE                                            0x00000004U
#define VCE_GPO_GPO2_ZERO                                           0x00000000U

// Field:     [1] GPO1
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPO_GPO1                                                0x00000002U
#define VCE_GPO_GPO1_M                                              0x00000002U
#define VCE_GPO_GPO1_S                                                       1U
#define VCE_GPO_GPO1_ONE                                            0x00000002U
#define VCE_GPO_GPO1_ZERO                                           0x00000000U

// Field:     [0] GPO0
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPO_GPO0                                                0x00000001U
#define VCE_GPO_GPO0_M                                              0x00000001U
#define VCE_GPO_GPO0_S                                                       0U
#define VCE_GPO_GPO0_ONE                                            0x00000001U
#define VCE_GPO_GPO0_ZERO                                           0x00000000U

//*****************************************************************************
//
// Register: VCE_O_GPOE
//
//*****************************************************************************
// Field:     [7] GPOE7
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPOE_GPOE7                                              0x00000080U
#define VCE_GPOE_GPOE7_M                                            0x00000080U
#define VCE_GPOE_GPOE7_S                                                     7U
#define VCE_GPOE_GPOE7_ONE                                          0x00000080U
#define VCE_GPOE_GPOE7_ZERO                                         0x00000000U

// Field:     [6] GPOE6
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPOE_GPOE6                                              0x00000040U
#define VCE_GPOE_GPOE6_M                                            0x00000040U
#define VCE_GPOE_GPOE6_S                                                     6U
#define VCE_GPOE_GPOE6_ONE                                          0x00000040U
#define VCE_GPOE_GPOE6_ZERO                                         0x00000000U

// Field:     [5] GPOE5
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPOE_GPOE5                                              0x00000020U
#define VCE_GPOE_GPOE5_M                                            0x00000020U
#define VCE_GPOE_GPOE5_S                                                     5U
#define VCE_GPOE_GPOE5_ONE                                          0x00000020U
#define VCE_GPOE_GPOE5_ZERO                                         0x00000000U

// Field:     [4] GPOE4
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPOE_GPOE4                                              0x00000010U
#define VCE_GPOE_GPOE4_M                                            0x00000010U
#define VCE_GPOE_GPOE4_S                                                     4U
#define VCE_GPOE_GPOE4_ONE                                          0x00000010U
#define VCE_GPOE_GPOE4_ZERO                                         0x00000000U

// Field:     [3] GPOE3
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPOE_GPOE3                                              0x00000008U
#define VCE_GPOE_GPOE3_M                                            0x00000008U
#define VCE_GPOE_GPOE3_S                                                     3U
#define VCE_GPOE_GPOE3_ONE                                          0x00000008U
#define VCE_GPOE_GPOE3_ZERO                                         0x00000000U

// Field:     [2] GPOE2
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPOE_GPOE2                                              0x00000004U
#define VCE_GPOE_GPOE2_M                                            0x00000004U
#define VCE_GPOE_GPOE2_S                                                     2U
#define VCE_GPOE_GPOE2_ONE                                          0x00000004U
#define VCE_GPOE_GPOE2_ZERO                                         0x00000000U

// Field:     [1] GPOE1
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPOE_GPOE1                                              0x00000002U
#define VCE_GPOE_GPOE1_M                                            0x00000002U
#define VCE_GPOE_GPOE1_S                                                     1U
#define VCE_GPOE_GPOE1_ONE                                          0x00000002U
#define VCE_GPOE_GPOE1_ZERO                                         0x00000000U

// Field:     [0] GPOE0
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPOE_GPOE0                                              0x00000001U
#define VCE_GPOE_GPOE0_M                                            0x00000001U
#define VCE_GPOE_GPOE0_S                                                     0U
#define VCE_GPOE_GPOE0_ONE                                          0x00000001U
#define VCE_GPOE_GPOE0_ZERO                                         0x00000000U

//*****************************************************************************
//
// Register: VCE_O_GPI
//
//*****************************************************************************
// Field:     [7] GPI7
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPI_GPI7                                                0x00000080U
#define VCE_GPI_GPI7_M                                              0x00000080U
#define VCE_GPI_GPI7_S                                                       7U
#define VCE_GPI_GPI7_ONE                                            0x00000080U
#define VCE_GPI_GPI7_ZERO                                           0x00000000U

// Field:     [6] GPI6
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPI_GPI6                                                0x00000040U
#define VCE_GPI_GPI6_M                                              0x00000040U
#define VCE_GPI_GPI6_S                                                       6U
#define VCE_GPI_GPI6_ONE                                            0x00000040U
#define VCE_GPI_GPI6_ZERO                                           0x00000000U

// Field:     [5] GPI5
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPI_GPI5                                                0x00000020U
#define VCE_GPI_GPI5_M                                              0x00000020U
#define VCE_GPI_GPI5_S                                                       5U
#define VCE_GPI_GPI5_ONE                                            0x00000020U
#define VCE_GPI_GPI5_ZERO                                           0x00000000U

// Field:     [4] GPI4
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPI_GPI4                                                0x00000010U
#define VCE_GPI_GPI4_M                                              0x00000010U
#define VCE_GPI_GPI4_S                                                       4U
#define VCE_GPI_GPI4_ONE                                            0x00000010U
#define VCE_GPI_GPI4_ZERO                                           0x00000000U

// Field:     [3] GPI3
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPI_GPI3                                                0x00000008U
#define VCE_GPI_GPI3_M                                              0x00000008U
#define VCE_GPI_GPI3_S                                                       3U
#define VCE_GPI_GPI3_ONE                                            0x00000008U
#define VCE_GPI_GPI3_ZERO                                           0x00000000U

// Field:     [2] GPI2
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPI_GPI2                                                0x00000004U
#define VCE_GPI_GPI2_M                                              0x00000004U
#define VCE_GPI_GPI2_S                                                       2U
#define VCE_GPI_GPI2_ONE                                            0x00000004U
#define VCE_GPI_GPI2_ZERO                                           0x00000000U

// Field:     [1] GPI1
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPI_GPI1                                                0x00000002U
#define VCE_GPI_GPI1_M                                              0x00000002U
#define VCE_GPI_GPI1_S                                                       1U
#define VCE_GPI_GPI1_ONE                                            0x00000002U
#define VCE_GPI_GPI1_ZERO                                           0x00000000U

// Field:     [0] GPI0
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_GPI_GPI0                                                0x00000001U
#define VCE_GPI_GPI0_M                                              0x00000001U
#define VCE_GPI_GPI0_S                                                       0U
#define VCE_GPI_GPI0_ONE                                            0x00000001U
#define VCE_GPI_GPI0_ZERO                                           0x00000000U

//*****************************************************************************
//
// Register: VCE_O_TRCCTL
//
//*****************************************************************************
// Field:     [0] SEND
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_TRCCTL_SEND                                             0x00000001U
#define VCE_TRCCTL_SEND_M                                           0x00000001U
#define VCE_TRCCTL_SEND_S                                                    0U
#define VCE_TRCCTL_SEND_ONE                                         0x00000001U
#define VCE_TRCCTL_SEND_ZERO                                        0x00000000U

//*****************************************************************************
//
// Register: VCE_O_TRCSTAT
//
//*****************************************************************************
// Field:     [0] BUSY
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_TRCSTAT_BUSY                                            0x00000001U
#define VCE_TRCSTAT_BUSY_M                                          0x00000001U
#define VCE_TRCSTAT_BUSY_S                                                   0U
#define VCE_TRCSTAT_BUSY_ONE                                        0x00000001U
#define VCE_TRCSTAT_BUSY_ZERO                                       0x00000000U

//*****************************************************************************
//
// Register: VCE_O_TRCCMD
//
//*****************************************************************************
// Field:   [9:8] PARCNT
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_TRCCMD_PARCNT_W                                                  2U
#define VCE_TRCCMD_PARCNT_M                                         0x00000300U
#define VCE_TRCCMD_PARCNT_S                                                  8U
#define VCE_TRCCMD_PARCNT_ALLONES                                   0x00000300U
#define VCE_TRCCMD_PARCNT_ALLZEROS                                  0x00000000U

// Field:   [7:0] PKTHDR
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_TRCCMD_PKTHDR_W                                                  8U
#define VCE_TRCCMD_PKTHDR_M                                         0x000000FFU
#define VCE_TRCCMD_PKTHDR_S                                                  0U
#define VCE_TRCCMD_PKTHDR_ALLONES                                   0x000000FFU
#define VCE_TRCCMD_PKTHDR_ALLZEROS                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_TRCPAR0
//
//*****************************************************************************
// Field:  [15:0] VAL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_TRCPAR0_VAL_W                                                   16U
#define VCE_TRCPAR0_VAL_M                                           0x0000FFFFU
#define VCE_TRCPAR0_VAL_S                                                    0U
#define VCE_TRCPAR0_VAL_ALLONES                                     0x0000FFFFU
#define VCE_TRCPAR0_VAL_ALLZEROS                                    0x00000000U

//*****************************************************************************
//
// Register: VCE_O_TRCPAR1
//
//*****************************************************************************
// Field:  [15:0] VAL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_TRCPAR1_VAL_W                                                   16U
#define VCE_TRCPAR1_VAL_M                                           0x0000FFFFU
#define VCE_TRCPAR1_VAL_S                                                    0U
#define VCE_TRCPAR1_VAL_ALLONES                                     0x0000FFFFU
#define VCE_TRCPAR1_VAL_ALLZEROS                                    0x00000000U

//*****************************************************************************
//
// Register: VCE_O_TIMCTL
//
//*****************************************************************************
// Field:  [13:8] CPTSRC
//
// Internal. Only to be used through TI provided API.
#define VCE_TIMCTL_CPTSRC_W                                                  6U
#define VCE_TIMCTL_CPTSRC_M                                         0x00003F00U
#define VCE_TIMCTL_CPTSRC_S                                                  8U

// Field:     [7] CPTCTL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ENABLE                   Internal. Only to be used through TI provided API.
// DISABLE                  Internal. Only to be used through TI provided API.
#define VCE_TIMCTL_CPTCTL                                           0x00000080U
#define VCE_TIMCTL_CPTCTL_M                                         0x00000080U
#define VCE_TIMCTL_CPTCTL_S                                                  7U
#define VCE_TIMCTL_CPTCTL_ENABLE                                    0x00000080U
#define VCE_TIMCTL_CPTCTL_DISABLE                                   0x00000000U

// Field:   [6:5] CNTRSRC
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// CLK                      Internal. Only to be used through TI provided API.
#define VCE_TIMCTL_CNTRSRC_W                                                 2U
#define VCE_TIMCTL_CNTRSRC_M                                        0x00000060U
#define VCE_TIMCTL_CNTRSRC_S                                                 5U
#define VCE_TIMCTL_CNTRSRC_CLK                                      0x00000000U

// Field:     [4] CNTRCLR
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_TIMCTL_CNTRCLR                                          0x00000010U
#define VCE_TIMCTL_CNTRCLR_M                                        0x00000010U
#define VCE_TIMCTL_CNTRCLR_S                                                 4U
#define VCE_TIMCTL_CNTRCLR_ONE                                      0x00000010U
#define VCE_TIMCTL_CNTRCLR_ZERO                                     0x00000000U

// Field:     [3] CNTRCTL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_TIMCTL_CNTRCTL                                          0x00000008U
#define VCE_TIMCTL_CNTRCTL_M                                        0x00000008U
#define VCE_TIMCTL_CNTRCTL_S                                                 3U
#define VCE_TIMCTL_CNTRCTL_ONE                                      0x00000008U
#define VCE_TIMCTL_CNTRCTL_ZERO                                     0x00000000U

// Field:   [2:1] TIMSRC
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// CLK                      Internal. Only to be used through TI provided API.
#define VCE_TIMCTL_TIMSRC_W                                                  2U
#define VCE_TIMCTL_TIMSRC_M                                         0x00000006U
#define VCE_TIMCTL_TIMSRC_S                                                  1U
#define VCE_TIMCTL_TIMSRC_CLK                                       0x00000000U

// Field:     [0] TIMCTL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ENABLE                   Internal. Only to be used through TI provided API.
// DISABLE                  Internal. Only to be used through TI provided API.
#define VCE_TIMCTL_TIMCTL                                           0x00000001U
#define VCE_TIMCTL_TIMCTL_M                                         0x00000001U
#define VCE_TIMCTL_TIMCTL_S                                                  0U
#define VCE_TIMCTL_TIMCTL_ENABLE                                    0x00000001U
#define VCE_TIMCTL_TIMCTL_DISABLE                                   0x00000000U

//*****************************************************************************
//
// Register: VCE_O_TIMINC
//
//*****************************************************************************
// Field:  [15:0] VAL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_TIMINC_VAL_W                                                    16U
#define VCE_TIMINC_VAL_M                                            0x0000FFFFU
#define VCE_TIMINC_VAL_S                                                     0U
#define VCE_TIMINC_VAL_ALLONES                                      0x0000FFFFU
#define VCE_TIMINC_VAL_ALLZEROS                                     0x00000000U

//*****************************************************************************
//
// Register: VCE_O_TIMPER
//
//*****************************************************************************
// Field:  [15:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_TIMPER_VAL_W                                                    16U
#define VCE_TIMPER_VAL_M                                            0x0000FFFFU
#define VCE_TIMPER_VAL_S                                                     0U

//*****************************************************************************
//
// Register: VCE_O_TIMCNT
//
//*****************************************************************************
// Field:  [15:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_TIMCNT_VAL_W                                                    16U
#define VCE_TIMCNT_VAL_M                                            0x0000FFFFU
#define VCE_TIMCNT_VAL_S                                                     0U

//*****************************************************************************
//
// Register: VCE_O_TIMCAPT
//
//*****************************************************************************
// Field:  [15:0] VALUE
//
// Internal. Only to be used through TI provided API.
#define VCE_TIMCAPT_VALUE_W                                                 16U
#define VCE_TIMCAPT_VALUE_M                                         0x0000FFFFU
#define VCE_TIMCAPT_VALUE_S                                                  0U

//*****************************************************************************
//
// Register: VCE_O_LSECTL
//
//*****************************************************************************
// Field:     [2] BITREV
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// EN                       Internal. Only to be used through TI provided API.
// DIS                      Internal. Only to be used through TI provided API.
#define VCE_LSECTL_BITREV                                           0x00000004U
#define VCE_LSECTL_BITREV_M                                         0x00000004U
#define VCE_LSECTL_BITREV_S                                                  2U
#define VCE_LSECTL_BITREV_EN                                        0x00000004U
#define VCE_LSECTL_BITREV_DIS                                       0x00000000U

// Field:     [1] SCHEDULING
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// HPIPELINE                Internal. Only to be used through TI provided API.
// PIPELINE                 Internal. Only to be used through TI provided API.
#define VCE_LSECTL_SCHEDULING                                       0x00000002U
#define VCE_LSECTL_SCHEDULING_M                                     0x00000002U
#define VCE_LSECTL_SCHEDULING_S                                              1U
#define VCE_LSECTL_SCHEDULING_HPIPELINE                             0x00000002U
#define VCE_LSECTL_SCHEDULING_PIPELINE                              0x00000000U

// Field:     [0] MEMORY
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// INTERLEAVED              Internal. Only to be used through TI provided API.
// MIRRORED                 Internal. Only to be used through TI provided API.
#define VCE_LSECTL_MEMORY                                           0x00000001U
#define VCE_LSECTL_MEMORY_M                                         0x00000001U
#define VCE_LSECTL_MEMORY_S                                                  0U
#define VCE_LSECTL_MEMORY_INTERLEAVED                               0x00000001U
#define VCE_LSECTL_MEMORY_MIRRORED                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_LSESTART
//
//*****************************************************************************
// Field:     [1] INITDST
//
// Internal. Only to be used through TI provided API.
#define VCE_LSESTART_INITDST                                        0x00000002U
#define VCE_LSESTART_INITDST_M                                      0x00000002U
#define VCE_LSESTART_INITDST_S                                               1U

// Field:     [0] START
//
// Internal. Only to be used through TI provided API.
#define VCE_LSESTART_START                                          0x00000001U
#define VCE_LSESTART_START_M                                        0x00000001U
#define VCE_LSESTART_START_S                                                 0U

//*****************************************************************************
//
// Register: VCE_O_LSEBASESRCA
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSEBASESRCA_VAL_W                                               11U
#define VCE_LSEBASESRCA_VAL_M                                       0x000007FFU
#define VCE_LSEBASESRCA_VAL_S                                                0U

//*****************************************************************************
//
// Register: VCE_O_LSEMODESRCA
//
//*****************************************************************************
// Field:   [3:0] VAL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// UNITCIRC                 Internal. Only to be used through TI provided API.
// CONJVEC                  Internal. Only to be used through TI provided API.
// PUSHC                    Internal. Only to be used through TI provided API.
// LOADK                    Internal. Only to be used through TI provided API.
// IFFT                     Internal. Only to be used through TI provided API.
// FFT                      Internal. Only to be used through TI provided API.
// DIAG                     Internal. Only to be used through TI provided API.
// SYM                      Internal. Only to be used through TI provided API.
// HERM                     Internal. Only to be used through TI provided API.
// MATRIX                   Internal. Only to be used through TI provided API.
// VECTOR                   Internal. Only to be used through TI provided API.
#define VCE_LSEMODESRCA_VAL_W                                                4U
#define VCE_LSEMODESRCA_VAL_M                                       0x0000000FU
#define VCE_LSEMODESRCA_VAL_S                                                0U
#define VCE_LSEMODESRCA_VAL_UNITCIRC                                0x0000000AU
#define VCE_LSEMODESRCA_VAL_CONJVEC                                 0x00000009U
#define VCE_LSEMODESRCA_VAL_PUSHC                                   0x00000008U
#define VCE_LSEMODESRCA_VAL_LOADK                                   0x00000007U
#define VCE_LSEMODESRCA_VAL_IFFT                                    0x00000006U
#define VCE_LSEMODESRCA_VAL_FFT                                     0x00000005U
#define VCE_LSEMODESRCA_VAL_DIAG                                    0x00000004U
#define VCE_LSEMODESRCA_VAL_SYM                                     0x00000003U
#define VCE_LSEMODESRCA_VAL_HERM                                    0x00000002U
#define VCE_LSEMODESRCA_VAL_MATRIX                                  0x00000001U
#define VCE_LSEMODESRCA_VAL_VECTOR                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_LSESUBMODESRCA
//
//*****************************************************************************
// Field:     [0] VAL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// COL                      Internal. Only to be used through TI provided API.
// ROW                      Internal. Only to be used through TI provided API.
#define VCE_LSESUBMODESRCA_VAL                                      0x00000001U
#define VCE_LSESUBMODESRCA_VAL_M                                    0x00000001U
#define VCE_LSESUBMODESRCA_VAL_S                                             0U
#define VCE_LSESUBMODESRCA_VAL_COL                                  0x00000001U
#define VCE_LSESUBMODESRCA_VAL_ROW                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_LSENSRCA
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSENSRCA_VAL_W                                                  11U
#define VCE_LSENSRCA_VAL_M                                          0x000007FFU
#define VCE_LSENSRCA_VAL_S                                                   0U

//*****************************************************************************
//
// Register: VCE_O_LSEMSRCA
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSEMSRCA_VAL_W                                                  11U
#define VCE_LSEMSRCA_VAL_M                                          0x000007FFU
#define VCE_LSEMSRCA_VAL_S                                                   0U

//*****************************************************************************
//
// Register: VCE_O_LSEELEMENTSRCA
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSEELEMENTSRCA_VAL_W                                            11U
#define VCE_LSEELEMENTSRCA_VAL_M                                    0x000007FFU
#define VCE_LSEELEMENTSRCA_VAL_S                                             0U

//*****************************************************************************
//
// Register: VCE_O_LSESTRIDESRCA
//
//*****************************************************************************
// Field:   [1:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSESTRIDESRCA_VAL_W                                              2U
#define VCE_LSESTRIDESRCA_VAL_M                                     0x00000003U
#define VCE_LSESTRIDESRCA_VAL_S                                              0U

//*****************************************************************************
//
// Register: VCE_O_LSEBASEDSTA
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSEBASEDSTA_VAL_W                                               11U
#define VCE_LSEBASEDSTA_VAL_M                                       0x000007FFU
#define VCE_LSEBASEDSTA_VAL_S                                                0U

//*****************************************************************************
//
// Register: VCE_O_LSEMODEDSTA
//
//*****************************************************************************
// Field:   [3:0] VAL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// CONJVEC                  Internal. Only to be used through TI provided API.
// PUSHC                    Internal. Only to be used through TI provided API.
// LOADK                    Internal. Only to be used through TI provided API.
// IFFT                     Internal. Only to be used through TI provided API.
// FFT                      Internal. Only to be used through TI provided API.
// DIAG                     Internal. Only to be used through TI provided API.
// SYM                      Internal. Only to be used through TI provided API.
// HERM                     Internal. Only to be used through TI provided API.
// MATRIX                   Internal. Only to be used through TI provided API.
// VECTOR                   Internal. Only to be used through TI provided API.
#define VCE_LSEMODEDSTA_VAL_W                                                4U
#define VCE_LSEMODEDSTA_VAL_M                                       0x0000000FU
#define VCE_LSEMODEDSTA_VAL_S                                                0U
#define VCE_LSEMODEDSTA_VAL_CONJVEC                                 0x00000009U
#define VCE_LSEMODEDSTA_VAL_PUSHC                                   0x00000008U
#define VCE_LSEMODEDSTA_VAL_LOADK                                   0x00000007U
#define VCE_LSEMODEDSTA_VAL_IFFT                                    0x00000006U
#define VCE_LSEMODEDSTA_VAL_FFT                                     0x00000005U
#define VCE_LSEMODEDSTA_VAL_DIAG                                    0x00000004U
#define VCE_LSEMODEDSTA_VAL_SYM                                     0x00000003U
#define VCE_LSEMODEDSTA_VAL_HERM                                    0x00000002U
#define VCE_LSEMODEDSTA_VAL_MATRIX                                  0x00000001U
#define VCE_LSEMODEDSTA_VAL_VECTOR                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_LSESUBMODEDSTA
//
//*****************************************************************************
// Field:     [0] VAL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// COL                      Internal. Only to be used through TI provided API.
// ROW                      Internal. Only to be used through TI provided API.
#define VCE_LSESUBMODEDSTA_VAL                                      0x00000001U
#define VCE_LSESUBMODEDSTA_VAL_M                                    0x00000001U
#define VCE_LSESUBMODEDSTA_VAL_S                                             0U
#define VCE_LSESUBMODEDSTA_VAL_COL                                  0x00000001U
#define VCE_LSESUBMODEDSTA_VAL_ROW                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_LSENDSTA
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSENDSTA_VAL_W                                                  11U
#define VCE_LSENDSTA_VAL_M                                          0x000007FFU
#define VCE_LSENDSTA_VAL_S                                                   0U

//*****************************************************************************
//
// Register: VCE_O_LSEMDSTA
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSEMDSTA_VAL_W                                                  11U
#define VCE_LSEMDSTA_VAL_M                                          0x000007FFU
#define VCE_LSEMDSTA_VAL_S                                                   0U

//*****************************************************************************
//
// Register: VCE_O_LSEELEMENTDSTA
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSEELEMENTDSTA_VAL_W                                            11U
#define VCE_LSEELEMENTDSTA_VAL_M                                    0x000007FFU
#define VCE_LSEELEMENTDSTA_VAL_S                                             0U

//*****************************************************************************
//
// Register: VCE_O_LSESTRIDEDSTA
//
//*****************************************************************************
// Field:   [1:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSESTRIDEDSTA_VAL_W                                              2U
#define VCE_LSESTRIDEDSTA_VAL_M                                     0x00000003U
#define VCE_LSESTRIDEDSTA_VAL_S                                              0U

//*****************************************************************************
//
// Register: VCE_O_LSEBASESRCB
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSEBASESRCB_VAL_W                                               11U
#define VCE_LSEBASESRCB_VAL_M                                       0x000007FFU
#define VCE_LSEBASESRCB_VAL_S                                                0U

//*****************************************************************************
//
// Register: VCE_O_LSEMODESRCB
//
//*****************************************************************************
// Field:   [3:0] VAL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// CONJVEC                  Internal. Only to be used through TI provided API.
// PUSHC                    Internal. Only to be used through TI provided API.
// LOADK                    Internal. Only to be used through TI provided API.
// IFFT                     Internal. Only to be used through TI provided API.
// FFT                      Internal. Only to be used through TI provided API.
// DIAG                     Internal. Only to be used through TI provided API.
// SYM                      Internal. Only to be used through TI provided API.
// HERM                     Internal. Only to be used through TI provided API.
// MATRIX                   Internal. Only to be used through TI provided API.
// VECTOR                   Internal. Only to be used through TI provided API.
#define VCE_LSEMODESRCB_VAL_W                                                4U
#define VCE_LSEMODESRCB_VAL_M                                       0x0000000FU
#define VCE_LSEMODESRCB_VAL_S                                                0U
#define VCE_LSEMODESRCB_VAL_CONJVEC                                 0x00000009U
#define VCE_LSEMODESRCB_VAL_PUSHC                                   0x00000008U
#define VCE_LSEMODESRCB_VAL_LOADK                                   0x00000007U
#define VCE_LSEMODESRCB_VAL_IFFT                                    0x00000006U
#define VCE_LSEMODESRCB_VAL_FFT                                     0x00000005U
#define VCE_LSEMODESRCB_VAL_DIAG                                    0x00000004U
#define VCE_LSEMODESRCB_VAL_SYM                                     0x00000003U
#define VCE_LSEMODESRCB_VAL_HERM                                    0x00000002U
#define VCE_LSEMODESRCB_VAL_MATRIX                                  0x00000001U
#define VCE_LSEMODESRCB_VAL_VECTOR                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_LSESUBMODESRCB
//
//*****************************************************************************
// Field:     [0] VAL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// COL                      Internal. Only to be used through TI provided API.
// ROW                      Internal. Only to be used through TI provided API.
#define VCE_LSESUBMODESRCB_VAL                                      0x00000001U
#define VCE_LSESUBMODESRCB_VAL_M                                    0x00000001U
#define VCE_LSESUBMODESRCB_VAL_S                                             0U
#define VCE_LSESUBMODESRCB_VAL_COL                                  0x00000001U
#define VCE_LSESUBMODESRCB_VAL_ROW                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_LSENSRCB
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSENSRCB_VAL_W                                                  11U
#define VCE_LSENSRCB_VAL_M                                          0x000007FFU
#define VCE_LSENSRCB_VAL_S                                                   0U

//*****************************************************************************
//
// Register: VCE_O_LSEMSRCB
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSEMSRCB_VAL_W                                                  11U
#define VCE_LSEMSRCB_VAL_M                                          0x000007FFU
#define VCE_LSEMSRCB_VAL_S                                                   0U

//*****************************************************************************
//
// Register: VCE_O_LSEELEMENTSRCB
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSEELEMENTSRCB_VAL_W                                            11U
#define VCE_LSEELEMENTSRCB_VAL_M                                    0x000007FFU
#define VCE_LSEELEMENTSRCB_VAL_S                                             0U

//*****************************************************************************
//
// Register: VCE_O_LSESTRIDESRCB
//
//*****************************************************************************
// Field:   [1:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSESTRIDESRCB_VAL_W                                              2U
#define VCE_LSESTRIDESRCB_VAL_M                                     0x00000003U
#define VCE_LSESTRIDESRCB_VAL_S                                              0U

//*****************************************************************************
//
// Register: VCE_O_LSEBASEDSTB
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSEBASEDSTB_VAL_W                                               11U
#define VCE_LSEBASEDSTB_VAL_M                                       0x000007FFU
#define VCE_LSEBASEDSTB_VAL_S                                                0U

//*****************************************************************************
//
// Register: VCE_O_LSEMODEDSTB
//
//*****************************************************************************
// Field:   [3:0] VAL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// CONJVEC                  Internal. Only to be used through TI provided API.
// PUSHC                    Internal. Only to be used through TI provided API.
// LOADK                    Internal. Only to be used through TI provided API.
// IFFT                     Internal. Only to be used through TI provided API.
// FFT                      Internal. Only to be used through TI provided API.
// DIAG                     Internal. Only to be used through TI provided API.
// SYM                      Internal. Only to be used through TI provided API.
// HERM                     Internal. Only to be used through TI provided API.
// MATRIX                   Internal. Only to be used through TI provided API.
// VECTOR                   Internal. Only to be used through TI provided API.
#define VCE_LSEMODEDSTB_VAL_W                                                4U
#define VCE_LSEMODEDSTB_VAL_M                                       0x0000000FU
#define VCE_LSEMODEDSTB_VAL_S                                                0U
#define VCE_LSEMODEDSTB_VAL_CONJVEC                                 0x00000009U
#define VCE_LSEMODEDSTB_VAL_PUSHC                                   0x00000008U
#define VCE_LSEMODEDSTB_VAL_LOADK                                   0x00000007U
#define VCE_LSEMODEDSTB_VAL_IFFT                                    0x00000006U
#define VCE_LSEMODEDSTB_VAL_FFT                                     0x00000005U
#define VCE_LSEMODEDSTB_VAL_DIAG                                    0x00000004U
#define VCE_LSEMODEDSTB_VAL_SYM                                     0x00000003U
#define VCE_LSEMODEDSTB_VAL_HERM                                    0x00000002U
#define VCE_LSEMODEDSTB_VAL_MATRIX                                  0x00000001U
#define VCE_LSEMODEDSTB_VAL_VECTOR                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_LSESUBMODEDSTB
//
//*****************************************************************************
// Field:     [0] VAL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// COL                      Internal. Only to be used through TI provided API.
// ROW                      Internal. Only to be used through TI provided API.
#define VCE_LSESUBMODEDSTB_VAL                                      0x00000001U
#define VCE_LSESUBMODEDSTB_VAL_M                                    0x00000001U
#define VCE_LSESUBMODEDSTB_VAL_S                                             0U
#define VCE_LSESUBMODEDSTB_VAL_COL                                  0x00000001U
#define VCE_LSESUBMODEDSTB_VAL_ROW                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_LSENDSTB
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSENDSTB_VAL_W                                                  11U
#define VCE_LSENDSTB_VAL_M                                          0x000007FFU
#define VCE_LSENDSTB_VAL_S                                                   0U

//*****************************************************************************
//
// Register: VCE_O_LSEMDSTB
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSEMDSTB_VAL_W                                                  11U
#define VCE_LSEMDSTB_VAL_M                                          0x000007FFU
#define VCE_LSEMDSTB_VAL_S                                                   0U

//*****************************************************************************
//
// Register: VCE_O_LSEELEMENTDSTB
//
//*****************************************************************************
// Field:  [10:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSEELEMENTDSTB_VAL_W                                            11U
#define VCE_LSEELEMENTDSTB_VAL_M                                    0x000007FFU
#define VCE_LSEELEMENTDSTB_VAL_S                                             0U

//*****************************************************************************
//
// Register: VCE_O_LSESTRIDEDSTB
//
//*****************************************************************************
// Field:   [1:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_LSESTRIDEDSTB_VAL_W                                              2U
#define VCE_LSESTRIDEDSTB_VAL_M                                     0x00000003U
#define VCE_LSESTRIDEDSTB_VAL_S                                              0U

//*****************************************************************************
//
// Register: VCE_O_XBAR0
//
//*****************************************************************************
// Field: [15:14] OUTSRCSELB
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// FMUL1                    Internal. Only to be used through TI provided API.
// FMUL0                    Internal. Only to be used through TI provided API.
// R2C                      Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
#define VCE_XBAR0_OUTSRCSELB_W                                               2U
#define VCE_XBAR0_OUTSRCSELB_M                                      0x0000C000U
#define VCE_XBAR0_OUTSRCSELB_S                                              14U
#define VCE_XBAR0_OUTSRCSELB_FMUL1                                  0x0000C000U
#define VCE_XBAR0_OUTSRCSELB_FMUL0                                  0x00008000U
#define VCE_XBAR0_OUTSRCSELB_R2C                                    0x00004000U
#define VCE_XBAR0_OUTSRCSELB_INPUT                                  0x00000000U

// Field: [13:12] OUTSRCSELA
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// FMUL1                    Internal. Only to be used through TI provided API.
// FMUL0                    Internal. Only to be used through TI provided API.
// R2C                      Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
#define VCE_XBAR0_OUTSRCSELA_W                                               2U
#define VCE_XBAR0_OUTSRCSELA_M                                      0x00003000U
#define VCE_XBAR0_OUTSRCSELA_S                                              12U
#define VCE_XBAR0_OUTSRCSELA_FMUL1                                  0x00003000U
#define VCE_XBAR0_OUTSRCSELA_FMUL0                                  0x00002000U
#define VCE_XBAR0_OUTSRCSELA_R2C                                    0x00001000U
#define VCE_XBAR0_OUTSRCSELA_INPUT                                  0x00000000U

// Field: [11:10] B3SRCSELB
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// FMUL0                    Internal. Only to be used through TI provided API.
// R2C                      Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_XBAR0_B3SRCSELB_W                                                2U
#define VCE_XBAR0_B3SRCSELB_M                                       0x00000C00U
#define VCE_XBAR0_B3SRCSELB_S                                               10U
#define VCE_XBAR0_B3SRCSELB_FMUL0                                   0x00000C00U
#define VCE_XBAR0_B3SRCSELB_R2C                                     0x00000800U
#define VCE_XBAR0_B3SRCSELB_INPUT                                   0x00000400U
#define VCE_XBAR0_B3SRCSELB_ZERO                                    0x00000000U

// Field:   [9:8] B3SRCSELA
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// FMUL0                    Internal. Only to be used through TI provided API.
// R2C                      Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_XBAR0_B3SRCSELA_W                                                2U
#define VCE_XBAR0_B3SRCSELA_M                                       0x00000300U
#define VCE_XBAR0_B3SRCSELA_S                                                8U
#define VCE_XBAR0_B3SRCSELA_FMUL0                                   0x00000300U
#define VCE_XBAR0_B3SRCSELA_R2C                                     0x00000200U
#define VCE_XBAR0_B3SRCSELA_INPUT                                   0x00000100U
#define VCE_XBAR0_B3SRCSELA_ZERO                                    0x00000000U

// Field:   [7:6] B2SRCSELB
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// FMUL1                    Internal. Only to be used through TI provided API.
// R2C                      Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_XBAR0_B2SRCSELB_W                                                2U
#define VCE_XBAR0_B2SRCSELB_M                                       0x000000C0U
#define VCE_XBAR0_B2SRCSELB_S                                                6U
#define VCE_XBAR0_B2SRCSELB_FMUL1                                   0x000000C0U
#define VCE_XBAR0_B2SRCSELB_R2C                                     0x00000080U
#define VCE_XBAR0_B2SRCSELB_INPUT                                   0x00000040U
#define VCE_XBAR0_B2SRCSELB_ZERO                                    0x00000000U

// Field:   [5:4] B2SRCSELA
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// FMUL1                    Internal. Only to be used through TI provided API.
// R2C                      Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_XBAR0_B2SRCSELA_W                                                2U
#define VCE_XBAR0_B2SRCSELA_M                                       0x00000030U
#define VCE_XBAR0_B2SRCSELA_S                                                4U
#define VCE_XBAR0_B2SRCSELA_FMUL1                                   0x00000030U
#define VCE_XBAR0_B2SRCSELA_R2C                                     0x00000020U
#define VCE_XBAR0_B2SRCSELA_INPUT                                   0x00000010U
#define VCE_XBAR0_B2SRCSELA_ZERO                                    0x00000000U

// Field:   [3:2] B1SRCSELB
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// FMUL1                    Internal. Only to be used through TI provided API.
// FMUL0                    Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_XBAR0_B1SRCSELB_W                                                2U
#define VCE_XBAR0_B1SRCSELB_M                                       0x0000000CU
#define VCE_XBAR0_B1SRCSELB_S                                                2U
#define VCE_XBAR0_B1SRCSELB_FMUL1                                   0x0000000CU
#define VCE_XBAR0_B1SRCSELB_FMUL0                                   0x00000008U
#define VCE_XBAR0_B1SRCSELB_INPUT                                   0x00000004U
#define VCE_XBAR0_B1SRCSELB_ZERO                                    0x00000000U

// Field:   [1:0] B1SRCSELA
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// FMUL1                    Internal. Only to be used through TI provided API.
// FMUL0                    Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_XBAR0_B1SRCSELA_W                                                2U
#define VCE_XBAR0_B1SRCSELA_M                                       0x00000003U
#define VCE_XBAR0_B1SRCSELA_S                                                0U
#define VCE_XBAR0_B1SRCSELA_FMUL1                                   0x00000003U
#define VCE_XBAR0_B1SRCSELA_FMUL0                                   0x00000002U
#define VCE_XBAR0_B1SRCSELA_INPUT                                   0x00000001U
#define VCE_XBAR0_B1SRCSELA_ZERO                                    0x00000000U

//*****************************************************************************
//
// Register: VCE_O_XBAR1
//
//*****************************************************************************
// Field: [15:14] OUTSRCSELB
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ADDSUB                   Internal. Only to be used through TI provided API.
// UCRD                     Internal. Only to be used through TI provided API.
// SINCOS                   Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
#define VCE_XBAR1_OUTSRCSELB_W                                               2U
#define VCE_XBAR1_OUTSRCSELB_M                                      0x0000C000U
#define VCE_XBAR1_OUTSRCSELB_S                                              14U
#define VCE_XBAR1_OUTSRCSELB_ADDSUB                                 0x0000C000U
#define VCE_XBAR1_OUTSRCSELB_UCRD                                   0x00008000U
#define VCE_XBAR1_OUTSRCSELB_SINCOS                                 0x00004000U
#define VCE_XBAR1_OUTSRCSELB_INPUT                                  0x00000000U

// Field: [13:12] OUTSRCSELA
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ADDSUB                   Internal. Only to be used through TI provided API.
// UCRD                     Internal. Only to be used through TI provided API.
// SINCOS                   Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
#define VCE_XBAR1_OUTSRCSELA_W                                               2U
#define VCE_XBAR1_OUTSRCSELA_M                                      0x00003000U
#define VCE_XBAR1_OUTSRCSELA_S                                              12U
#define VCE_XBAR1_OUTSRCSELA_ADDSUB                                 0x00003000U
#define VCE_XBAR1_OUTSRCSELA_UCRD                                   0x00002000U
#define VCE_XBAR1_OUTSRCSELA_SINCOS                                 0x00001000U
#define VCE_XBAR1_OUTSRCSELA_INPUT                                  0x00000000U

// Field: [11:10] B3SRCSELB
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// UCRD                     Internal. Only to be used through TI provided API.
// SINCOS                   Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_XBAR1_B3SRCSELB_W                                                2U
#define VCE_XBAR1_B3SRCSELB_M                                       0x00000C00U
#define VCE_XBAR1_B3SRCSELB_S                                               10U
#define VCE_XBAR1_B3SRCSELB_UCRD                                    0x00000C00U
#define VCE_XBAR1_B3SRCSELB_SINCOS                                  0x00000800U
#define VCE_XBAR1_B3SRCSELB_INPUT                                   0x00000400U
#define VCE_XBAR1_B3SRCSELB_ZERO                                    0x00000000U

// Field:   [9:8] B3SRCSELA
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// UCRD                     Internal. Only to be used through TI provided API.
// SINCOS                   Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_XBAR1_B3SRCSELA_W                                                2U
#define VCE_XBAR1_B3SRCSELA_M                                       0x00000300U
#define VCE_XBAR1_B3SRCSELA_S                                                8U
#define VCE_XBAR1_B3SRCSELA_UCRD                                    0x00000300U
#define VCE_XBAR1_B3SRCSELA_SINCOS                                  0x00000200U
#define VCE_XBAR1_B3SRCSELA_INPUT                                   0x00000100U
#define VCE_XBAR1_B3SRCSELA_ZERO                                    0x00000000U

// Field:   [7:6] B2SRCSELB
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ADDSUB                   Internal. Only to be used through TI provided API.
// SINCOS                   Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_XBAR1_B2SRCSELB_W                                                2U
#define VCE_XBAR1_B2SRCSELB_M                                       0x000000C0U
#define VCE_XBAR1_B2SRCSELB_S                                                6U
#define VCE_XBAR1_B2SRCSELB_ADDSUB                                  0x000000C0U
#define VCE_XBAR1_B2SRCSELB_SINCOS                                  0x00000080U
#define VCE_XBAR1_B2SRCSELB_INPUT                                   0x00000040U
#define VCE_XBAR1_B2SRCSELB_ZERO                                    0x00000000U

// Field:   [5:4] B2SRCSELA
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ADDSUB                   Internal. Only to be used through TI provided API.
// SINCOS                   Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_XBAR1_B2SRCSELA_W                                                2U
#define VCE_XBAR1_B2SRCSELA_M                                       0x00000030U
#define VCE_XBAR1_B2SRCSELA_S                                                4U
#define VCE_XBAR1_B2SRCSELA_ADDSUB                                  0x00000030U
#define VCE_XBAR1_B2SRCSELA_SINCOS                                  0x00000020U
#define VCE_XBAR1_B2SRCSELA_INPUT                                   0x00000010U
#define VCE_XBAR1_B2SRCSELA_ZERO                                    0x00000000U

// Field:   [3:2] B1SRCSELB
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ADDSUB                   Internal. Only to be used through TI provided API.
// UCRD                     Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_XBAR1_B1SRCSELB_W                                                2U
#define VCE_XBAR1_B1SRCSELB_M                                       0x0000000CU
#define VCE_XBAR1_B1SRCSELB_S                                                2U
#define VCE_XBAR1_B1SRCSELB_ADDSUB                                  0x0000000CU
#define VCE_XBAR1_B1SRCSELB_UCRD                                    0x00000008U
#define VCE_XBAR1_B1SRCSELB_INPUT                                   0x00000004U
#define VCE_XBAR1_B1SRCSELB_ZERO                                    0x00000000U

// Field:   [1:0] B1SRCSELA
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ADDSUB                   Internal. Only to be used through TI provided API.
// UCRD                     Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_XBAR1_B1SRCSELA_W                                                2U
#define VCE_XBAR1_B1SRCSELA_M                                       0x00000003U
#define VCE_XBAR1_B1SRCSELA_S                                                0U
#define VCE_XBAR1_B1SRCSELA_ADDSUB                                  0x00000003U
#define VCE_XBAR1_B1SRCSELA_UCRD                                    0x00000002U
#define VCE_XBAR1_B1SRCSELA_INPUT                                   0x00000001U
#define VCE_XBAR1_B1SRCSELA_ZERO                                    0x00000000U

//*****************************************************************************
//
// Register: VCE_O_XBAR2
//
//*****************************************************************************
// Field: [15:14] OUTSRCSELB
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// FDIV                     Internal. Only to be used through TI provided API.
// FX2FP                    Internal. Only to be used through TI provided API.
// MAXMIN                   Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
#define VCE_XBAR2_OUTSRCSELB_W                                               2U
#define VCE_XBAR2_OUTSRCSELB_M                                      0x0000C000U
#define VCE_XBAR2_OUTSRCSELB_S                                              14U
#define VCE_XBAR2_OUTSRCSELB_FDIV                                   0x0000C000U
#define VCE_XBAR2_OUTSRCSELB_FX2FP                                  0x00008000U
#define VCE_XBAR2_OUTSRCSELB_MAXMIN                                 0x00004000U
#define VCE_XBAR2_OUTSRCSELB_INPUT                                  0x00000000U

// Field: [13:12] OUTSRCSELA
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// FDIV                     Internal. Only to be used through TI provided API.
// FX2FP                    Internal. Only to be used through TI provided API.
// MAXMIN                   Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
#define VCE_XBAR2_OUTSRCSELA_W                                               2U
#define VCE_XBAR2_OUTSRCSELA_M                                      0x00003000U
#define VCE_XBAR2_OUTSRCSELA_S                                              12U
#define VCE_XBAR2_OUTSRCSELA_FDIV                                   0x00003000U
#define VCE_XBAR2_OUTSRCSELA_FX2FP                                  0x00002000U
#define VCE_XBAR2_OUTSRCSELA_MAXMIN                                 0x00001000U
#define VCE_XBAR2_OUTSRCSELA_INPUT                                  0x00000000U

// Field: [11:10] B3SRCSELB
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// FX2FP                    Internal. Only to be used through TI provided API.
// MAXMIN                   Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_XBAR2_B3SRCSELB_W                                                2U
#define VCE_XBAR2_B3SRCSELB_M                                       0x00000C00U
#define VCE_XBAR2_B3SRCSELB_S                                               10U
#define VCE_XBAR2_B3SRCSELB_FX2FP                                   0x00000C00U
#define VCE_XBAR2_B3SRCSELB_MAXMIN                                  0x00000800U
#define VCE_XBAR2_B3SRCSELB_INPUT                                   0x00000400U
#define VCE_XBAR2_B3SRCSELB_ZERO                                    0x00000000U

// Field:   [9:8] B3SRCSELA
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// FX2FP                    Internal. Only to be used through TI provided API.
// MAXMIN                   Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_XBAR2_B3SRCSELA_W                                                2U
#define VCE_XBAR2_B3SRCSELA_M                                       0x00000300U
#define VCE_XBAR2_B3SRCSELA_S                                                8U
#define VCE_XBAR2_B3SRCSELA_FX2FP                                   0x00000300U
#define VCE_XBAR2_B3SRCSELA_MAXMIN                                  0x00000200U
#define VCE_XBAR2_B3SRCSELA_INPUT                                   0x00000100U
#define VCE_XBAR2_B3SRCSELA_ZERO                                    0x00000000U

// Field:   [7:6] B2SRCSELB
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// FDIV                     Internal. Only to be used through TI provided API.
// MAXMIN                   Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_XBAR2_B2SRCSELB_W                                                2U
#define VCE_XBAR2_B2SRCSELB_M                                       0x000000C0U
#define VCE_XBAR2_B2SRCSELB_S                                                6U
#define VCE_XBAR2_B2SRCSELB_FDIV                                    0x000000C0U
#define VCE_XBAR2_B2SRCSELB_MAXMIN                                  0x00000080U
#define VCE_XBAR2_B2SRCSELB_INPUT                                   0x00000040U
#define VCE_XBAR2_B2SRCSELB_ZERO                                    0x00000000U

// Field:   [5:4] B2SRCSELA
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// FDIV                     Internal. Only to be used through TI provided API.
// MAXMIN                   Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_XBAR2_B2SRCSELA_W                                                2U
#define VCE_XBAR2_B2SRCSELA_M                                       0x00000030U
#define VCE_XBAR2_B2SRCSELA_S                                                4U
#define VCE_XBAR2_B2SRCSELA_FDIV                                    0x00000030U
#define VCE_XBAR2_B2SRCSELA_MAXMIN                                  0x00000020U
#define VCE_XBAR2_B2SRCSELA_INPUT                                   0x00000010U
#define VCE_XBAR2_B2SRCSELA_ZERO                                    0x00000000U

// Field:   [3:2] B1SRCSELB
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// FDIV                     Internal. Only to be used through TI provided API.
// FX2FP                    Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_XBAR2_B1SRCSELB_W                                                2U
#define VCE_XBAR2_B1SRCSELB_M                                       0x0000000CU
#define VCE_XBAR2_B1SRCSELB_S                                                2U
#define VCE_XBAR2_B1SRCSELB_FDIV                                    0x0000000CU
#define VCE_XBAR2_B1SRCSELB_FX2FP                                   0x00000008U
#define VCE_XBAR2_B1SRCSELB_INPUT                                   0x00000004U
#define VCE_XBAR2_B1SRCSELB_ZERO                                    0x00000000U

// Field:   [1:0] B1SRCSELA
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// FDIV                     Internal. Only to be used through TI provided API.
// FX2FP                    Internal. Only to be used through TI provided API.
// INPUT                    Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_XBAR2_B1SRCSELA_W                                                2U
#define VCE_XBAR2_B1SRCSELA_M                                       0x00000003U
#define VCE_XBAR2_B1SRCSELA_S                                                0U
#define VCE_XBAR2_B1SRCSELA_FDIV                                    0x00000003U
#define VCE_XBAR2_B1SRCSELA_FX2FP                                   0x00000002U
#define VCE_XBAR2_B1SRCSELA_INPUT                                   0x00000001U
#define VCE_XBAR2_B1SRCSELA_ZERO                                    0x00000000U

//*****************************************************************************
//
// Register: VCE_O_R2C
//
//*****************************************************************************
// Field:   [2:0] OP
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ABS                      Internal. Only to be used through TI provided API.
// IMA                      Internal. Only to be used through TI provided API.
// RA                       Internal. Only to be used through TI provided API.
// R2CCA                    Internal. Only to be used through TI provided API.
// R2CA                     Internal. Only to be used through TI provided API.
// R2CC                     Internal. Only to be used through TI provided API.
// R2C                      Internal. Only to be used through TI provided API.
#define VCE_R2C_OP_W                                                         3U
#define VCE_R2C_OP_M                                                0x00000007U
#define VCE_R2C_OP_S                                                         0U
#define VCE_R2C_OP_ABS                                              0x00000006U
#define VCE_R2C_OP_IMA                                              0x00000005U
#define VCE_R2C_OP_RA                                               0x00000004U
#define VCE_R2C_OP_R2CCA                                            0x00000003U
#define VCE_R2C_OP_R2CA                                             0x00000002U
#define VCE_R2C_OP_R2CC                                             0x00000001U
#define VCE_R2C_OP_R2C                                              0x00000000U

//*****************************************************************************
//
// Register: VCE_O_FMUL0
//
//*****************************************************************************
// Field:   [6:4] RNDADDSUB
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// AWAYZ                    Internal. Only to be used through TI provided API.
// NEARU                    Internal. Only to be used through TI provided API.
// NINF                     Internal. Only to be used through TI provided API.
// PINF                     Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
// NEARE                    Internal. Only to be used through TI provided API.
#define VCE_FMUL0_RNDADDSUB_W                                                3U
#define VCE_FMUL0_RNDADDSUB_M                                       0x00000070U
#define VCE_FMUL0_RNDADDSUB_S                                                4U
#define VCE_FMUL0_RNDADDSUB_AWAYZ                                   0x00000050U
#define VCE_FMUL0_RNDADDSUB_NEARU                                   0x00000040U
#define VCE_FMUL0_RNDADDSUB_NINF                                    0x00000030U
#define VCE_FMUL0_RNDADDSUB_PINF                                    0x00000020U
#define VCE_FMUL0_RNDADDSUB_ZERO                                    0x00000010U
#define VCE_FMUL0_RNDADDSUB_NEARE                                   0x00000000U

// Field:   [3:1] RNDMUL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// AWAYZ                    Internal. Only to be used through TI provided API.
// NEARU                    Internal. Only to be used through TI provided API.
// NINF                     Internal. Only to be used through TI provided API.
// PINF                     Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
// NEARE                    Internal. Only to be used through TI provided API.
#define VCE_FMUL0_RNDMUL_W                                                   3U
#define VCE_FMUL0_RNDMUL_M                                          0x0000000EU
#define VCE_FMUL0_RNDMUL_S                                                   1U
#define VCE_FMUL0_RNDMUL_AWAYZ                                      0x0000000AU
#define VCE_FMUL0_RNDMUL_NEARU                                      0x00000008U
#define VCE_FMUL0_RNDMUL_NINF                                       0x00000006U
#define VCE_FMUL0_RNDMUL_PINF                                       0x00000004U
#define VCE_FMUL0_RNDMUL_ZERO                                       0x00000002U
#define VCE_FMUL0_RNDMUL_NEARE                                      0x00000000U

// Field:     [0] OP
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// KMULT                    Internal. Only to be used through TI provided API.
// MULT                     Internal. Only to be used through TI provided API.
#define VCE_FMUL0_OP                                                0x00000001U
#define VCE_FMUL0_OP_M                                              0x00000001U
#define VCE_FMUL0_OP_S                                                       0U
#define VCE_FMUL0_OP_KMULT                                          0x00000001U
#define VCE_FMUL0_OP_MULT                                           0x00000000U

//*****************************************************************************
//
// Register: VCE_O_FMUL1
//
//*****************************************************************************
// Field:   [6:4] RNDADDSUB
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// AWAYZ                    Internal. Only to be used through TI provided API.
// NEARU                    Internal. Only to be used through TI provided API.
// NINF                     Internal. Only to be used through TI provided API.
// PINF                     Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
// NEARE                    Internal. Only to be used through TI provided API.
#define VCE_FMUL1_RNDADDSUB_W                                                3U
#define VCE_FMUL1_RNDADDSUB_M                                       0x00000070U
#define VCE_FMUL1_RNDADDSUB_S                                                4U
#define VCE_FMUL1_RNDADDSUB_AWAYZ                                   0x00000050U
#define VCE_FMUL1_RNDADDSUB_NEARU                                   0x00000040U
#define VCE_FMUL1_RNDADDSUB_NINF                                    0x00000030U
#define VCE_FMUL1_RNDADDSUB_PINF                                    0x00000020U
#define VCE_FMUL1_RNDADDSUB_ZERO                                    0x00000010U
#define VCE_FMUL1_RNDADDSUB_NEARE                                   0x00000000U

// Field:   [3:1] RNDMUL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// AWAYZ                    Internal. Only to be used through TI provided API.
// NEARU                    Internal. Only to be used through TI provided API.
// NINF                     Internal. Only to be used through TI provided API.
// PINF                     Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
// NEARE                    Internal. Only to be used through TI provided API.
#define VCE_FMUL1_RNDMUL_W                                                   3U
#define VCE_FMUL1_RNDMUL_M                                          0x0000000EU
#define VCE_FMUL1_RNDMUL_S                                                   1U
#define VCE_FMUL1_RNDMUL_AWAYZ                                      0x0000000AU
#define VCE_FMUL1_RNDMUL_NEARU                                      0x00000008U
#define VCE_FMUL1_RNDMUL_NINF                                       0x00000006U
#define VCE_FMUL1_RNDMUL_PINF                                       0x00000004U
#define VCE_FMUL1_RNDMUL_ZERO                                       0x00000002U
#define VCE_FMUL1_RNDMUL_NEARE                                      0x00000000U

// Field:     [0] OP
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// KMULT                    Internal. Only to be used through TI provided API.
// MULT                     Internal. Only to be used through TI provided API.
#define VCE_FMUL1_OP                                                0x00000001U
#define VCE_FMUL1_OP_M                                              0x00000001U
#define VCE_FMUL1_OP_S                                                       0U
#define VCE_FMUL1_OP_KMULT                                          0x00000001U
#define VCE_FMUL1_OP_MULT                                           0x00000000U

//*****************************************************************************
//
// Register: VCE_O_UCRD
//
//*****************************************************************************
// Field:     [0] MODE
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ROT                      Internal. Only to be used through TI provided API.
// VECT                     Internal. Only to be used through TI provided API.
#define VCE_UCRD_MODE                                               0x00000001U
#define VCE_UCRD_MODE_M                                             0x00000001U
#define VCE_UCRD_MODE_S                                                      0U
#define VCE_UCRD_MODE_ROT                                           0x00000001U
#define VCE_UCRD_MODE_VECT                                          0x00000000U

//*****************************************************************************
//
// Register: VCE_O_ADDSUB
//
//*****************************************************************************
// Field:   [4:2] RNDADD
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// AWAYZ                    Internal. Only to be used through TI provided API.
// NEARU                    Internal. Only to be used through TI provided API.
// NINF                     Internal. Only to be used through TI provided API.
// PINF                     Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
// NEARE                    Internal. Only to be used through TI provided API.
#define VCE_ADDSUB_RNDADD_W                                                  3U
#define VCE_ADDSUB_RNDADD_M                                         0x0000001CU
#define VCE_ADDSUB_RNDADD_S                                                  2U
#define VCE_ADDSUB_RNDADD_AWAYZ                                     0x00000014U
#define VCE_ADDSUB_RNDADD_NEARU                                     0x00000010U
#define VCE_ADDSUB_RNDADD_NINF                                      0x0000000CU
#define VCE_ADDSUB_RNDADD_PINF                                      0x00000008U
#define VCE_ADDSUB_RNDADD_ZERO                                      0x00000004U
#define VCE_ADDSUB_RNDADD_NEARE                                     0x00000000U

// Field:   [1:0] OP
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ACC                      Internal. Only to be used through TI provided API.
// ADDK                     Internal. Only to be used through TI provided API.
// SUB                      Internal. Only to be used through TI provided API.
// ADD                      Internal. Only to be used through TI provided API.
#define VCE_ADDSUB_OP_W                                                      2U
#define VCE_ADDSUB_OP_M                                             0x00000003U
#define VCE_ADDSUB_OP_S                                                      0U
#define VCE_ADDSUB_OP_ACC                                           0x00000003U
#define VCE_ADDSUB_OP_ADDK                                          0x00000002U
#define VCE_ADDSUB_OP_SUB                                           0x00000001U
#define VCE_ADDSUB_OP_ADD                                           0x00000000U

//*****************************************************************************
//
// Register: VCE_O_ADDSUBDECACC
//
//*****************************************************************************
// Field:   [9:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_ADDSUBDECACC_VAL_W                                              10U
#define VCE_ADDSUBDECACC_VAL_M                                      0x000003FFU
#define VCE_ADDSUBDECACC_VAL_S                                               0U

//*****************************************************************************
//
// Register: VCE_O_ADDSUBSTAT
//
//*****************************************************************************
// Field:     [1] LASTSIGNIM
//
// Internal. Only to be used through TI provided API.
#define VCE_ADDSUBSTAT_LASTSIGNIM                                   0x00000002U
#define VCE_ADDSUBSTAT_LASTSIGNIM_M                                 0x00000002U
#define VCE_ADDSUBSTAT_LASTSIGNIM_S                                          1U

// Field:     [0] LASTSIGNRE
//
// Internal. Only to be used through TI provided API.
#define VCE_ADDSUBSTAT_LASTSIGNRE                                   0x00000001U
#define VCE_ADDSUBSTAT_LASTSIGNRE_M                                 0x00000001U
#define VCE_ADDSUBSTAT_LASTSIGNRE_S                                          0U

//*****************************************************************************
//
// Register: VCE_O_MAXMIN
//
//*****************************************************************************
// Field:     [0] OP
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// MAX                      Internal. Only to be used through TI provided API.
// MIN                      Internal. Only to be used through TI provided API.
#define VCE_MAXMIN_OP                                               0x00000001U
#define VCE_MAXMIN_OP_M                                             0x00000001U
#define VCE_MAXMIN_OP_S                                                      0U
#define VCE_MAXMIN_OP_MAX                                           0x00000001U
#define VCE_MAXMIN_OP_MIN                                           0x00000000U

//*****************************************************************************
//
// Register: VCE_O_MAXMINDECACC
//
//*****************************************************************************
// Field:   [9:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_MAXMINDECACC_VAL_W                                              10U
#define VCE_MAXMINDECACC_VAL_M                                      0x000003FFU
#define VCE_MAXMINDECACC_VAL_S                                               0U

//*****************************************************************************
//
// Register: VCE_O_MAXMININDEX
//
//*****************************************************************************
// Field:   [9:0] VAL
//
// Internal. Only to be used through TI provided API.
#define VCE_MAXMININDEX_VAL_W                                               10U
#define VCE_MAXMININDEX_VAL_M                                       0x000003FFU
#define VCE_MAXMININDEX_VAL_S                                                0U

//*****************************************************************************
//
// Register: VCE_O_FX2FP
//
//*****************************************************************************
// Field:   [3:1] RND
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// NEARE                    Internal. Only to be used through TI provided API.
// AWAYZ                    Internal. Only to be used through TI provided API.
// NEARU                    Internal. Only to be used through TI provided API.
// NINF                     Internal. Only to be used through TI provided API.
// PINF                     Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_FX2FP_RND_W                                                      3U
#define VCE_FX2FP_RND_M                                             0x0000000EU
#define VCE_FX2FP_RND_S                                                      1U
#define VCE_FX2FP_RND_NEARE                                         0x0000000CU
#define VCE_FX2FP_RND_AWAYZ                                         0x0000000AU
#define VCE_FX2FP_RND_NEARU                                         0x00000008U
#define VCE_FX2FP_RND_NINF                                          0x00000006U
#define VCE_FX2FP_RND_PINF                                          0x00000004U
#define VCE_FX2FP_RND_ZERO                                          0x00000002U

// Field:     [0] OP
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// CONVR                    Internal. Only to be used through TI provided API.
// CONVA                    Internal. Only to be used through TI provided API.
#define VCE_FX2FP_OP                                                0x00000001U
#define VCE_FX2FP_OP_M                                              0x00000001U
#define VCE_FX2FP_OP_S                                                       0U
#define VCE_FX2FP_OP_CONVR                                          0x00000001U
#define VCE_FX2FP_OP_CONVA                                          0x00000000U

//*****************************************************************************
//
// Register: VCE_O_FX2FPR
//
//*****************************************************************************
// Field:  [15:0] R
//
// Internal. Only to be used through TI provided API.
#define VCE_FX2FPR_R_W                                                      16U
#define VCE_FX2FPR_R_M                                              0x0000FFFFU
#define VCE_FX2FPR_R_S                                                       0U

//*****************************************************************************
//
// Register: VCE_O_FX2FPCONVR
//
//*****************************************************************************
// Field:     [0] INIT
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_FX2FPCONVR_INIT                                         0x00000001U
#define VCE_FX2FPCONVR_INIT_M                                       0x00000001U
#define VCE_FX2FPCONVR_INIT_S                                                0U
#define VCE_FX2FPCONVR_INIT_ONE                                     0x00000001U
#define VCE_FX2FPCONVR_INIT_ZERO                                    0x00000000U

//*****************************************************************************
//
// Register: VCE_O_FDIV
//
//*****************************************************************************
// Field:   [2:0] RND
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// NEARE                    Internal. Only to be used through TI provided API.
// AWAYZ                    Internal. Only to be used through TI provided API.
// NEARU                    Internal. Only to be used through TI provided API.
// NINF                     Internal. Only to be used through TI provided API.
// PINF                     Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_FDIV_RND_W                                                       3U
#define VCE_FDIV_RND_M                                              0x00000007U
#define VCE_FDIV_RND_S                                                       0U
#define VCE_FDIV_RND_NEARE                                          0x00000006U
#define VCE_FDIV_RND_AWAYZ                                          0x00000005U
#define VCE_FDIV_RND_NEARU                                          0x00000004U
#define VCE_FDIV_RND_NINF                                           0x00000003U
#define VCE_FDIV_RND_PINF                                           0x00000002U
#define VCE_FDIV_RND_ZERO                                           0x00000001U

//*****************************************************************************
//
// Register: VCE_O_FDIVSTAT
//
//*****************************************************************************
// Field:     [0] DIVZERO
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// SET                      Internal. Only to be used through TI provided API.
// CLEAR                    Internal. Only to be used through TI provided API.
#define VCE_FDIVSTAT_DIVZERO                                        0x00000001U
#define VCE_FDIVSTAT_DIVZERO_M                                      0x00000001U
#define VCE_FDIVSTAT_DIVZERO_S                                               0U
#define VCE_FDIVSTAT_DIVZERO_SET                                    0x00000001U
#define VCE_FDIVSTAT_DIVZERO_CLEAR                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_CFG
//
//*****************************************************************************
// Field:   [8:7] PRESCAL
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// DIV4                     Internal. Only to be used through TI provided API.
// DIV3                     Internal. Only to be used through TI provided API.
// DIV2                     Internal. Only to be used through TI provided API.
// DIV1                     Internal. Only to be used through TI provided API.
#define VCE_CFG_PRESCAL_W                                                    2U
#define VCE_CFG_PRESCAL_M                                           0x00000180U
#define VCE_CFG_PRESCAL_S                                                    7U
#define VCE_CFG_PRESCAL_DIV4                                        0x00000180U
#define VCE_CFG_PRESCAL_DIV3                                        0x00000100U
#define VCE_CFG_PRESCAL_DIV2                                        0x00000080U
#define VCE_CFG_PRESCAL_DIV1                                        0x00000000U

// Field:     [6] TSCLR
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_CFG_TSCLR                                               0x00000040U
#define VCE_CFG_TSCLR_M                                             0x00000040U
#define VCE_CFG_TSCLR_S                                                      6U
#define VCE_CFG_TSCLR_ONE                                           0x00000040U
#define VCE_CFG_TSCLR_ZERO                                          0x00000000U

// Field:     [5] TSEN
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ONE                      Internal. Only to be used through TI provided API.
// ZERO                     Internal. Only to be used through TI provided API.
#define VCE_CFG_TSEN                                                0x00000020U
#define VCE_CFG_TSEN_M                                              0x00000020U
#define VCE_CFG_TSEN_S                                                       5U
#define VCE_CFG_TSEN_ONE                                            0x00000020U
#define VCE_CFG_TSEN_ZERO                                           0x00000000U

// Field:   [4:3] CH3EN
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// TOPSM                    Internal. Only to be used through TI provided API.
// NORM                     Internal. Only to be used through TI provided API.
// OFF                      Internal. Only to be used through TI provided API.
#define VCE_CFG_CH3EN_W                                                      2U
#define VCE_CFG_CH3EN_M                                             0x00000018U
#define VCE_CFG_CH3EN_S                                                      3U
#define VCE_CFG_CH3EN_TOPSM                                         0x00000010U
#define VCE_CFG_CH3EN_NORM                                          0x00000008U
#define VCE_CFG_CH3EN_OFF                                           0x00000000U

// Field:   [2:1] CH2EN
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// TOPSM                    Internal. Only to be used through TI provided API.
// NORM                     Internal. Only to be used through TI provided API.
// OFF                      Internal. Only to be used through TI provided API.
#define VCE_CFG_CH2EN_W                                                      2U
#define VCE_CFG_CH2EN_M                                             0x00000006U
#define VCE_CFG_CH2EN_S                                                      1U
#define VCE_CFG_CH2EN_TOPSM                                         0x00000004U
#define VCE_CFG_CH2EN_NORM                                          0x00000002U
#define VCE_CFG_CH2EN_OFF                                           0x00000000U

// Field:     [0] CH1EN
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ENABLED                  Internal. Only to be used through TI provided API.
// OFF                      Internal. Only to be used through TI provided API.
#define VCE_CFG_CH1EN                                               0x00000001U
#define VCE_CFG_CH1EN_M                                             0x00000001U
#define VCE_CFG_CH1EN_S                                                      0U
#define VCE_CFG_CH1EN_ENABLED                                       0x00000001U
#define VCE_CFG_CH1EN_OFF                                           0x00000000U

//*****************************************************************************
//
// Register: VCE_O_CH1CMD
//
//*****************************************************************************
// Field:  [15:8] PKTHDR
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CH1CMD_PKTHDR_W                                                  8U
#define VCE_CH1CMD_PKTHDR_M                                         0x0000FF00U
#define VCE_CH1CMD_PKTHDR_S                                                  8U
#define VCE_CH1CMD_PKTHDR_ALLONES                                   0x0000FF00U
#define VCE_CH1CMD_PKTHDR_ALLZEROS                                  0x00000000U

// Field:   [2:0] PARCNT
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CH1CMD_PARCNT_W                                                  3U
#define VCE_CH1CMD_PARCNT_M                                         0x00000007U
#define VCE_CH1CMD_PARCNT_S                                                  0U
#define VCE_CH1CMD_PARCNT_ALLONES                                   0x00000007U
#define VCE_CH1CMD_PARCNT_ALLZEROS                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_CH2CMD
//
//*****************************************************************************
// Field:  [15:8] PKTHDR
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CH2CMD_PKTHDR_W                                                  8U
#define VCE_CH2CMD_PKTHDR_M                                         0x0000FF00U
#define VCE_CH2CMD_PKTHDR_S                                                  8U
#define VCE_CH2CMD_PKTHDR_ALLONES                                   0x0000FF00U
#define VCE_CH2CMD_PKTHDR_ALLZEROS                                  0x00000000U

// Field:   [2:0] PARCNT
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CH2CMD_PARCNT_W                                                  3U
#define VCE_CH2CMD_PARCNT_M                                         0x00000007U
#define VCE_CH2CMD_PARCNT_S                                                  0U
#define VCE_CH2CMD_PARCNT_ALLONES                                   0x00000007U
#define VCE_CH2CMD_PARCNT_ALLZEROS                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_CH3CMD
//
//*****************************************************************************
// Field:  [15:8] PKTHDR
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CH3CMD_PKTHDR_W                                                  8U
#define VCE_CH3CMD_PKTHDR_M                                         0x0000FF00U
#define VCE_CH3CMD_PKTHDR_S                                                  8U
#define VCE_CH3CMD_PKTHDR_ALLONES                                   0x0000FF00U
#define VCE_CH3CMD_PKTHDR_ALLZEROS                                  0x00000000U

// Field:   [2:0] PARCNT
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CH3CMD_PARCNT_W                                                  3U
#define VCE_CH3CMD_PARCNT_M                                         0x00000007U
#define VCE_CH3CMD_PARCNT_S                                                  0U
#define VCE_CH3CMD_PARCNT_ALLONES                                   0x00000007U
#define VCE_CH3CMD_PARCNT_ALLZEROS                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_CH1PAR01
//
//*****************************************************************************
// Field: [31:16] PAR1
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CH1PAR01_PAR1_W                                                 16U
#define VCE_CH1PAR01_PAR1_M                                         0xFFFF0000U
#define VCE_CH1PAR01_PAR1_S                                                 16U
#define VCE_CH1PAR01_PAR1_ALLONES                                   0xFFFF0000U
#define VCE_CH1PAR01_PAR1_ALLZEROS                                  0x00000000U

// Field:  [15:0] PAR0
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CH1PAR01_PAR0_W                                                 16U
#define VCE_CH1PAR01_PAR0_M                                         0x0000FFFFU
#define VCE_CH1PAR01_PAR0_S                                                  0U
#define VCE_CH1PAR01_PAR0_ALLONES                                   0x0000FFFFU
#define VCE_CH1PAR01_PAR0_ALLZEROS                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_CH2PAR01
//
//*****************************************************************************
// Field: [31:16] PAR1
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CH2PAR01_PAR1_W                                                 16U
#define VCE_CH2PAR01_PAR1_M                                         0xFFFF0000U
#define VCE_CH2PAR01_PAR1_S                                                 16U
#define VCE_CH2PAR01_PAR1_ALLONES                                   0xFFFF0000U
#define VCE_CH2PAR01_PAR1_ALLZEROS                                  0x00000000U

// Field:  [15:0] PAR0
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CH2PAR01_PAR0_W                                                 16U
#define VCE_CH2PAR01_PAR0_M                                         0x0000FFFFU
#define VCE_CH2PAR01_PAR0_S                                                  0U
#define VCE_CH2PAR01_PAR0_ALLONES                                   0x0000FFFFU
#define VCE_CH2PAR01_PAR0_ALLZEROS                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_CH3PAR01
//
//*****************************************************************************
// Field: [31:16] PAR1
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CH3PAR01_PAR1_W                                                 16U
#define VCE_CH3PAR01_PAR1_M                                         0xFFFF0000U
#define VCE_CH3PAR01_PAR1_S                                                 16U
#define VCE_CH3PAR01_PAR1_ALLONES                                   0xFFFF0000U
#define VCE_CH3PAR01_PAR1_ALLZEROS                                  0x00000000U

// Field:  [15:0] PAR0
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CH3PAR01_PAR0_W                                                 16U
#define VCE_CH3PAR01_PAR0_M                                         0x0000FFFFU
#define VCE_CH3PAR01_PAR0_S                                                  0U
#define VCE_CH3PAR01_PAR0_ALLONES                                   0x0000FFFFU
#define VCE_CH3PAR01_PAR0_ALLZEROS                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_CH1PAR23
//
//*****************************************************************************
// Field: [31:16] PAR3
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CH1PAR23_PAR3_W                                                 16U
#define VCE_CH1PAR23_PAR3_M                                         0xFFFF0000U
#define VCE_CH1PAR23_PAR3_S                                                 16U
#define VCE_CH1PAR23_PAR3_ALLONES                                   0xFFFF0000U
#define VCE_CH1PAR23_PAR3_ALLZEROS                                  0x00000000U

// Field:  [15:0] PAR2
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CH1PAR23_PAR2_W                                                 16U
#define VCE_CH1PAR23_PAR2_M                                         0x0000FFFFU
#define VCE_CH1PAR23_PAR2_S                                                  0U
#define VCE_CH1PAR23_PAR2_ALLONES                                   0x0000FFFFU
#define VCE_CH1PAR23_PAR2_ALLZEROS                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_CH2PAR23
//
//*****************************************************************************
// Field: [31:16] PAR3
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CH2PAR23_PAR3_W                                                 16U
#define VCE_CH2PAR23_PAR3_M                                         0xFFFF0000U
#define VCE_CH2PAR23_PAR3_S                                                 16U
#define VCE_CH2PAR23_PAR3_ALLONES                                   0xFFFF0000U
#define VCE_CH2PAR23_PAR3_ALLZEROS                                  0x00000000U

// Field:  [15:0] PAR2
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CH2PAR23_PAR2_W                                                 16U
#define VCE_CH2PAR23_PAR2_M                                         0x0000FFFFU
#define VCE_CH2PAR23_PAR2_S                                                  0U
#define VCE_CH2PAR23_PAR2_ALLONES                                   0x0000FFFFU
#define VCE_CH2PAR23_PAR2_ALLZEROS                                  0x00000000U

//*****************************************************************************
//
// Register: VCE_O_CH3PAR23
//
//*****************************************************************************
// Field: [31:16] PAR3
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CH3PAR23_PAR3_W                                                 16U
#define VCE_CH3PAR23_PAR3_M                                         0xFFFF0000U
#define VCE_CH3PAR23_PAR3_S                                                 16U
#define VCE_CH3PAR23_PAR3_ALLONES                                   0xFFFF0000U
#define VCE_CH3PAR23_PAR3_ALLZEROS                                  0x00000000U

// Field:  [15:0] PAR2
//
// Internal. Only to be used through TI provided API.
// ENUMs:
// ALLONES                  Internal. Only to be used through TI provided API.
// ALLZEROS                 Internal. Only to be used through TI provided API.
#define VCE_CH3PAR23_PAR2_W                                                 16U
#define VCE_CH3PAR23_PAR2_M                                         0x0000FFFFU
#define VCE_CH3PAR23_PAR2_S                                                  0U
#define VCE_CH3PAR23_PAR2_ALLONES                                   0x0000FFFFU
#define VCE_CH3PAR23_PAR2_ALLZEROS                                  0x00000000U


#endif // __VCE__
