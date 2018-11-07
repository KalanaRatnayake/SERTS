#include "__cf_robot.h"
#ifndef RTW_HEADER_robot_acc_h_
#define RTW_HEADER_robot_acc_h_
#include <stddef.h>
#include <float.h>
#include <string.h>
#ifndef robot_acc_COMMON_INCLUDES_
#define robot_acc_COMMON_INCLUDES_
#include <stdlib.h>
#define S_FUNCTION_NAME simulink_only_sfcn 
#define S_FUNCTION_LEVEL 2
#define RTW_GENERATED_S_FUNCTION
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "simtarget/slSimTgtSigstreamRTW.h"
#include "simtarget/slSimTgtSlioCoreRTW.h"
#include "simtarget/slSimTgtSlioClientsRTW.h"
#include "simtarget/slSimTgtSlioSdiRTW.h"
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#endif
#include "robot_acc_types.h"
#include "multiword_types.h"
#include "mwmathutil.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
typedef struct { real_T B_24_2064_0 ; real_T B_24_2065_0 ; real_T B_24_2066_0
[ 3664 ] ; real_T B_24_2067_0 [ 24 ] ; real_T B_24_2068_0 ; real_T
B_24_2069_0 [ 9 ] ; real_T B_24_2070_0 [ 9 ] ; real_T B_24_2071_0 ; real_T
B_24_2072_0 ; real_T B_24_2073_0 ; real_T B_24_2075_0 ; real_T B_24_2076_0 ;
real_T B_24_2077_0 ; real_T B_24_2079_0 ; real_T B_24_2080_0 ; real_T
B_24_2081_0 ; real_T B_24_2083_0 ; real_T B_24_2084_0 ; real_T B_24_2098_0 ;
real_T B_24_2099_0 ; real_T B_24_2100_0 ; real_T B_24_2102_0 ; real_T
B_24_2104_0 ; real_T B_24_2105_0 ; real_T B_24_2106_0 ; real_T B_24_2108_0 ;
real_T B_24_2110_0 ; real_T B_24_2111_0 ; real_T B_24_2112_0 ; real_T
B_24_2114_0 ; real_T B_24_2116_0 ; real_T B_24_2117_0 ; real_T B_24_2118_0 ;
real_T B_24_2120_0 ; real_T B_24_2122_0 ; real_T B_24_2123_0 ; real_T
B_24_2124_0 ; real_T B_24_2126_0 ; real_T B_24_2128_0 ; real_T B_24_2129_0 ;
real_T B_24_2130_0 ; real_T B_24_2132_0 ; real_T B_24_2134_0 ; real_T
B_24_2135_0 ; real_T B_24_2136_0 ; real_T B_24_2138_0 ; real_T B_24_2140_0 ;
real_T B_24_2141_0 ; real_T B_24_2142_0 ; real_T B_24_2144_0 ; real_T
B_24_2146_0 ; real_T B_24_2147_0 ; real_T B_24_2148_0 ; real_T B_24_2150_0 ;
real_T B_24_4958_0 [ 9 ] ; real_T B_24_4959_0 [ 9 ] ; real_T B_24_4960_0 ;
real_T B_24_4961_0 ; real_T B_24_4962_0 ; real_T B_24_4964_0 ; real_T
B_24_4965_0 ; real_T B_24_4966_0 ; real_T B_24_4968_0 ; real_T B_24_4969_0 ;
real_T B_24_4970_0 ; real_T B_24_4972_0 ; real_T B_24_4973_0 ; real_T
B_24_4974_0 ; real_T B_24_4975_0 ; real_T B_24_4976_0 ; real_T B_24_4978_0 ;
real_T B_24_4979_0 ; real_T B_24_4980_0 ; real_T B_24_5005_0 ; real_T
B_24_5006_0 ; real_T B_24_5007_0 ; real_T B_24_5009_0 ; real_T B_24_5011_0 ;
real_T B_24_5012_0 ; real_T B_24_5013_0 ; real_T B_24_5015_0 ; real_T
B_24_5017_0 ; real_T B_24_5018_0 ; real_T B_24_5019_0 ; real_T B_24_5021_0 ;
real_T B_24_5023_0 ; real_T B_24_5024_0 ; real_T B_24_5025_0 ; real_T
B_24_5027_0 ; real_T B_24_5029_0 ; real_T B_24_5030_0 ; real_T B_24_5031_0 ;
real_T B_24_5033_0 ; real_T B_24_5035_0 ; real_T B_24_5036_0 ; real_T
B_24_5037_0 ; real_T B_24_5039_0 ; real_T B_24_5041_0 ; real_T B_24_5042_0 ;
real_T B_24_5043_0 ; real_T B_24_5045_0 ; real_T B_24_5047_0 ; real_T
B_24_5048_0 ; real_T B_24_5049_0 ; real_T B_24_5051_0 ; real_T B_24_5053_0 ;
real_T B_24_5054_0 ; real_T B_24_5055_0 ; real_T B_24_5057_0 ; real_T
B_24_7929_0 [ 9 ] ; real_T B_24_7930_0 [ 9 ] ; real_T B_24_7932_0 ; real_T
B_24_7933_0 ; real_T B_24_7934_0 ; real_T B_24_7936_0 ; real_T B_24_7938_0 ;
real_T B_24_7939_0 ; real_T B_24_7940_0 ; real_T B_24_7942_0 ; real_T
B_24_7944_0 ; real_T B_24_7945_0 ; real_T B_24_7946_0 ; real_T B_24_7948_0 ;
real_T B_24_7951_0 ; real_T B_24_7952_0 ; real_T B_24_7953_0 ; real_T
B_24_7955_0 ; real_T B_24_7957_0 ; real_T B_24_7958_0 ; real_T B_24_7959_0 ;
real_T B_24_7961_0 ; real_T B_24_7963_0 ; real_T B_24_7964_0 ; real_T
B_24_7965_0 ; real_T B_24_7967_0 ; real_T B_24_7970_0 ; real_T B_24_7971_0 ;
real_T B_24_7972_0 ; real_T B_24_7974_0 ; real_T B_24_7976_0 ; real_T
B_24_7977_0 ; real_T B_24_7978_0 ; real_T B_24_7980_0 ; real_T B_24_7982_0 ;
real_T B_24_7983_0 ; real_T B_24_7984_0 ; real_T B_24_7986_0 ; real_T
B_24_7988_0 ; real_T B_24_7989_0 ; real_T B_24_7990_0 ; real_T B_24_7991_0 ;
real_T B_24_7992_0 ; real_T B_24_7994_0 ; real_T B_24_7995_0 ; real_T
B_24_7996_0 ; real_T B_24_10980_0 ; real_T B_24_10981_0 ; real_T B_24_10982_0
; real_T B_24_10983_0 [ 9 ] ; real_T B_24_10984_0 [ 9 ] ; real_T B_24_10986_0
; real_T B_24_10987_0 ; real_T B_24_10988_0 ; real_T B_24_10990_0 ; real_T
B_24_10992_0 ; real_T B_24_10993_0 ; real_T B_24_10994_0 ; real_T
B_24_10996_0 ; real_T B_24_10998_0 ; real_T B_24_10999_0 ; real_T
B_24_11000_0 ; real_T B_24_11002_0 ; real_T B_24_11005_0 ; real_T
B_24_11006_0 ; real_T B_24_11007_0 ; real_T B_24_11009_0 ; real_T
B_24_11011_0 ; real_T B_24_11012_0 ; real_T B_24_11013_0 ; real_T
B_24_11015_0 ; real_T B_24_11017_0 ; real_T B_24_11018_0 ; real_T
B_24_11019_0 ; real_T B_24_11021_0 ; real_T B_24_11024_0 ; real_T
B_24_11025_0 ; real_T B_24_11026_0 ; real_T B_24_11028_0 ; real_T
B_24_11030_0 ; real_T B_24_11031_0 ; real_T B_24_11032_0 ; real_T
B_24_11034_0 ; real_T B_24_11036_0 ; real_T B_24_11037_0 ; real_T
B_24_11038_0 ; real_T B_24_11040_0 ; real_T B_24_11042_0 ; real_T
B_24_11043_0 ; real_T B_24_11884_0 [ 4 ] ; real_T B_24_11885_0 [ 4 ] ; real_T
B_24_11886_0 [ 4 ] ; real_T B_24_11887_0 [ 4 ] ; real_T B_24_11888_0 [ 4 ] ;
real_T B_24_11889_0 [ 4 ] ; real_T B_24_11890_0 [ 4 ] ; real_T B_24_11891_0 [
4 ] ; real_T B_24_11892_0 [ 4 ] ; real_T B_24_11893_0 [ 4 ] ; real_T
B_24_11894_0 [ 4 ] ; real_T B_24_11895_0 [ 4 ] ; real_T B_23_0_1 [ 3 ] ;
real_T B_22_0_1 [ 3 ] ; real_T B_21_0_1 ; real_T B_21_0_2 ; real_T B_21_0_3 ;
real_T B_20_0_1 [ 3 ] ; real_T B_19_0_1 [ 3 ] ; real_T B_18_0_1 [ 3 ] ;
real_T B_17_0_1 [ 3 ] ; real_T B_16_0_1 [ 3 ] ; real_T B_15_0_1 ; real_T
B_15_0_2 ; real_T B_15_0_3 ; real_T B_14_0_1 [ 3 ] ; real_T B_13_0_1 [ 3 ] ;
real_T B_12_0_1 [ 3 ] ; real_T B_11_0_1 [ 3 ] ; real_T B_10_0_1 [ 3 ] ;
real_T B_9_0_1 ; real_T B_9_0_2 ; real_T B_9_0_3 ; real_T B_8_0_1 [ 3 ] ;
real_T B_7_0_1 [ 3 ] ; real_T B_6_0_1 [ 3 ] ; real_T B_5_0_1 [ 3 ] ; real_T
B_4_0_1 [ 3 ] ; real_T B_3_0_1 ; real_T B_3_0_2 ; real_T B_3_0_3 ; real_T
B_2_0_1 [ 3 ] ; real_T B_1_0_1 [ 3 ] ; real_T B_0_0_1 [ 3 ] ; } B_robot_T ;
typedef struct { real_T INPUT_1_1_1_discrete [ 2 ] ; real_T
INPUT_2_1_1_discrete [ 2 ] ; real_T INPUT_3_1_1_discrete [ 2 ] ; real_T
INPUT_4_1_1_discrete [ 2 ] ; real_T INPUT_5_1_1_discrete [ 2 ] ; real_T
INPUT_6_1_1_discrete [ 2 ] ; real_T INPUT_7_1_1_discrete [ 2 ] ; real_T
INPUT_8_1_1_discrete [ 2 ] ; real_T INPUT_9_1_1_discrete [ 2 ] ; real_T
INPUT_10_1_1_discrete [ 2 ] ; real_T INPUT_11_1_1_discrete [ 2 ] ; real_T
INPUT_12_1_1_discrete [ 2 ] ; struct { real_T modelTStart ; }
TransportDelay1_RWORK ; struct { real_T modelTStart ; } TransportDelay_RWORK
; struct { real_T modelTStart ; } TransportDelay7_RWORK ; struct { real_T
modelTStart ; } TransportDelay2_RWORK ; struct { real_T modelTStart ; }
TransportDelay3_RWORK ; struct { real_T modelTStart ; } TransportDelay4_RWORK
; struct { real_T modelTStart ; } TransportDelay5_RWORK ; struct { real_T
modelTStart ; } TransportDelay6_RWORK ; struct { real_T modelTStart ; }
TransportDelay8_RWORK ; struct { real_T modelTStart ; }
TransportDelay_RWORK_a ; struct { real_T modelTStart ; }
TransportDelay2_RWORK_a ; struct { real_T modelTStart ; }
TransportDelay3_RWORK_l ; struct { real_T modelTStart ; }
TransportDelay4_RWORK_h ; struct { real_T modelTStart ; }
TransportDelay5_RWORK_p ; struct { real_T modelTStart ; }
TransportDelay6_RWORK_o ; struct { real_T modelTStart ; }
TransportDelay7_RWORK_c ; struct { real_T modelTStart ; }
TransportDelay8_RWORK_e ; struct { real_T modelTStart ; }
TransportDelay9_RWORK ; void * STATE_1_Simulator ; void *
STATE_1_SimulationData ; void * STATE_1_DiagnosticManager ; void *
STATE_1_Logger ; void * STATE_1_SampleTimeIdx ; void * OUTPUT_1_0_Simulator ;
void * OUTPUT_1_0_SimulationData ; void * OUTPUT_1_0_DiagnosticManager ; void
* OUTPUT_1_0_Logger ; void * OUTPUT_1_0_SampleTimeIdx ; struct { void *
TUbufferPtrs [ 2 ] ; } TransportDelay1_PWORK ; struct { void * TUbufferPtrs [
2 ] ; } TransportDelay_PWORK ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay7_PWORK ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay2_PWORK ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay3_PWORK ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay4_PWORK ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay5_PWORK ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay6_PWORK ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay8_PWORK ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK_m ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay2_PWORK_d ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay3_PWORK_n ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay4_PWORK_g ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay5_PWORK_h ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay6_PWORK_j ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay7_PWORK_g ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay8_PWORK_j ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay9_PWORK ; struct { void * AQHandles ; void * SlioLTF ; }
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_0_PWORK ; struct {
void * AQHandles ; void * SlioLTF ; }
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_1_PWORK ; struct {
void * AQHandles ; void * SlioLTF ; }
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_2_PWORK ; struct {
void * AQHandles ; void * SlioLTF ; }
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_0_PWORK_h ; struct
{ void * AQHandles ; void * SlioLTF ; }
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_1_PWORK_h ; struct
{ void * AQHandles ; void * SlioLTF ; }
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_2_PWORK_j ; void *
SINK_1_Simulator ; void * SINK_1_SimulationData ; void *
SINK_1_DiagnosticManager ; void * SINK_1_Logger ; void * SINK_1_SampleTimeIdx
; int32_T MATLABFunction5_sysIdxToRun ; int32_T MATLABFunction4_sysIdxToRun ;
int32_T MATLABFunction1_sysIdxToRun ; int32_T MATLABFunction2_sysIdxToRun ;
int32_T MATLABFunction1_sysIdxToRun_d ; int32_T MATLABFunction_sysIdxToRun ;
int32_T MATLABFunction5_sysIdxToRun_a ; int32_T MATLABFunction4_sysIdxToRun_i
; int32_T MATLABFunction1_sysIdxToRun_p ; int32_T
MATLABFunction2_sysIdxToRun_h ; int32_T MATLABFunction1_sysIdxToRun_e ;
int32_T MATLABFunction_sysIdxToRun_p ; int32_T MATLABFunction5_sysIdxToRun_aj
; int32_T MATLABFunction4_sysIdxToRun_d ; int32_T
MATLABFunction1_sysIdxToRun_j ; int32_T MATLABFunction2_sysIdxToRun_j ;
int32_T MATLABFunction1_sysIdxToRun_a ; int32_T MATLABFunction_sysIdxToRun_i
; int32_T MATLABFunction5_sysIdxToRun_ah ; int32_T
MATLABFunction4_sysIdxToRun_m ; int32_T MATLABFunction1_sysIdxToRun_f ;
int32_T MATLABFunction2_sysIdxToRun_e ; int32_T MATLABFunction1_sysIdxToRun_g
; int32_T MATLABFunction_sysIdxToRun_e ; struct { int_T Tail ; int_T Head ;
int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay1_IWORK ; struct { int_T Tail ; int_T Head ; int_T Last ; int_T
CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay_IWORK ; struct {
int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } TransportDelay7_IWORK ; struct { int_T Tail ; int_T Head ;
int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay2_IWORK ; struct { int_T Tail ; int_T Head ; int_T Last ; int_T
CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay3_IWORK ; struct {
int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } TransportDelay4_IWORK ; struct { int_T Tail ; int_T Head ;
int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay5_IWORK ; struct { int_T Tail ; int_T Head ; int_T Last ; int_T
CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay6_IWORK ; struct {
int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } TransportDelay8_IWORK ; struct { int_T Tail ; int_T Head ;
int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_a ; struct { int_T Tail ; int_T Head ; int_T Last ;
int_T CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay2_IWORK_n ;
struct { int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } TransportDelay3_IWORK_g ; struct { int_T Tail ; int_T Head
; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay4_IWORK_p ; struct { int_T Tail ; int_T Head ; int_T Last ;
int_T CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay5_IWORK_i ;
struct { int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } TransportDelay6_IWORK_k ; struct { int_T Tail ; int_T Head
; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay7_IWORK_e ; struct { int_T Tail ; int_T Head ; int_T Last ;
int_T CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay8_IWORK_m ;
struct { int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } TransportDelay9_IWORK ; boolean_T
STATE_1_CallSimulatorOutput ; boolean_T OUTPUT_1_0_CallSimulatorOutput ;
boolean_T SINK_1_CallSimulatorOutput ; char_T pad_SINK_1_CallSimulatorOutput
[ 5 ] ; } DW_robot_T ; typedef struct { real_T robotPrismaticPzp [ 3664 ] ; }
X_robot_T ; typedef struct { real_T robotPrismaticPzp [ 3664 ] ; }
XDot_robot_T ; typedef struct { boolean_T robotPrismaticPzp [ 3664 ] ; }
XDis_robot_T ; typedef struct { real_T robotPrismaticPzp [ 3664 ] ; }
CStateAbsTol_robot_T ; struct P_robot_T_ { real_T P_0 ; real_T P_1 ; real_T
P_2 ; real_T P_3 [ 9 ] ; real_T P_4 [ 9 ] ; real_T P_5 ; real_T P_6 ; real_T
P_7 ; real_T P_8 ; real_T P_9 ; real_T P_10 ; real_T P_11 ; real_T P_12 ;
real_T P_13 ; real_T P_14 ; real_T P_15 ; real_T P_16 ; real_T P_17 ; real_T
P_18 ; real_T P_19 ; real_T P_20 ; real_T P_21 ; real_T P_22 ; real_T P_23 ;
real_T P_24 [ 5 ] ; real_T P_25 [ 5 ] ; real_T P_26 ; real_T P_27 [ 5 ] ;
real_T P_28 [ 5 ] ; real_T P_29 ; real_T P_30 [ 5 ] ; real_T P_31 [ 5 ] ;
real_T P_32 ; real_T P_33 [ 9 ] ; real_T P_34 [ 9 ] ; real_T P_35 ; real_T
P_36 [ 9 ] ; real_T P_37 [ 9 ] ; real_T P_38 ; real_T P_39 [ 9 ] ; real_T
P_40 [ 9 ] ; real_T P_41 ; real_T P_42 [ 6 ] ; real_T P_43 [ 6 ] ; real_T
P_44 ; real_T P_45 [ 6 ] ; real_T P_46 [ 6 ] ; real_T P_47 ; real_T P_48 [ 6
] ; real_T P_49 [ 6 ] ; real_T P_50 [ 9 ] ; real_T P_51 [ 9 ] ; real_T P_52 ;
real_T P_53 ; real_T P_54 ; real_T P_55 ; real_T P_56 ; real_T P_57 ; real_T
P_58 ; real_T P_59 ; real_T P_60 ; real_T P_61 ; real_T P_62 ; real_T P_63 ;
real_T P_64 ; real_T P_65 ; real_T P_66 ; real_T P_67 ; real_T P_68 ; real_T
P_69 ; real_T P_70 ; real_T P_71 ; real_T P_72 ; real_T P_73 ; real_T P_74 ;
real_T P_75 ; real_T P_76 ; real_T P_77 [ 5 ] ; real_T P_78 [ 5 ] ; real_T
P_79 ; real_T P_80 [ 5 ] ; real_T P_81 [ 5 ] ; real_T P_82 ; real_T P_83 [ 5
] ; real_T P_84 [ 5 ] ; real_T P_85 ; real_T P_86 [ 9 ] ; real_T P_87 [ 9 ] ;
real_T P_88 ; real_T P_89 [ 9 ] ; real_T P_90 [ 9 ] ; real_T P_91 ; real_T
P_92 [ 9 ] ; real_T P_93 [ 9 ] ; real_T P_94 ; real_T P_95 [ 6 ] ; real_T
P_96 [ 6 ] ; real_T P_97 ; real_T P_98 [ 6 ] ; real_T P_99 [ 6 ] ; real_T
P_100 ; real_T P_101 [ 6 ] ; real_T P_102 [ 6 ] ; real_T P_103 [ 9 ] ; real_T
P_104 [ 9 ] ; real_T P_105 ; real_T P_106 [ 6 ] ; real_T P_107 [ 6 ] ; real_T
P_108 ; real_T P_109 [ 6 ] ; real_T P_110 [ 6 ] ; real_T P_111 ; real_T P_112
[ 6 ] ; real_T P_113 [ 6 ] ; real_T P_114 ; real_T P_115 [ 9 ] ; real_T P_116
[ 9 ] ; real_T P_117 ; real_T P_118 [ 9 ] ; real_T P_119 [ 9 ] ; real_T P_120
; real_T P_121 [ 9 ] ; real_T P_122 [ 9 ] ; real_T P_123 ; real_T P_124 [ 5 ]
; real_T P_125 [ 5 ] ; real_T P_126 ; real_T P_127 [ 5 ] ; real_T P_128 [ 5 ]
; real_T P_129 ; real_T P_130 [ 5 ] ; real_T P_131 [ 5 ] ; real_T P_132 ;
real_T P_133 ; real_T P_134 ; real_T P_135 ; real_T P_136 ; real_T P_137 ;
real_T P_138 ; real_T P_139 ; real_T P_140 ; real_T P_141 [ 9 ] ; real_T
P_142 [ 9 ] ; real_T P_143 ; real_T P_144 [ 6 ] ; real_T P_145 [ 6 ] ; real_T
P_146 ; real_T P_147 [ 6 ] ; real_T P_148 [ 6 ] ; real_T P_149 ; real_T P_150
[ 6 ] ; real_T P_151 [ 6 ] ; real_T P_152 ; real_T P_153 [ 9 ] ; real_T P_154
[ 9 ] ; real_T P_155 ; real_T P_156 [ 9 ] ; real_T P_157 [ 9 ] ; real_T P_158
; real_T P_159 [ 9 ] ; real_T P_160 [ 9 ] ; real_T P_161 ; real_T P_162 [ 5 ]
; real_T P_163 [ 5 ] ; real_T P_164 ; real_T P_165 [ 5 ] ; real_T P_166 [ 5 ]
; real_T P_167 ; real_T P_168 [ 5 ] ; real_T P_169 [ 5 ] ; } ; extern
P_robot_T robot_rtDefaultP ;
#endif
