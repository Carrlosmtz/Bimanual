/*
 * BimanualHD2LfD_2016a_dt.h
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

#include "ext_types.h"

/* data type size table */
static uint_T rtDataTypeSizes[] = {
  sizeof(real_T),
  sizeof(real32_T),
  sizeof(int8_T),
  sizeof(uint8_T),
  sizeof(int16_T),
  sizeof(uint16_T),
  sizeof(int32_T),
  sizeof(uint32_T),
  sizeof(boolean_T),
  sizeof(fcn_call_T),
  sizeof(int_T),
  sizeof(pointer_T),
  sizeof(action_T),
  2*sizeof(uint32_T),
  sizeof(t_pstream)
};

/* data type name table */
static const char_T * rtDataTypeNames[] = {
  "real_T",
  "real32_T",
  "int8_T",
  "uint8_T",
  "int16_T",
  "uint16_T",
  "int32_T",
  "uint32_T",
  "boolean_T",
  "fcn_call_T",
  "int_T",
  "pointer_T",
  "action_T",
  "timer_uint32_pair_T",
  "t_pstream"
};

/* data type transitions for block I/O structure */
static DataTypeTransition rtBTransitions[] = {
  { (char_T *)(&BimanualHD2LfD_2016a_B.MovementOnOff), 0, 0, 29 },

  { (char_T *)(&BimanualHD2LfD_2016a_B.StreamServerRecv_o5), 8, 0, 1 }
  ,

  { (char_T *)(&BimanualHD2LfD_2016a_DW.StreamServerRecv_Stream), 14, 0, 2 },

  { (char_T *)(&BimanualHD2LfD_2016a_DW.ControlSpeed_PWORK.LoggedData), 11, 0,
    13 }
};

/* data type transition table for block I/O structure */
static DataTypeTransitionTable rtBTransTable = {
  4U,
  rtBTransitions
};

/* data type transitions for Parameters structure */
static DataTypeTransition rtPTransitions[] = {
  { (char_T *)(&BimanualHD2LfD_2016a_P.StreamServerRecv_default_value[0]), 0, 0,
    23 },

  { (char_T *)(&BimanualHD2LfD_2016a_P.StreamServerRecv_SndPriority), 6, 0, 4 },

  { (char_T *)(&BimanualHD2LfD_2016a_P.StreamServerRecv_SndSize), 7, 0, 8 },

  { (char_T *)(&BimanualHD2LfD_2016a_P.StreamServerRecv_Optimize), 2, 0, 4 },

  { (char_T *)(&BimanualHD2LfD_2016a_P.MovementOnOff_CurrentSetting), 3, 0, 48 },

  { (char_T *)(&BimanualHD2LfD_2016a_P.StreamServerRecv_Active), 8, 0, 2 }
};

/* data type transition table for Parameters structure */
static DataTypeTransitionTable rtPTransTable = {
  6U,
  rtPTransitions
};

/* [EOF] BimanualHD2LfD_2016a_dt.h */
