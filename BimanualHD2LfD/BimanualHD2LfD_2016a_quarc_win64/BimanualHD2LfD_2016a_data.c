/*
 * BimanualHD2LfD_2016a_data.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "BimanualHD2LfD_2016a".
 *
 * Model version              : 1.278
 * Simulink Coder version : 8.10 (R2016a) 10-Feb-2016
 * C source code generated on : Wed Jun 06 15:06:46 2018
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "BimanualHD2LfD_2016a.h"
#include "BimanualHD2LfD_2016a_private.h"

/* Block parameters (auto storage) */
P_BimanualHD2LfD_2016a_T BimanualHD2LfD_2016a_P = {
  /*  Mask Parameter: StreamServerRecv_default_value
   * Referenced by: '<S11>/Stream Server Recv'
   */
  { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },

  /*  Mask Parameter: StreamServerSend_default_value
   * Referenced by: '<S11>/Stream Server Send'
   */
  { 0.0, 0.0, 0.0, 0.0 },
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<Root>/velNone'
                                        */

  /*  Expression: [0 0 0]
   * Referenced by: '<Root>/dirNone'
   */
  { 0.0, 0.0, 0.0 },
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S11>/Constant2'
                                        */
  0.0,                                 /* Expression: 0
                                        * Referenced by: '<S11>/Constant3'
                                        */
  1.0,                                 /* Expression: 1
                                        * Referenced by: '<S11>/Constant1'
                                        */
  0,                                   /* Computed Parameter: StreamServerRecv_SndPriority
                                        * Referenced by: '<S11>/Stream Server Recv'
                                        */
  0,                                   /* Computed Parameter: StreamServerRecv_RcvPriority
                                        * Referenced by: '<S11>/Stream Server Recv'
                                        */
  0,                                   /* Computed Parameter: StreamServerSend_SndPriority
                                        * Referenced by: '<S11>/Stream Server Send'
                                        */
  0,                                   /* Computed Parameter: StreamServerSend_RcvPriority
                                        * Referenced by: '<S11>/Stream Server Send'
                                        */
  1460U,                               /* Computed Parameter: StreamServerRecv_SndSize
                                        * Referenced by: '<S11>/Stream Server Recv'
                                        */
  1460U,                               /* Computed Parameter: StreamServerRecv_RcvSize
                                        * Referenced by: '<S11>/Stream Server Recv'
                                        */
  1000U,                               /* Computed Parameter: StreamServerRecv_SndFIFO
                                        * Referenced by: '<S11>/Stream Server Recv'
                                        */
  1000U,                               /* Computed Parameter: StreamServerRecv_RcvFIFO
                                        * Referenced by: '<S11>/Stream Server Recv'
                                        */
  1460U,                               /* Computed Parameter: StreamServerSend_SndSize
                                        * Referenced by: '<S11>/Stream Server Send'
                                        */
  1460U,                               /* Computed Parameter: StreamServerSend_RcvSize
                                        * Referenced by: '<S11>/Stream Server Send'
                                        */
  1000U,                               /* Computed Parameter: StreamServerSend_SndFIFO
                                        * Referenced by: '<S11>/Stream Server Send'
                                        */
  1000U,                               /* Computed Parameter: StreamServerSend_RcvFIFO
                                        * Referenced by: '<S11>/Stream Server Send'
                                        */
  2,                                   /* Computed Parameter: StreamServerRecv_Optimize
                                        * Referenced by: '<S11>/Stream Server Recv'
                                        */
  2,                                   /* Computed Parameter: StreamServerRecv_Implementation
                                        * Referenced by: '<S11>/Stream Server Recv'
                                        */
  2,                                   /* Computed Parameter: StreamServerSend_Optimize
                                        * Referenced by: '<S11>/Stream Server Send'
                                        */
  2,                                   /* Computed Parameter: StreamServerSend_Implementation
                                        * Referenced by: '<S11>/Stream Server Send'
                                        */
  0U,                                  /* Computed Parameter: MovementOnOff_CurrentSetting
                                        * Referenced by: '<Root>/Movement  On//Off'
                                        */

  /*  Expression: uri_argument
   * Referenced by: '<S11>/Stream Server Recv'
   */
  { 115U, 104U, 109U, 101U, 109U, 58U, 47U, 47U, 85U, 68U, 80U, 109U, 101U, 109U,
    95U, 114U, 101U, 99U, 118U, 58U, 50U, 0U },
  1U,                                  /* Computed Parameter: StreamServerRecv_Endian
                                        * Referenced by: '<S11>/Stream Server Recv'
                                        */
  0U,                                  /* Computed Parameter: DirectionOnOff_CurrentSetting
                                        * Referenced by: '<Root>/Direction  On//Off'
                                        */

  /*  Expression: uri_argument
   * Referenced by: '<S11>/Stream Server Send'
   */
  { 115U, 104U, 109U, 101U, 109U, 58U, 47U, 47U, 85U, 68U, 80U, 109U, 101U, 109U,
    95U, 115U, 101U, 110U, 100U, 58U, 49U, 0U },
  1U,                                  /* Computed Parameter: StreamServerSend_Endian
                                        * Referenced by: '<S11>/Stream Server Send'
                                        */
  1,                                   /* Computed Parameter: StreamServerRecv_Active
                                        * Referenced by: '<S11>/Stream Server Recv'
                                        */
  1                                    /* Computed Parameter: StreamServerSend_Active
                                        * Referenced by: '<S11>/Stream Server Send'
                                        */
};
