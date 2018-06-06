/*
 * BimanualHD2LfD_2016a.c
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
#include "BimanualHD2LfD_2016a_dt.h"

/* Block signals (auto storage) */
B_BimanualHD2LfD_2016a_T BimanualHD2LfD_2016a_B;

/* Block states (auto storage) */
DW_BimanualHD2LfD_2016a_T BimanualHD2LfD_2016a_DW;

/* Real-time model */
RT_MODEL_BimanualHD2LfD_2016a_T BimanualHD2LfD_2016a_M_;
RT_MODEL_BimanualHD2LfD_2016a_T *const BimanualHD2LfD_2016a_M =
  &BimanualHD2LfD_2016a_M_;

/* Model output function */
void BimanualHD2LfD_2016a_output(void)
{
  /* local block i/o variables */
  real_T rtb_TmpSignalConversionAtStream[4];

  /* ManualSwitch: '<Root>/Movement  On//Off' incorporates:
   *  Constant: '<Root>/velNone'
   */
  if (BimanualHD2LfD_2016a_P.MovementOnOff_CurrentSetting == 1) {
    BimanualHD2LfD_2016a_B.MovementOnOff = 0.0;
  } else {
    BimanualHD2LfD_2016a_B.MovementOnOff = BimanualHD2LfD_2016a_P.velNone_Value;
  }

  /* End of ManualSwitch: '<Root>/Movement  On//Off' */

  /* S-Function (stream_server_block): '<S11>/Stream Server Recv' */

  /* S-Function Block: BimanualHD2LfD_2016a/QUARC UDP Communication/Stream Server Recv (stream_server_block) */
  {
    t_pstream_state state;
    t_error send_result;
    t_error receive_result;
    if (BimanualHD2LfD_2016a_P.Constant3_Value) {
      send_result = pstream_send(BimanualHD2LfD_2016a_DW.StreamServerRecv_Stream,
        &BimanualHD2LfD_2016a_P.Constant2_Value);
    } else {
      send_result = 0;
    }

    receive_result = pstream_receive
      (BimanualHD2LfD_2016a_DW.StreamServerRecv_Stream,
       &BimanualHD2LfD_2016a_B.StreamServerRecv_o4[0]);
    BimanualHD2LfD_2016a_B.StreamServerRecv_o5 = (receive_result > 0);
    pstream_get_state(BimanualHD2LfD_2016a_DW.StreamServerRecv_Stream, &state);
  }

  /* Reshape: '<Root>/Reshape' */
  BimanualHD2LfD_2016a_B.Reshape[0] =
    BimanualHD2LfD_2016a_B.StreamServerRecv_o4[0];

  /* Reshape: '<Root>/Reshape1' */
  BimanualHD2LfD_2016a_B.Reshape1[0] =
    BimanualHD2LfD_2016a_B.StreamServerRecv_o4[3];

  /* Reshape: '<Root>/Reshape2' */
  BimanualHD2LfD_2016a_B.Reshape2[0] =
    BimanualHD2LfD_2016a_B.StreamServerRecv_o4[6];

  /* Reshape: '<Root>/Reshape3' */
  BimanualHD2LfD_2016a_B.Reshape3[0] =
    BimanualHD2LfD_2016a_B.StreamServerRecv_o4[9];

  /* Reshape: '<Root>/Reshape' */
  BimanualHD2LfD_2016a_B.Reshape[1] =
    BimanualHD2LfD_2016a_B.StreamServerRecv_o4[1];

  /* Reshape: '<Root>/Reshape1' */
  BimanualHD2LfD_2016a_B.Reshape1[1] =
    BimanualHD2LfD_2016a_B.StreamServerRecv_o4[4];

  /* Reshape: '<Root>/Reshape2' */
  BimanualHD2LfD_2016a_B.Reshape2[1] =
    BimanualHD2LfD_2016a_B.StreamServerRecv_o4[7];

  /* Reshape: '<Root>/Reshape3' */
  BimanualHD2LfD_2016a_B.Reshape3[1] =
    BimanualHD2LfD_2016a_B.StreamServerRecv_o4[10];

  /* Reshape: '<Root>/Reshape' */
  BimanualHD2LfD_2016a_B.Reshape[2] =
    BimanualHD2LfD_2016a_B.StreamServerRecv_o4[2];

  /* Reshape: '<Root>/Reshape1' */
  BimanualHD2LfD_2016a_B.Reshape1[2] =
    BimanualHD2LfD_2016a_B.StreamServerRecv_o4[5];

  /* Reshape: '<Root>/Reshape2' */
  BimanualHD2LfD_2016a_B.Reshape2[2] =
    BimanualHD2LfD_2016a_B.StreamServerRecv_o4[8];

  /* Reshape: '<Root>/Reshape3' */
  BimanualHD2LfD_2016a_B.Reshape3[2] =
    BimanualHD2LfD_2016a_B.StreamServerRecv_o4[11];

  /* ManualSwitch: '<Root>/Direction  On//Off' */
  if (BimanualHD2LfD_2016a_P.DirectionOnOff_CurrentSetting == 1) {
    /* SignalConversion: '<S11>/TmpSignal ConversionAtStream Server SendInport1' */
    rtb_TmpSignalConversionAtStream[0] = 0.0;
    rtb_TmpSignalConversionAtStream[1] = 0.0;
    rtb_TmpSignalConversionAtStream[2] = 0.0;
  } else {
    /* SignalConversion: '<S11>/TmpSignal ConversionAtStream Server SendInport1' incorporates:
     *  Constant: '<Root>/dirNone'
     */
    rtb_TmpSignalConversionAtStream[0] = BimanualHD2LfD_2016a_P.dirNone_Value[0];
    rtb_TmpSignalConversionAtStream[1] = BimanualHD2LfD_2016a_P.dirNone_Value[1];
    rtb_TmpSignalConversionAtStream[2] = BimanualHD2LfD_2016a_P.dirNone_Value[2];
  }

  /* End of ManualSwitch: '<Root>/Direction  On//Off' */

  /* SignalConversion: '<S11>/TmpSignal ConversionAtStream Server SendInport1' */
  rtb_TmpSignalConversionAtStream[3] = BimanualHD2LfD_2016a_B.MovementOnOff;

  /* S-Function (stream_server_block): '<S11>/Stream Server Send' */

  /* S-Function Block: BimanualHD2LfD_2016a/QUARC UDP Communication/Stream Server Send (stream_server_block) */
  {
    t_pstream_state state;
    t_error send_result;
    t_error receive_result;
    if (BimanualHD2LfD_2016a_P.Constant1_Value) {
      send_result = pstream_send(BimanualHD2LfD_2016a_DW.StreamServerSend_Stream,
        &rtb_TmpSignalConversionAtStream[0]);
    } else {
      send_result = 0;
    }

    receive_result = pstream_receive
      (BimanualHD2LfD_2016a_DW.StreamServerSend_Stream,
       &BimanualHD2LfD_2016a_B.StreamServerSend_o4[0]);
    pstream_get_state(BimanualHD2LfD_2016a_DW.StreamServerSend_Stream, &state);
  }
}

/* Model update function */
void BimanualHD2LfD_2016a_update(void)
{
  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++BimanualHD2LfD_2016a_M->Timing.clockTick0)) {
    ++BimanualHD2LfD_2016a_M->Timing.clockTickH0;
  }

  BimanualHD2LfD_2016a_M->Timing.t[0] =
    BimanualHD2LfD_2016a_M->Timing.clockTick0 *
    BimanualHD2LfD_2016a_M->Timing.stepSize0 +
    BimanualHD2LfD_2016a_M->Timing.clockTickH0 *
    BimanualHD2LfD_2016a_M->Timing.stepSize0 * 4294967296.0;
}

/* Model initialize function */
void BimanualHD2LfD_2016a_initialize(void)
{
  /* Start for S-Function (stream_server_block): '<S11>/Stream Server Recv' */

  /* S-Function Block: BimanualHD2LfD_2016a/QUARC UDP Communication/Stream Server Recv (stream_server_block) */
  {
    qthread_attr_t send_thread_attributes;
    qthread_attr_t receive_thread_attributes;
    struct qsched_param scheduling_parameters;
    int min_priority = qsched_get_priority_min(QSCHED_FIFO);
    int max_priority = qsched_get_priority_max(QSCHED_FIFO);
    t_pstream_options options;
    t_error result;
    BimanualHD2LfD_2016a_DW.StreamServerRecv_Stream = NULL;

    {
      int_T i1;
      real_T *y3 = &BimanualHD2LfD_2016a_B.StreamServerRecv_o4[0];
      const real_T *p_StreamServerRecv_default_value =
        BimanualHD2LfD_2016a_P.StreamServerRecv_default_value;
      for (i1=0; i1 < 12; i1++) {
        y3[i1] = p_StreamServerRecv_default_value[i1];
      }
    }

    result = 0;
    options.size = sizeof(options);
    options.flags = BimanualHD2LfD_2016a_P.StreamServerRecv_Endian &
      PSTREAM_FLAG_ENDIAN_MASK;
    if (BimanualHD2LfD_2016a_P.StreamServerRecv_Implementation ==
        STREAM_SERVER_IMPLEMENTATION_THREAD) {
      options.flags |= PSTREAM_FLAG_MULTITHREADED;
    }

    if (BimanualHD2LfD_2016a_P.StreamServerRecv_Optimize ==
        STREAM_SERVER_OPTIMIZE_LATENCY) {
      options.flags |= PSTREAM_FLAG_MINIMIZE_LATENCY;
    }

    options.flags |= PSTREAM_FLAG_SEND_MOST_RECENT;
    options.flags |= PSTREAM_FLAG_RECEIVE_MOST_RECENT;
    options.send_unit_size = 8;
    options.num_send_units = 1;
    options.send_buffer_size = BimanualHD2LfD_2016a_P.StreamServerRecv_SndSize;
    options.send_fifo_size = BimanualHD2LfD_2016a_P.StreamServerRecv_SndFIFO;
    options.num_send_dimensions = 0;
    options.max_send_dimensions = NULL;
    if (BimanualHD2LfD_2016a_P.StreamServerRecv_SndPriority < min_priority) {
      scheduling_parameters.sched_priority = min_priority;
    } else if (BimanualHD2LfD_2016a_P.StreamServerRecv_SndPriority >
               max_priority) {
      scheduling_parameters.sched_priority = max_priority;
    } else {
      scheduling_parameters.sched_priority =
        BimanualHD2LfD_2016a_P.StreamServerRecv_SndPriority;
    }

    qthread_attr_init(&send_thread_attributes);
    result = qthread_attr_setschedpolicy(&send_thread_attributes, QSCHED_FIFO);
    if (result == 0) {
      result = qthread_attr_setschedparam(&send_thread_attributes,
        &scheduling_parameters);
      if (result == 0) {
        result = qthread_attr_setinheritsched(&send_thread_attributes,
          QTHREAD_EXPLICIT_SCHED);
        if (result < 0) {
          rtmSetErrorStatus(BimanualHD2LfD_2016a_M,
                            "Unable to set scheduling inheritance for Stream Client sending thread");
        }
      } else {
        rtmSetErrorStatus(BimanualHD2LfD_2016a_M,
                          "The specified thread priority for the Stream Client sending thread is not valid for this target");
      }
    } else {
      rtmSetErrorStatus(BimanualHD2LfD_2016a_M,
                        "Unable to set scheduling policy for Stream Client sending thread");
    }

    options.send_thread_attributes = &send_thread_attributes;
    options.receive_unit_size = 8;
    options.num_receive_units = 12;
    options.receive_buffer_size =
      BimanualHD2LfD_2016a_P.StreamServerRecv_RcvSize;
    options.receive_fifo_size = BimanualHD2LfD_2016a_P.StreamServerRecv_RcvFIFO;
    options.num_receive_dimensions = 0;
    options.max_receive_dimensions = NULL;
    if (BimanualHD2LfD_2016a_P.StreamServerRecv_RcvPriority < min_priority) {
      scheduling_parameters.sched_priority = min_priority;
    } else if (BimanualHD2LfD_2016a_P.StreamServerRecv_RcvPriority >
               max_priority) {
      scheduling_parameters.sched_priority = max_priority;
    } else {
      scheduling_parameters.sched_priority =
        BimanualHD2LfD_2016a_P.StreamServerRecv_RcvPriority;
    }

    qthread_attr_init(&receive_thread_attributes);
    if (result == 0) {
      result = qthread_attr_setschedpolicy(&receive_thread_attributes,
        QSCHED_FIFO);
      if (result == 0) {
        result = qthread_attr_setschedparam(&receive_thread_attributes,
          &scheduling_parameters);
        if (result == 0) {
          result = qthread_attr_setinheritsched(&receive_thread_attributes,
            QTHREAD_EXPLICIT_SCHED);
          if (result < 0) {
            rtmSetErrorStatus(BimanualHD2LfD_2016a_M,
                              "Unable to set scheduling inheritance for Stream Client receiving thread");
          }
        } else {
          rtmSetErrorStatus(BimanualHD2LfD_2016a_M,
                            "The specified thread priority for the Stream Client receiving thread is not valid for this target");
        }
      } else {
        rtmSetErrorStatus(BimanualHD2LfD_2016a_M,
                          "Unable to set scheduling policy for Stream Client receiving thread");
      }
    }

    options.receive_thread_attributes = &receive_thread_attributes;
    if (result == 0) {
      result = pstream_listen((const char *)
        BimanualHD2LfD_2016a_P.StreamServerRecv_URI, &options,
        &BimanualHD2LfD_2016a_DW.StreamServerRecv_Stream);
      if (result < 0 && result != -QERR_WOULD_BLOCK) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(BimanualHD2LfD_2016a_M, _rt_error_message);
      }
    }

    qthread_attr_destroy(&send_thread_attributes);
    qthread_attr_destroy(&receive_thread_attributes);
  }

  /* Start for S-Function (stream_server_block): '<S11>/Stream Server Send' */

  /* S-Function Block: BimanualHD2LfD_2016a/QUARC UDP Communication/Stream Server Send (stream_server_block) */
  {
    qthread_attr_t send_thread_attributes;
    qthread_attr_t receive_thread_attributes;
    struct qsched_param scheduling_parameters;
    int min_priority = qsched_get_priority_min(QSCHED_FIFO);
    int max_priority = qsched_get_priority_max(QSCHED_FIFO);
    t_pstream_options options;
    t_error result;
    BimanualHD2LfD_2016a_DW.StreamServerSend_Stream = NULL;
    BimanualHD2LfD_2016a_B.StreamServerSend_o4[0] =
      BimanualHD2LfD_2016a_P.StreamServerSend_default_value[0];
    BimanualHD2LfD_2016a_B.StreamServerSend_o4[1] =
      BimanualHD2LfD_2016a_P.StreamServerSend_default_value[1];
    BimanualHD2LfD_2016a_B.StreamServerSend_o4[2] =
      BimanualHD2LfD_2016a_P.StreamServerSend_default_value[2];
    BimanualHD2LfD_2016a_B.StreamServerSend_o4[3] =
      BimanualHD2LfD_2016a_P.StreamServerSend_default_value[3];
    result = 0;
    options.size = sizeof(options);
    options.flags = BimanualHD2LfD_2016a_P.StreamServerSend_Endian &
      PSTREAM_FLAG_ENDIAN_MASK;
    if (BimanualHD2LfD_2016a_P.StreamServerSend_Implementation ==
        STREAM_SERVER_IMPLEMENTATION_THREAD) {
      options.flags |= PSTREAM_FLAG_MULTITHREADED;
    }

    if (BimanualHD2LfD_2016a_P.StreamServerSend_Optimize ==
        STREAM_SERVER_OPTIMIZE_LATENCY) {
      options.flags |= PSTREAM_FLAG_MINIMIZE_LATENCY;
    }

    options.flags |= PSTREAM_FLAG_SEND_MOST_RECENT;
    options.flags |= PSTREAM_FLAG_RECEIVE_MOST_RECENT;
    options.send_unit_size = 8;
    options.num_send_units = 4;
    options.send_buffer_size = BimanualHD2LfD_2016a_P.StreamServerSend_SndSize;
    options.send_fifo_size = BimanualHD2LfD_2016a_P.StreamServerSend_SndFIFO;
    options.num_send_dimensions = 0;
    options.max_send_dimensions = NULL;
    if (BimanualHD2LfD_2016a_P.StreamServerSend_SndPriority < min_priority) {
      scheduling_parameters.sched_priority = min_priority;
    } else if (BimanualHD2LfD_2016a_P.StreamServerSend_SndPriority >
               max_priority) {
      scheduling_parameters.sched_priority = max_priority;
    } else {
      scheduling_parameters.sched_priority =
        BimanualHD2LfD_2016a_P.StreamServerSend_SndPriority;
    }

    qthread_attr_init(&send_thread_attributes);
    result = qthread_attr_setschedpolicy(&send_thread_attributes, QSCHED_FIFO);
    if (result == 0) {
      result = qthread_attr_setschedparam(&send_thread_attributes,
        &scheduling_parameters);
      if (result == 0) {
        result = qthread_attr_setinheritsched(&send_thread_attributes,
          QTHREAD_EXPLICIT_SCHED);
        if (result < 0) {
          rtmSetErrorStatus(BimanualHD2LfD_2016a_M,
                            "Unable to set scheduling inheritance for Stream Client sending thread");
        }
      } else {
        rtmSetErrorStatus(BimanualHD2LfD_2016a_M,
                          "The specified thread priority for the Stream Client sending thread is not valid for this target");
      }
    } else {
      rtmSetErrorStatus(BimanualHD2LfD_2016a_M,
                        "Unable to set scheduling policy for Stream Client sending thread");
    }

    options.send_thread_attributes = &send_thread_attributes;
    options.receive_unit_size = 8;
    options.num_receive_units = 4;
    options.receive_buffer_size =
      BimanualHD2LfD_2016a_P.StreamServerSend_RcvSize;
    options.receive_fifo_size = BimanualHD2LfD_2016a_P.StreamServerSend_RcvFIFO;
    options.num_receive_dimensions = 0;
    options.max_receive_dimensions = NULL;
    if (BimanualHD2LfD_2016a_P.StreamServerSend_RcvPriority < min_priority) {
      scheduling_parameters.sched_priority = min_priority;
    } else if (BimanualHD2LfD_2016a_P.StreamServerSend_RcvPriority >
               max_priority) {
      scheduling_parameters.sched_priority = max_priority;
    } else {
      scheduling_parameters.sched_priority =
        BimanualHD2LfD_2016a_P.StreamServerSend_RcvPriority;
    }

    qthread_attr_init(&receive_thread_attributes);
    if (result == 0) {
      result = qthread_attr_setschedpolicy(&receive_thread_attributes,
        QSCHED_FIFO);
      if (result == 0) {
        result = qthread_attr_setschedparam(&receive_thread_attributes,
          &scheduling_parameters);
        if (result == 0) {
          result = qthread_attr_setinheritsched(&receive_thread_attributes,
            QTHREAD_EXPLICIT_SCHED);
          if (result < 0) {
            rtmSetErrorStatus(BimanualHD2LfD_2016a_M,
                              "Unable to set scheduling inheritance for Stream Client receiving thread");
          }
        } else {
          rtmSetErrorStatus(BimanualHD2LfD_2016a_M,
                            "The specified thread priority for the Stream Client receiving thread is not valid for this target");
        }
      } else {
        rtmSetErrorStatus(BimanualHD2LfD_2016a_M,
                          "Unable to set scheduling policy for Stream Client receiving thread");
      }
    }

    options.receive_thread_attributes = &receive_thread_attributes;
    if (result == 0) {
      result = pstream_listen((const char *)
        BimanualHD2LfD_2016a_P.StreamServerSend_URI, &options,
        &BimanualHD2LfD_2016a_DW.StreamServerSend_Stream);
      if (result < 0 && result != -QERR_WOULD_BLOCK) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(BimanualHD2LfD_2016a_M, _rt_error_message);
      }
    }

    qthread_attr_destroy(&send_thread_attributes);
    qthread_attr_destroy(&receive_thread_attributes);
  }
}

/* Model terminate function */
void BimanualHD2LfD_2016a_terminate(void)
{
  /* Terminate for S-Function (stream_server_block): '<S11>/Stream Server Recv' */

  /* S-Function Block: BimanualHD2LfD_2016a/QUARC UDP Communication/Stream Server Recv (stream_server_block) */
  {
    if (BimanualHD2LfD_2016a_DW.StreamServerRecv_Stream != NULL) {
      pstream_close(BimanualHD2LfD_2016a_DW.StreamServerRecv_Stream);
    }

    BimanualHD2LfD_2016a_DW.StreamServerRecv_Stream = NULL;
  }

  /* Terminate for S-Function (stream_server_block): '<S11>/Stream Server Send' */

  /* S-Function Block: BimanualHD2LfD_2016a/QUARC UDP Communication/Stream Server Send (stream_server_block) */
  {
    if (BimanualHD2LfD_2016a_DW.StreamServerSend_Stream != NULL) {
      pstream_close(BimanualHD2LfD_2016a_DW.StreamServerSend_Stream);
    }

    BimanualHD2LfD_2016a_DW.StreamServerSend_Stream = NULL;
  }
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/
void MdlOutputs(int_T tid)
{
  BimanualHD2LfD_2016a_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  BimanualHD2LfD_2016a_update();
  UNUSED_PARAMETER(tid);
}

void MdlInitializeSizes(void)
{
}

void MdlInitializeSampleTimes(void)
{
}

void MdlInitialize(void)
{
}

void MdlStart(void)
{
  BimanualHD2LfD_2016a_initialize();
}

void MdlTerminate(void)
{
  BimanualHD2LfD_2016a_terminate();
}

/* Registration function */
RT_MODEL_BimanualHD2LfD_2016a_T *BimanualHD2LfD_2016a(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)BimanualHD2LfD_2016a_M, 0,
                sizeof(RT_MODEL_BimanualHD2LfD_2016a_T));

  /* Initialize timing info */
  {
    int_T *mdlTsMap = BimanualHD2LfD_2016a_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    BimanualHD2LfD_2016a_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    BimanualHD2LfD_2016a_M->Timing.sampleTimes =
      (&BimanualHD2LfD_2016a_M->Timing.sampleTimesArray[0]);
    BimanualHD2LfD_2016a_M->Timing.offsetTimes =
      (&BimanualHD2LfD_2016a_M->Timing.offsetTimesArray[0]);

    /* task periods */
    BimanualHD2LfD_2016a_M->Timing.sampleTimes[0] = (0.002);

    /* task offsets */
    BimanualHD2LfD_2016a_M->Timing.offsetTimes[0] = (0.0);
  }

  rtmSetTPtr(BimanualHD2LfD_2016a_M, &BimanualHD2LfD_2016a_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = BimanualHD2LfD_2016a_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    BimanualHD2LfD_2016a_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(BimanualHD2LfD_2016a_M, -1);
  BimanualHD2LfD_2016a_M->Timing.stepSize0 = 0.002;

  /* External mode info */
  BimanualHD2LfD_2016a_M->Sizes.checksums[0] = (550266907U);
  BimanualHD2LfD_2016a_M->Sizes.checksums[1] = (2544446046U);
  BimanualHD2LfD_2016a_M->Sizes.checksums[2] = (2824691166U);
  BimanualHD2LfD_2016a_M->Sizes.checksums[3] = (3412559130U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[3];
    BimanualHD2LfD_2016a_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    systemRan[1] = &rtAlwaysEnabled;
    systemRan[2] = &rtAlwaysEnabled;
    rteiSetModelMappingInfoPtr(BimanualHD2LfD_2016a_M->extModeInfo,
      &BimanualHD2LfD_2016a_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(BimanualHD2LfD_2016a_M->extModeInfo,
                        BimanualHD2LfD_2016a_M->Sizes.checksums);
    rteiSetTPtr(BimanualHD2LfD_2016a_M->extModeInfo, rtmGetTPtr
                (BimanualHD2LfD_2016a_M));
  }

  BimanualHD2LfD_2016a_M->solverInfoPtr = (&BimanualHD2LfD_2016a_M->solverInfo);
  BimanualHD2LfD_2016a_M->Timing.stepSize = (0.002);
  rtsiSetFixedStepSize(&BimanualHD2LfD_2016a_M->solverInfo, 0.002);
  rtsiSetSolverMode(&BimanualHD2LfD_2016a_M->solverInfo,
                    SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  BimanualHD2LfD_2016a_M->ModelData.blockIO = ((void *) &BimanualHD2LfD_2016a_B);
  (void) memset(((void *) &BimanualHD2LfD_2016a_B), 0,
                sizeof(B_BimanualHD2LfD_2016a_T));

  /* parameters */
  BimanualHD2LfD_2016a_M->ModelData.defaultParam = ((real_T *)
    &BimanualHD2LfD_2016a_P);

  /* states (dwork) */
  BimanualHD2LfD_2016a_M->ModelData.dwork = ((void *) &BimanualHD2LfD_2016a_DW);
  (void) memset((void *)&BimanualHD2LfD_2016a_DW, 0,
                sizeof(DW_BimanualHD2LfD_2016a_T));

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    BimanualHD2LfD_2016a_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 15;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.BTransTable = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.PTransTable = &rtPTransTable;
  }

  /* Initialize Sizes */
  BimanualHD2LfD_2016a_M->Sizes.numContStates = (0);/* Number of continuous states */
  BimanualHD2LfD_2016a_M->Sizes.numY = (0);/* Number of model outputs */
  BimanualHD2LfD_2016a_M->Sizes.numU = (0);/* Number of model inputs */
  BimanualHD2LfD_2016a_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  BimanualHD2LfD_2016a_M->Sizes.numSampTimes = (1);/* Number of sample times */
  BimanualHD2LfD_2016a_M->Sizes.numBlocks = (19);/* Number of blocks */
  BimanualHD2LfD_2016a_M->Sizes.numBlockIO = (8);/* Number of block outputs */
  BimanualHD2LfD_2016a_M->Sizes.numBlockPrms = (89);/* Sum of parameter "widths" */
  return BimanualHD2LfD_2016a_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
