#include "__cf_robot.h"
#include <math.h>
#include "robot_acc.h"
#include "robot_acc_private.h"
#include <stdio.h>
#include "slexec_vm_simstruct_bridge.h"
#include "slexec_vm_zc_functions.h"
#include "slexec_vm_lookup_functions.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#include "simtarget/slAccSfcnBridge.h"
#ifndef __RTW_UTFREE__  
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T robot_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T * bufSzPtr , int_T *
tailPtr , int_T * headPtr , int_T * lastPtr , real_T tMinusDelay , real_T * *
tBufPtr , real_T * * uBufPtr , real_T * * xBufPtr , boolean_T isfixedbuf ,
boolean_T istransportdelay , int_T * maxNewBufSzPtr ) { int_T testIdx ; int_T
tail = * tailPtr ; int_T bufSz = * bufSzPtr ; real_T * tBuf = * tBufPtr ;
real_T * xBuf = ( NULL ) ; int_T numBuffer = 2 ; if ( istransportdelay ) {
numBuffer = 3 ; xBuf = * xBufPtr ; } testIdx = ( tail < ( bufSz - 1 ) ) ? (
tail + 1 ) : 0 ; if ( ( tMinusDelay <= tBuf [ testIdx ] ) && ! isfixedbuf ) {
int_T j ; real_T * tempT ; real_T * tempU ; real_T * tempX = ( NULL ) ;
real_T * uBuf = * uBufPtr ; int_T newBufSz = bufSz + 1024 ; if ( newBufSz > *
maxNewBufSzPtr ) { * maxNewBufSzPtr = newBufSz ; } tempU = ( real_T * )
utMalloc ( numBuffer * newBufSz * sizeof ( real_T ) ) ; if ( tempU == ( NULL
) ) { return ( false ) ; } tempT = tempU + newBufSz ; if ( istransportdelay )
tempX = tempT + newBufSz ; for ( j = tail ; j < bufSz ; j ++ ) { tempT [ j -
tail ] = tBuf [ j ] ; tempU [ j - tail ] = uBuf [ j ] ; if ( istransportdelay
) tempX [ j - tail ] = xBuf [ j ] ; } for ( j = 0 ; j < tail ; j ++ ) { tempT
[ j + bufSz - tail ] = tBuf [ j ] ; tempU [ j + bufSz - tail ] = uBuf [ j ] ;
if ( istransportdelay ) tempX [ j + bufSz - tail ] = xBuf [ j ] ; } if ( *
lastPtr > tail ) { * lastPtr -= tail ; } else { * lastPtr += ( bufSz - tail )
; } * tailPtr = 0 ; * headPtr = bufSz ; utFree ( uBuf ) ; * bufSzPtr =
newBufSz ; * tBufPtr = tempT ; * uBufPtr = tempU ; if ( istransportdelay ) *
xBufPtr = tempX ; } else { * tailPtr = testIdx ; } return ( true ) ; } real_T
robot_acc_rt_TDelayInterpolate ( real_T tMinusDelay , real_T tStart , real_T
* tBuf , real_T * uBuf , int_T bufSz , int_T * lastIdx , int_T oldestIdx ,
int_T newIdx , real_T initOutput , boolean_T discrete , boolean_T
minorStepAndTAtLastMajorOutput ) { int_T i ; real_T yout , t1 , t2 , u1 , u2
; if ( ( newIdx == 0 ) && ( oldestIdx == 0 ) && ( tMinusDelay > tStart ) )
return initOutput ; if ( tMinusDelay <= tStart ) return initOutput ; if ( (
tMinusDelay <= tBuf [ oldestIdx ] ) ) { if ( discrete ) { return ( uBuf [
oldestIdx ] ) ; } else { int_T tempIdx = oldestIdx + 1 ; if ( oldestIdx ==
bufSz - 1 ) tempIdx = 0 ; t1 = tBuf [ oldestIdx ] ; t2 = tBuf [ tempIdx ] ;
u1 = uBuf [ oldestIdx ] ; u2 = uBuf [ tempIdx ] ; if ( t2 == t1 ) { if (
tMinusDelay >= t2 ) { yout = u2 ; } else { yout = u1 ; } } else { real_T f1 =
( t2 - tMinusDelay ) / ( t2 - t1 ) ; real_T f2 = 1.0 - f1 ; yout = f1 * u1 +
f2 * u2 ; } return yout ; } } if ( minorStepAndTAtLastMajorOutput ) { if (
newIdx != 0 ) { if ( * lastIdx == newIdx ) { ( * lastIdx ) -- ; } newIdx -- ;
} else { if ( * lastIdx == newIdx ) { * lastIdx = bufSz - 1 ; } newIdx =
bufSz - 1 ; } } i = * lastIdx ; if ( tBuf [ i ] < tMinusDelay ) { while (
tBuf [ i ] < tMinusDelay ) { if ( i == newIdx ) break ; i = ( i < ( bufSz - 1
) ) ? ( i + 1 ) : 0 ; } } else { while ( tBuf [ i ] >= tMinusDelay ) { i = (
i > 0 ) ? i - 1 : ( bufSz - 1 ) ; } i = ( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0
; } * lastIdx = i ; if ( discrete ) { double tempEps = ( DBL_EPSILON ) *
128.0 ; double localEps = tempEps * muDoubleScalarAbs ( tBuf [ i ] ) ; if (
tempEps > localEps ) { localEps = tempEps ; } localEps = localEps / 2.0 ; if
( tMinusDelay >= ( tBuf [ i ] - localEps ) ) { yout = uBuf [ i ] ; } else {
if ( i == 0 ) { yout = uBuf [ bufSz - 1 ] ; } else { yout = uBuf [ i - 1 ] ;
} } } else { if ( i == 0 ) { t1 = tBuf [ bufSz - 1 ] ; u1 = uBuf [ bufSz - 1
] ; } else { t1 = tBuf [ i - 1 ] ; u1 = uBuf [ i - 1 ] ; } t2 = tBuf [ i ] ;
u2 = uBuf [ i ] ; if ( t2 == t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; }
else { yout = u1 ; } } else { real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 )
; real_T f2 = 1.0 - f1 ; yout = f1 * u1 + f2 * u2 ; } } return ( yout ) ; }
real_T look1_binlxpw ( real_T u0 , const real_T bp0 [ ] , const real_T table
[ ] , uint32_T maxIndex ) { real_T frac ; uint32_T iRght ; uint32_T iLeft ;
uint32_T bpIdx ; if ( u0 <= bp0 [ 0U ] ) { iLeft = 0U ; frac = ( u0 - bp0 [
0U ] ) / ( bp0 [ 1U ] - bp0 [ 0U ] ) ; } else if ( u0 < bp0 [ maxIndex ] ) {
bpIdx = maxIndex >> 1U ; iLeft = 0U ; iRght = maxIndex ; while ( iRght -
iLeft > 1U ) { if ( u0 < bp0 [ bpIdx ] ) { iRght = bpIdx ; } else { iLeft =
bpIdx ; } bpIdx = ( iRght + iLeft ) >> 1U ; } frac = ( u0 - bp0 [ iLeft ] ) /
( bp0 [ iLeft + 1U ] - bp0 [ iLeft ] ) ; } else { iLeft = maxIndex - 1U ;
frac = ( u0 - bp0 [ maxIndex - 1U ] ) / ( bp0 [ maxIndex ] - bp0 [ maxIndex -
1U ] ) ; } return ( table [ iLeft + 1U ] - table [ iLeft ] ) * frac + table [
iLeft ] ; } void rt_ssGetBlockPath ( SimStruct * S , int_T sysIdx , int_T
blkIdx , char_T * * path ) { _ssGetBlockPath ( S , sysIdx , blkIdx , path ) ;
} void rt_ssSet_slErrMsg ( SimStruct * S , void * diag ) { _ssSet_slErrMsg (
S , diag ) ; } void rt_ssReportDiagnosticAsWarning ( SimStruct * S , void *
diag ) { _ssReportDiagnosticAsWarning ( S , diag ) ; } static void mdlOutputs
( SimStruct * S , int_T tid ) { int32_T isHit ; B_robot_T * _rtB ; P_robot_T
* _rtP ; DW_robot_T * _rtDW ; _rtDW = ( ( DW_robot_T * ) ssGetRootDWork ( S )
) ; _rtP = ( ( P_robot_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( ( B_robot_T * )
_ssGetModelBlockIO ( S ) ) ; ssCallAccelRunBlock ( S , 24 , 2066 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 24 , 2067 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_24_2068_0 = _rtP -> P_2 * _rtB ->
B_24_2067_0 [ 1 ] ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay1_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T *
* ) & _rtDW -> TransportDelay1_PWORK . TUbufferPtrs [ 1 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_5 ; _rtB ->
B_24_2071_0 = robot_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * tBuffer
, * uBuffer , _rtDW -> TransportDelay1_IWORK . CircularBufSize , & _rtDW ->
TransportDelay1_IWORK . Last , _rtDW -> TransportDelay1_IWORK . Tail , _rtDW
-> TransportDelay1_IWORK . Head , _rtP -> P_6 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK .
TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - _rtP -> P_7 ; _rtB -> B_24_2072_0 =
robot_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * tBuffer , * uBuffer ,
_rtDW -> TransportDelay_IWORK . CircularBufSize , & _rtDW ->
TransportDelay_IWORK . Last , _rtDW -> TransportDelay_IWORK . Tail , _rtDW ->
TransportDelay_IWORK . Head , _rtP -> P_8 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay7_PWORK
. TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
TransportDelay7_PWORK . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - _rtP -> P_9 ; _rtB -> B_24_2073_0 =
robot_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * tBuffer , * uBuffer ,
_rtDW -> TransportDelay7_IWORK . CircularBufSize , & _rtDW ->
TransportDelay7_IWORK . Last , _rtDW -> TransportDelay7_IWORK . Tail , _rtDW
-> TransportDelay7_IWORK . Head , _rtP -> P_10 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } ssCallAccelRunBlock ( S , 0 , 0 , SS_CALL_MDL_OUTPUTS ) ; { real_T * *
uBuffer = ( real_T * * ) & _rtDW -> TransportDelay2_PWORK . TUbufferPtrs [ 0
] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay2_PWORK .
TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay =
simTime - _rtP -> P_11 ; _rtB -> B_24_2075_0 = robot_acc_rt_TDelayInterpolate
( tMinusDelay , 0.0 , * tBuffer , * uBuffer , _rtDW -> TransportDelay2_IWORK
. CircularBufSize , & _rtDW -> TransportDelay2_IWORK . Last , _rtDW ->
TransportDelay2_IWORK . Tail , _rtDW -> TransportDelay2_IWORK . Head , _rtP
-> P_12 , 0 , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && (
ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) ) ) ; } { real_T * * uBuffer =
( real_T * * ) & _rtDW -> TransportDelay3_PWORK . TUbufferPtrs [ 0 ] ; real_T
* * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay3_PWORK . TUbufferPtrs
[ 1 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP
-> P_13 ; _rtB -> B_24_2076_0 = robot_acc_rt_TDelayInterpolate ( tMinusDelay
, 0.0 , * tBuffer , * uBuffer , _rtDW -> TransportDelay3_IWORK .
CircularBufSize , & _rtDW -> TransportDelay3_IWORK . Last , _rtDW ->
TransportDelay3_IWORK . Tail , _rtDW -> TransportDelay3_IWORK . Head , _rtP
-> P_14 , 0 , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && (
ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) ) ) ; } { real_T * * uBuffer =
( real_T * * ) & _rtDW -> TransportDelay4_PWORK . TUbufferPtrs [ 0 ] ; real_T
* * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay4_PWORK . TUbufferPtrs
[ 1 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP
-> P_15 ; _rtB -> B_24_2077_0 = robot_acc_rt_TDelayInterpolate ( tMinusDelay
, 0.0 , * tBuffer , * uBuffer , _rtDW -> TransportDelay4_IWORK .
CircularBufSize , & _rtDW -> TransportDelay4_IWORK . Last , _rtDW ->
TransportDelay4_IWORK . Tail , _rtDW -> TransportDelay4_IWORK . Head , _rtP
-> P_16 , 0 , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && (
ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) ) ) ; } ssCallAccelRunBlock ( S
, 2 , 0 , SS_CALL_MDL_OUTPUTS ) ; { real_T * * uBuffer = ( real_T * * ) &
_rtDW -> TransportDelay5_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = (
real_T * * ) & _rtDW -> TransportDelay5_PWORK . TUbufferPtrs [ 1 ] ; real_T
simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_17 ; _rtB
-> B_24_2079_0 = robot_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , _rtDW -> TransportDelay5_IWORK . CircularBufSize , &
_rtDW -> TransportDelay5_IWORK . Last , _rtDW -> TransportDelay5_IWORK . Tail
, _rtDW -> TransportDelay5_IWORK . Head , _rtP -> P_18 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay6_PWORK
. TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
TransportDelay6_PWORK . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - _rtP -> P_19 ; _rtB -> B_24_2080_0 =
robot_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * tBuffer , * uBuffer ,
_rtDW -> TransportDelay6_IWORK . CircularBufSize , & _rtDW ->
TransportDelay6_IWORK . Last , _rtDW -> TransportDelay6_IWORK . Tail , _rtDW
-> TransportDelay6_IWORK . Head , _rtP -> P_20 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay8_PWORK
. TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
TransportDelay8_PWORK . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - _rtP -> P_21 ; _rtB -> B_24_2081_0 =
robot_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * tBuffer , * uBuffer ,
_rtDW -> TransportDelay8_IWORK . CircularBufSize , & _rtDW ->
TransportDelay8_IWORK . Last , _rtDW -> TransportDelay8_IWORK . Tail , _rtDW
-> TransportDelay8_IWORK . Head , _rtP -> P_22 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } ssCallAccelRunBlock ( S , 1 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_2083_0 = _rtB -> B_24_2064_0 - _rtB -> B_24_2067_0 [ 0 ] ; _rtB ->
B_24_2084_0 = _rtB -> B_24_2067_0 [ 4 ] - _rtB -> B_24_2065_0 ;
ssCallAccelRunBlock ( S , 4 , 0 , SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock
( S , 5 , 0 , SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 3 , 0 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_24_2100_0 = ssGetT ( S ) - _rtB ->
B_24_2099_0 ; _rtB -> B_24_2102_0 = look1_binlxpw ( muDoubleScalarRem ( _rtB
-> B_24_2100_0 , _rtB -> B_24_2098_0 ) , _rtP -> P_25 , _rtP -> P_24 , 4U ) ;
_rtB -> B_24_2106_0 = ssGetT ( S ) - _rtB -> B_24_2105_0 ; _rtB ->
B_24_2108_0 = look1_binlxpw ( muDoubleScalarRem ( _rtB -> B_24_2106_0 , _rtB
-> B_24_2104_0 ) , _rtP -> P_28 , _rtP -> P_27 , 4U ) ; _rtB -> B_24_2112_0 =
ssGetT ( S ) - _rtB -> B_24_2111_0 ; _rtB -> B_24_2114_0 = look1_binlxpw (
muDoubleScalarRem ( _rtB -> B_24_2112_0 , _rtB -> B_24_2110_0 ) , _rtP ->
P_31 , _rtP -> P_30 , 4U ) ; _rtB -> B_24_2118_0 = ssGetT ( S ) - _rtB ->
B_24_2117_0 ; _rtB -> B_24_2120_0 = look1_binlxpw ( muDoubleScalarRem ( _rtB
-> B_24_2118_0 , _rtB -> B_24_2116_0 ) , _rtP -> P_34 , _rtP -> P_33 , 8U ) ;
_rtB -> B_24_2124_0 = ssGetT ( S ) - _rtB -> B_24_2123_0 ; _rtB ->
B_24_2126_0 = look1_binlxpw ( muDoubleScalarRem ( _rtB -> B_24_2124_0 , _rtB
-> B_24_2122_0 ) , _rtP -> P_37 , _rtP -> P_36 , 8U ) ; _rtB -> B_24_2130_0 =
ssGetT ( S ) - _rtB -> B_24_2129_0 ; _rtB -> B_24_2132_0 = look1_binlxpw (
muDoubleScalarRem ( _rtB -> B_24_2130_0 , _rtB -> B_24_2128_0 ) , _rtP ->
P_40 , _rtP -> P_39 , 8U ) ; _rtB -> B_24_2136_0 = ssGetT ( S ) - _rtB ->
B_24_2135_0 ; _rtB -> B_24_2138_0 = look1_binlxpw ( muDoubleScalarRem ( _rtB
-> B_24_2136_0 , _rtB -> B_24_2134_0 ) , _rtP -> P_43 , _rtP -> P_42 , 5U ) ;
_rtB -> B_24_2142_0 = ssGetT ( S ) - _rtB -> B_24_2141_0 ; _rtB ->
B_24_2144_0 = look1_binlxpw ( muDoubleScalarRem ( _rtB -> B_24_2142_0 , _rtB
-> B_24_2140_0 ) , _rtP -> P_46 , _rtP -> P_45 , 5U ) ; _rtB -> B_24_2148_0 =
ssGetT ( S ) - _rtB -> B_24_2147_0 ; _rtB -> B_24_2150_0 = look1_binlxpw (
muDoubleScalarRem ( _rtB -> B_24_2148_0 , _rtB -> B_24_2146_0 ) , _rtP ->
P_49 , _rtP -> P_48 , 5U ) ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_m . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T *
* ) & _rtDW -> TransportDelay_PWORK_m . TUbufferPtrs [ 1 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_52 ; _rtB ->
B_24_4960_0 = robot_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * tBuffer
, * uBuffer , _rtDW -> TransportDelay_IWORK_a . CircularBufSize , & _rtDW ->
TransportDelay_IWORK_a . Last , _rtDW -> TransportDelay_IWORK_a . Tail ,
_rtDW -> TransportDelay_IWORK_a . Head , _rtP -> P_53 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay2_PWORK_d . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & _rtDW -> TransportDelay2_PWORK_d . TUbufferPtrs [ 1 ] ; real_T
simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_54 ; _rtB
-> B_24_4961_0 = robot_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , _rtDW -> TransportDelay2_IWORK_n . CircularBufSize , &
_rtDW -> TransportDelay2_IWORK_n . Last , _rtDW -> TransportDelay2_IWORK_n .
Tail , _rtDW -> TransportDelay2_IWORK_n . Head , _rtP -> P_55 , 0 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) ==
ssGetT ( S ) ) ) ) ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay3_PWORK_n . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & _rtDW -> TransportDelay3_PWORK_n . TUbufferPtrs [ 1 ] ; real_T
simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_56 ; _rtB
-> B_24_4962_0 = robot_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , _rtDW -> TransportDelay3_IWORK_g . CircularBufSize , &
_rtDW -> TransportDelay3_IWORK_g . Last , _rtDW -> TransportDelay3_IWORK_g .
Tail , _rtDW -> TransportDelay3_IWORK_g . Head , _rtP -> P_57 , 0 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) ==
ssGetT ( S ) ) ) ) ; } ssCallAccelRunBlock ( S , 6 , 0 , SS_CALL_MDL_OUTPUTS
) ; { real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay4_PWORK_g
. TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
TransportDelay4_PWORK_g . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S )
; real_T tMinusDelay = simTime - _rtP -> P_58 ; _rtB -> B_24_4964_0 =
robot_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * tBuffer , * uBuffer ,
_rtDW -> TransportDelay4_IWORK_p . CircularBufSize , & _rtDW ->
TransportDelay4_IWORK_p . Last , _rtDW -> TransportDelay4_IWORK_p . Tail ,
_rtDW -> TransportDelay4_IWORK_p . Head , _rtP -> P_59 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay5_PWORK_h . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & _rtDW -> TransportDelay5_PWORK_h . TUbufferPtrs [ 1 ] ; real_T
simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_60 ; _rtB
-> B_24_4965_0 = robot_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , _rtDW -> TransportDelay5_IWORK_i . CircularBufSize , &
_rtDW -> TransportDelay5_IWORK_i . Last , _rtDW -> TransportDelay5_IWORK_i .
Tail , _rtDW -> TransportDelay5_IWORK_i . Head , _rtP -> P_61 , 0 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) ==
ssGetT ( S ) ) ) ) ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay6_PWORK_j . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & _rtDW -> TransportDelay6_PWORK_j . TUbufferPtrs [ 1 ] ; real_T
simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_62 ; _rtB
-> B_24_4966_0 = robot_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , _rtDW -> TransportDelay6_IWORK_k . CircularBufSize , &
_rtDW -> TransportDelay6_IWORK_k . Last , _rtDW -> TransportDelay6_IWORK_k .
Tail , _rtDW -> TransportDelay6_IWORK_k . Head , _rtP -> P_63 , 0 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) ==
ssGetT ( S ) ) ) ) ; } ssCallAccelRunBlock ( S , 8 , 0 , SS_CALL_MDL_OUTPUTS
) ; { real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay7_PWORK_g
. TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
TransportDelay7_PWORK_g . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S )
; real_T tMinusDelay = simTime - _rtP -> P_64 ; _rtB -> B_24_4968_0 =
robot_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * tBuffer , * uBuffer ,
_rtDW -> TransportDelay7_IWORK_e . CircularBufSize , & _rtDW ->
TransportDelay7_IWORK_e . Last , _rtDW -> TransportDelay7_IWORK_e . Tail ,
_rtDW -> TransportDelay7_IWORK_e . Head , _rtP -> P_65 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay8_PWORK_j . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & _rtDW -> TransportDelay8_PWORK_j . TUbufferPtrs [ 1 ] ; real_T
simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_66 ; _rtB
-> B_24_4969_0 = robot_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
tBuffer , * uBuffer , _rtDW -> TransportDelay8_IWORK_m . CircularBufSize , &
_rtDW -> TransportDelay8_IWORK_m . Last , _rtDW -> TransportDelay8_IWORK_m .
Tail , _rtDW -> TransportDelay8_IWORK_m . Head , _rtP -> P_67 , 0 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) ==
ssGetT ( S ) ) ) ) ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay9_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T *
* ) & _rtDW -> TransportDelay9_PWORK . TUbufferPtrs [ 1 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_68 ; _rtB ->
B_24_4970_0 = robot_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * tBuffer
, * uBuffer , _rtDW -> TransportDelay9_IWORK . CircularBufSize , & _rtDW ->
TransportDelay9_IWORK . Last , _rtDW -> TransportDelay9_IWORK . Tail , _rtDW
-> TransportDelay9_IWORK . Head , _rtP -> P_69 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } ssCallAccelRunBlock ( S , 7 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_4973_0 = _rtB -> B_24_4972_0 - _rtB -> B_24_2067_0 [ 6 ] ; _rtB ->
B_24_4974_0 = _rtP -> P_71 * _rtB -> B_24_2067_0 [ 8 ] ; _rtB -> B_24_4976_0
= _rtB -> B_24_4975_0 - _rtB -> B_24_2067_0 [ 10 ] ; ssCallAccelRunBlock ( S
, 10 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_24_4978_0 = _rtP -> P_73 * _rtB
-> B_24_2067_0 [ 7 ] ; _rtB -> B_24_4979_0 = _rtP -> P_74 * _rtB ->
B_24_2067_0 [ 9 ] ; _rtB -> B_24_4980_0 = _rtP -> P_75 * _rtB -> B_24_2067_0
[ 11 ] ; ssCallAccelRunBlock ( S , 11 , 0 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 9 , 0 , SS_CALL_MDL_OUTPUTS ) ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { { if ( ( _rtDW ->
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_0_PWORK . AQHandles
|| _rtDW -> HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_0_PWORK
. SlioLTF ) && ssGetLogOutput ( S ) ) { sdiSlioSdiWriteSignal ( _rtDW ->
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_0_PWORK . AQHandles
, _rtDW -> HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_0_PWORK
. SlioLTF , 0 , ssGetTaskTime ( S , 1 ) , ( void * ) & _rtB -> B_9_0_1 ) ; }
} { if ( ( _rtDW ->
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_1_PWORK . AQHandles
|| _rtDW -> HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_1_PWORK
. SlioLTF ) && ssGetLogOutput ( S ) ) { sdiSlioSdiWriteSignal ( _rtDW ->
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_1_PWORK . AQHandles
, _rtDW -> HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_1_PWORK
. SlioLTF , 0 , ssGetTaskTime ( S , 1 ) , ( void * ) & _rtB -> B_9_0_2 ) ; }
} { if ( ( _rtDW ->
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_2_PWORK . AQHandles
|| _rtDW -> HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_2_PWORK
. SlioLTF ) && ssGetLogOutput ( S ) ) { sdiSlioSdiWriteSignal ( _rtDW ->
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_2_PWORK . AQHandles
, _rtDW -> HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_2_PWORK
. SlioLTF , 0 , ssGetTaskTime ( S , 1 ) , ( void * ) & _rtB -> B_9_0_3 ) ; }
} } _rtB -> B_24_5007_0 = ssGetT ( S ) - _rtB -> B_24_5006_0 ; _rtB ->
B_24_5009_0 = look1_binlxpw ( muDoubleScalarRem ( _rtB -> B_24_5007_0 , _rtB
-> B_24_5005_0 ) , _rtP -> P_78 , _rtP -> P_77 , 4U ) ; _rtB -> B_24_5013_0 =
ssGetT ( S ) - _rtB -> B_24_5012_0 ; _rtB -> B_24_5015_0 = look1_binlxpw (
muDoubleScalarRem ( _rtB -> B_24_5013_0 , _rtB -> B_24_5011_0 ) , _rtP ->
P_81 , _rtP -> P_80 , 4U ) ; _rtB -> B_24_5019_0 = ssGetT ( S ) - _rtB ->
B_24_5018_0 ; _rtB -> B_24_5021_0 = look1_binlxpw ( muDoubleScalarRem ( _rtB
-> B_24_5019_0 , _rtB -> B_24_5017_0 ) , _rtP -> P_84 , _rtP -> P_83 , 4U ) ;
_rtB -> B_24_5025_0 = ssGetT ( S ) - _rtB -> B_24_5024_0 ; _rtB ->
B_24_5027_0 = look1_binlxpw ( muDoubleScalarRem ( _rtB -> B_24_5025_0 , _rtB
-> B_24_5023_0 ) , _rtP -> P_87 , _rtP -> P_86 , 8U ) ; _rtB -> B_24_5031_0 =
ssGetT ( S ) - _rtB -> B_24_5030_0 ; _rtB -> B_24_5033_0 = look1_binlxpw (
muDoubleScalarRem ( _rtB -> B_24_5031_0 , _rtB -> B_24_5029_0 ) , _rtP ->
P_90 , _rtP -> P_89 , 8U ) ; _rtB -> B_24_5037_0 = ssGetT ( S ) - _rtB ->
B_24_5036_0 ; _rtB -> B_24_5039_0 = look1_binlxpw ( muDoubleScalarRem ( _rtB
-> B_24_5037_0 , _rtB -> B_24_5035_0 ) , _rtP -> P_93 , _rtP -> P_92 , 8U ) ;
_rtB -> B_24_5043_0 = ssGetT ( S ) - _rtB -> B_24_5042_0 ; _rtB ->
B_24_5045_0 = look1_binlxpw ( muDoubleScalarRem ( _rtB -> B_24_5043_0 , _rtB
-> B_24_5041_0 ) , _rtP -> P_96 , _rtP -> P_95 , 5U ) ; _rtB -> B_24_5049_0 =
ssGetT ( S ) - _rtB -> B_24_5048_0 ; _rtB -> B_24_5051_0 = look1_binlxpw (
muDoubleScalarRem ( _rtB -> B_24_5049_0 , _rtB -> B_24_5047_0 ) , _rtP ->
P_99 , _rtP -> P_98 , 5U ) ; _rtB -> B_24_5055_0 = ssGetT ( S ) - _rtB ->
B_24_5054_0 ; _rtB -> B_24_5057_0 = look1_binlxpw ( muDoubleScalarRem ( _rtB
-> B_24_5055_0 , _rtB -> B_24_5053_0 ) , _rtP -> P_102 , _rtP -> P_101 , 5U )
; _rtB -> B_24_7933_0 = ssGetT ( S ) - _rtB -> B_24_7932_0 ; _rtB ->
B_24_7936_0 = look1_binlxpw ( muDoubleScalarRem ( _rtB -> B_24_7933_0 , _rtB
-> B_24_7934_0 ) , _rtP -> P_107 , _rtP -> P_106 , 5U ) ; _rtB -> B_24_7939_0
= ssGetT ( S ) - _rtB -> B_24_7938_0 ; _rtB -> B_24_7942_0 = look1_binlxpw (
muDoubleScalarRem ( _rtB -> B_24_7939_0 , _rtB -> B_24_7940_0 ) , _rtP ->
P_110 , _rtP -> P_109 , 5U ) ; _rtB -> B_24_7945_0 = ssGetT ( S ) - _rtB ->
B_24_7944_0 ; _rtB -> B_24_7948_0 = look1_binlxpw ( muDoubleScalarRem ( _rtB
-> B_24_7945_0 , _rtB -> B_24_7946_0 ) , _rtP -> P_113 , _rtP -> P_112 , 5U )
; ssCallAccelRunBlock ( S , 12 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_7952_0 = ssGetT ( S ) - _rtB -> B_24_7951_0 ; _rtB -> B_24_7955_0 =
look1_binlxpw ( muDoubleScalarRem ( _rtB -> B_24_7952_0 , _rtB -> B_24_7953_0
) , _rtP -> P_116 , _rtP -> P_115 , 8U ) ; _rtB -> B_24_7958_0 = ssGetT ( S )
- _rtB -> B_24_7957_0 ; _rtB -> B_24_7961_0 = look1_binlxpw (
muDoubleScalarRem ( _rtB -> B_24_7958_0 , _rtB -> B_24_7959_0 ) , _rtP ->
P_119 , _rtP -> P_118 , 8U ) ; _rtB -> B_24_7964_0 = ssGetT ( S ) - _rtB ->
B_24_7963_0 ; _rtB -> B_24_7967_0 = look1_binlxpw ( muDoubleScalarRem ( _rtB
-> B_24_7964_0 , _rtB -> B_24_7965_0 ) , _rtP -> P_122 , _rtP -> P_121 , 8U )
; ssCallAccelRunBlock ( S , 14 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_7971_0 = ssGetT ( S ) - _rtB -> B_24_7970_0 ; _rtB -> B_24_7974_0 =
look1_binlxpw ( muDoubleScalarRem ( _rtB -> B_24_7971_0 , _rtB -> B_24_7972_0
) , _rtP -> P_125 , _rtP -> P_124 , 4U ) ; _rtB -> B_24_7977_0 = ssGetT ( S )
- _rtB -> B_24_7976_0 ; _rtB -> B_24_7980_0 = look1_binlxpw (
muDoubleScalarRem ( _rtB -> B_24_7977_0 , _rtB -> B_24_7978_0 ) , _rtP ->
P_128 , _rtP -> P_127 , 4U ) ; _rtB -> B_24_7983_0 = ssGetT ( S ) - _rtB ->
B_24_7982_0 ; _rtB -> B_24_7986_0 = look1_binlxpw ( muDoubleScalarRem ( _rtB
-> B_24_7983_0 , _rtB -> B_24_7984_0 ) , _rtP -> P_131 , _rtP -> P_130 , 4U )
; ssCallAccelRunBlock ( S , 13 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_7989_0 = _rtB -> B_24_7988_0 - _rtB -> B_24_2067_0 [ 12 ] ; _rtB ->
B_24_7990_0 = _rtP -> P_133 * _rtB -> B_24_2067_0 [ 14 ] ; _rtB ->
B_24_7992_0 = _rtB -> B_24_7991_0 - _rtB -> B_24_2067_0 [ 16 ] ;
ssCallAccelRunBlock ( S , 16 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_7994_0 = _rtP -> P_135 * _rtB -> B_24_2067_0 [ 13 ] ; _rtB ->
B_24_7995_0 = _rtP -> P_136 * _rtB -> B_24_2067_0 [ 15 ] ; _rtB ->
B_24_7996_0 = _rtP -> P_137 * _rtB -> B_24_2067_0 [ 17 ] ;
ssCallAccelRunBlock ( S , 17 , 0 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 15 , 0 , SS_CALL_MDL_OUTPUTS ) ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { { if ( ( _rtDW ->
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_0_PWORK_h .
AQHandles || _rtDW ->
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_0_PWORK_h . SlioLTF
) && ssGetLogOutput ( S ) ) { sdiSlioSdiWriteSignal ( _rtDW ->
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_0_PWORK_h .
AQHandles , _rtDW ->
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_0_PWORK_h . SlioLTF
, 0 , ssGetTaskTime ( S , 1 ) , ( void * ) & _rtB -> B_15_0_1 ) ; } } { if (
( _rtDW ->
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_1_PWORK_h .
AQHandles || _rtDW ->
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_1_PWORK_h . SlioLTF
) && ssGetLogOutput ( S ) ) { sdiSlioSdiWriteSignal ( _rtDW ->
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_1_PWORK_h .
AQHandles , _rtDW ->
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_1_PWORK_h . SlioLTF
, 0 , ssGetTaskTime ( S , 1 ) , ( void * ) & _rtB -> B_15_0_2 ) ; } } { if (
( _rtDW ->
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_2_PWORK_j .
AQHandles || _rtDW ->
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_2_PWORK_j . SlioLTF
) && ssGetLogOutput ( S ) ) { sdiSlioSdiWriteSignal ( _rtDW ->
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_2_PWORK_j .
AQHandles , _rtDW ->
HiddenToAsyncQueue_InsertedFor_MATLABFunction1_at_outport_2_PWORK_j . SlioLTF
, 0 , ssGetTaskTime ( S , 1 ) , ( void * ) & _rtB -> B_15_0_3 ) ; } } } _rtB
-> B_24_10982_0 = _rtP -> P_140 * _rtB -> B_24_2067_0 [ 19 ] ; _rtB ->
B_24_10987_0 = ssGetT ( S ) - _rtB -> B_24_10986_0 ; _rtB -> B_24_10990_0 =
look1_binlxpw ( muDoubleScalarRem ( _rtB -> B_24_10987_0 , _rtB ->
B_24_10988_0 ) , _rtP -> P_145 , _rtP -> P_144 , 5U ) ; _rtB -> B_24_10993_0
= ssGetT ( S ) - _rtB -> B_24_10992_0 ; _rtB -> B_24_10996_0 = look1_binlxpw
( muDoubleScalarRem ( _rtB -> B_24_10993_0 , _rtB -> B_24_10994_0 ) , _rtP ->
P_148 , _rtP -> P_147 , 5U ) ; _rtB -> B_24_10999_0 = ssGetT ( S ) - _rtB ->
B_24_10998_0 ; _rtB -> B_24_11002_0 = look1_binlxpw ( muDoubleScalarRem (
_rtB -> B_24_10999_0 , _rtB -> B_24_11000_0 ) , _rtP -> P_151 , _rtP -> P_150
, 5U ) ; ssCallAccelRunBlock ( S , 18 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_11006_0 = ssGetT ( S ) - _rtB -> B_24_11005_0 ; _rtB -> B_24_11009_0 =
look1_binlxpw ( muDoubleScalarRem ( _rtB -> B_24_11006_0 , _rtB ->
B_24_11007_0 ) , _rtP -> P_154 , _rtP -> P_153 , 8U ) ; _rtB -> B_24_11012_0
= ssGetT ( S ) - _rtB -> B_24_11011_0 ; _rtB -> B_24_11015_0 = look1_binlxpw
( muDoubleScalarRem ( _rtB -> B_24_11012_0 , _rtB -> B_24_11013_0 ) , _rtP ->
P_157 , _rtP -> P_156 , 8U ) ; _rtB -> B_24_11018_0 = ssGetT ( S ) - _rtB ->
B_24_11017_0 ; _rtB -> B_24_11021_0 = look1_binlxpw ( muDoubleScalarRem (
_rtB -> B_24_11018_0 , _rtB -> B_24_11019_0 ) , _rtP -> P_160 , _rtP -> P_159
, 8U ) ; ssCallAccelRunBlock ( S , 20 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_11025_0 = ssGetT ( S ) - _rtB -> B_24_11024_0 ; _rtB -> B_24_11028_0 =
look1_binlxpw ( muDoubleScalarRem ( _rtB -> B_24_11025_0 , _rtB ->
B_24_11026_0 ) , _rtP -> P_163 , _rtP -> P_162 , 4U ) ; _rtB -> B_24_11031_0
= ssGetT ( S ) - _rtB -> B_24_11030_0 ; _rtB -> B_24_11034_0 = look1_binlxpw
( muDoubleScalarRem ( _rtB -> B_24_11031_0 , _rtB -> B_24_11032_0 ) , _rtP ->
P_166 , _rtP -> P_165 , 4U ) ; _rtB -> B_24_11037_0 = ssGetT ( S ) - _rtB ->
B_24_11036_0 ; _rtB -> B_24_11040_0 = look1_binlxpw ( muDoubleScalarRem (
_rtB -> B_24_11037_0 , _rtB -> B_24_11038_0 ) , _rtP -> P_169 , _rtP -> P_168
, 4U ) ; ssCallAccelRunBlock ( S , 19 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_11042_0 = _rtB -> B_24_10980_0 - _rtB -> B_24_2067_0 [ 18 ] ; _rtB ->
B_24_11043_0 = _rtB -> B_24_2067_0 [ 22 ] - _rtB -> B_24_10981_0 ;
ssCallAccelRunBlock ( S , 22 , 0 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 23 , 0 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 21 , 0 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_24_11884_0 [ 0 ] = _rtB -> B_3_0_1 ; _rtB -> B_24_11884_0 [ 1 ] = 0.0 ;
_rtB -> B_24_11884_0 [ 2 ] = 0.0 ; _rtB -> B_24_11884_0 [ 3 ] = 0.0 ; _rtB ->
B_24_11885_0 [ 0 ] = _rtB -> B_3_0_2 ; _rtB -> B_24_11885_0 [ 1 ] = 0.0 ;
_rtB -> B_24_11885_0 [ 2 ] = 0.0 ; _rtB -> B_24_11885_0 [ 3 ] = 0.0 ; _rtB ->
B_24_11886_0 [ 0 ] = _rtB -> B_3_0_3 ; _rtB -> B_24_11886_0 [ 1 ] = 0.0 ;
_rtB -> B_24_11886_0 [ 2 ] = 0.0 ; _rtB -> B_24_11886_0 [ 3 ] = 0.0 ; _rtB ->
B_24_11887_0 [ 0 ] = _rtB -> B_9_0_1 ; _rtB -> B_24_11887_0 [ 1 ] = 0.0 ;
_rtB -> B_24_11887_0 [ 2 ] = 0.0 ; _rtB -> B_24_11887_0 [ 3 ] = 0.0 ; _rtB ->
B_24_11888_0 [ 0 ] = _rtB -> B_9_0_2 ; _rtB -> B_24_11888_0 [ 1 ] = 0.0 ;
_rtB -> B_24_11888_0 [ 2 ] = 0.0 ; _rtB -> B_24_11888_0 [ 3 ] = 0.0 ; _rtB ->
B_24_11889_0 [ 0 ] = _rtB -> B_9_0_3 ; _rtB -> B_24_11889_0 [ 1 ] = 0.0 ;
_rtB -> B_24_11889_0 [ 2 ] = 0.0 ; _rtB -> B_24_11889_0 [ 3 ] = 0.0 ; _rtB ->
B_24_11890_0 [ 0 ] = _rtB -> B_15_0_1 ; _rtB -> B_24_11890_0 [ 1 ] = 0.0 ;
_rtB -> B_24_11890_0 [ 2 ] = 0.0 ; _rtB -> B_24_11890_0 [ 3 ] = 0.0 ; _rtB ->
B_24_11891_0 [ 0 ] = _rtB -> B_15_0_2 ; _rtB -> B_24_11891_0 [ 1 ] = 0.0 ;
_rtB -> B_24_11891_0 [ 2 ] = 0.0 ; _rtB -> B_24_11891_0 [ 3 ] = 0.0 ; _rtB ->
B_24_11892_0 [ 0 ] = _rtB -> B_15_0_3 ; _rtB -> B_24_11892_0 [ 1 ] = 0.0 ;
_rtB -> B_24_11892_0 [ 2 ] = 0.0 ; _rtB -> B_24_11892_0 [ 3 ] = 0.0 ; _rtB ->
B_24_11893_0 [ 0 ] = _rtB -> B_21_0_1 ; _rtB -> B_24_11893_0 [ 1 ] = 0.0 ;
_rtB -> B_24_11893_0 [ 2 ] = 0.0 ; _rtB -> B_24_11893_0 [ 3 ] = 0.0 ; _rtB ->
B_24_11894_0 [ 0 ] = _rtB -> B_21_0_2 ; _rtB -> B_24_11894_0 [ 1 ] = 0.0 ;
_rtB -> B_24_11894_0 [ 2 ] = 0.0 ; _rtB -> B_24_11894_0 [ 3 ] = 0.0 ; _rtB ->
B_24_11895_0 [ 0 ] = _rtB -> B_21_0_3 ; _rtB -> B_24_11895_0 [ 1 ] = 0.0 ;
_rtB -> B_24_11895_0 [ 2 ] = 0.0 ; _rtB -> B_24_11895_0 [ 3 ] = 0.0 ;
ssCallAccelRunBlock ( S , 24 , 11896 , SS_CALL_MDL_OUTPUTS ) ;
UNUSED_PARAMETER ( tid ) ; } static void mdlOutputsTID2 ( SimStruct * S ,
int_T tid ) { B_robot_T * _rtB ; P_robot_T * _rtP ; _rtP = ( ( P_robot_T * )
ssGetModelRtp ( S ) ) ; _rtB = ( ( B_robot_T * ) _ssGetModelBlockIO ( S ) ) ;
_rtB -> B_24_2064_0 = _rtP -> P_0 ; _rtB -> B_24_2065_0 = _rtP -> P_1 ; _rtB
-> B_24_2098_0 = _rtP -> P_23 ; _rtB -> B_24_2104_0 = _rtP -> P_26 ; _rtB ->
B_24_2110_0 = _rtP -> P_29 ; _rtB -> B_24_2116_0 = _rtP -> P_32 ; _rtB ->
B_24_2122_0 = _rtP -> P_35 ; _rtB -> B_24_2128_0 = _rtP -> P_38 ; _rtB ->
B_24_2134_0 = _rtP -> P_41 ; _rtB -> B_24_2140_0 = _rtP -> P_44 ; _rtB ->
B_24_2146_0 = _rtP -> P_47 ; _rtB -> B_24_4972_0 = _rtP -> P_70 ; _rtB ->
B_24_4975_0 = _rtP -> P_72 ; _rtB -> B_24_5005_0 = _rtP -> P_76 ; _rtB ->
B_24_5011_0 = _rtP -> P_79 ; _rtB -> B_24_5017_0 = _rtP -> P_82 ; _rtB ->
B_24_5023_0 = _rtP -> P_85 ; _rtB -> B_24_5029_0 = _rtP -> P_88 ; _rtB ->
B_24_5035_0 = _rtP -> P_91 ; _rtB -> B_24_5041_0 = _rtP -> P_94 ; _rtB ->
B_24_5047_0 = _rtP -> P_97 ; _rtB -> B_24_5053_0 = _rtP -> P_100 ; _rtB ->
B_24_7934_0 = _rtP -> P_105 ; _rtB -> B_24_7940_0 = _rtP -> P_108 ; _rtB ->
B_24_7946_0 = _rtP -> P_111 ; _rtB -> B_24_7953_0 = _rtP -> P_114 ; _rtB ->
B_24_7959_0 = _rtP -> P_117 ; _rtB -> B_24_7965_0 = _rtP -> P_120 ; _rtB ->
B_24_7972_0 = _rtP -> P_123 ; _rtB -> B_24_7978_0 = _rtP -> P_126 ; _rtB ->
B_24_7984_0 = _rtP -> P_129 ; _rtB -> B_24_7988_0 = _rtP -> P_132 ; _rtB ->
B_24_7991_0 = _rtP -> P_134 ; _rtB -> B_24_10980_0 = _rtP -> P_138 ; _rtB ->
B_24_10981_0 = _rtP -> P_139 ; memcpy ( & _rtB -> B_24_2069_0 [ 0 ] , & _rtP
-> P_3 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_24_2070_0 [ 0
] , & _rtP -> P_4 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB ->
B_24_4958_0 [ 0 ] , & _rtP -> P_50 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy
( & _rtB -> B_24_4959_0 [ 0 ] , & _rtP -> P_51 [ 0 ] , 9U * sizeof ( real_T )
) ; memcpy ( & _rtB -> B_24_7929_0 [ 0 ] , & _rtP -> P_103 [ 0 ] , 9U *
sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_24_7930_0 [ 0 ] , & _rtP -> P_104
[ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_24_10983_0 [ 0 ] , &
_rtP -> P_141 [ 0 ] , 9U * sizeof ( real_T ) ) ; memcpy ( & _rtB ->
B_24_10984_0 [ 0 ] , & _rtP -> P_142 [ 0 ] , 9U * sizeof ( real_T ) ) ; _rtB
-> B_24_10988_0 = _rtP -> P_143 ; _rtB -> B_24_10994_0 = _rtP -> P_146 ; _rtB
-> B_24_11000_0 = _rtP -> P_149 ; _rtB -> B_24_11007_0 = _rtP -> P_152 ; _rtB
-> B_24_11013_0 = _rtP -> P_155 ; _rtB -> B_24_11019_0 = _rtP -> P_158 ; _rtB
-> B_24_11026_0 = _rtP -> P_161 ; _rtB -> B_24_11032_0 = _rtP -> P_164 ; _rtB
-> B_24_11038_0 = _rtP -> P_167 ; UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { B_robot_T * _rtB ;
P_robot_T * _rtP ; DW_robot_T * _rtDW ; _rtDW = ( ( DW_robot_T * )
ssGetRootDWork ( S ) ) ; _rtP = ( ( P_robot_T * ) ssGetModelRtp ( S ) ) ;
_rtB = ( ( B_robot_T * ) _ssGetModelBlockIO ( S ) ) ; { real_T * * uBuffer =
( real_T * * ) & _rtDW -> TransportDelay1_PWORK . TUbufferPtrs [ 0 ] ; real_T
* * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay1_PWORK . TUbufferPtrs
[ 1 ] ; real_T simTime = ssGetT ( S ) ; _rtDW -> TransportDelay1_IWORK . Head
= ( ( _rtDW -> TransportDelay1_IWORK . Head < ( _rtDW ->
TransportDelay1_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay1_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay1_IWORK . Head == _rtDW -> TransportDelay1_IWORK . Tail ) { if
( ! robot_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay1_IWORK
. CircularBufSize , & _rtDW -> TransportDelay1_IWORK . Tail , & _rtDW ->
TransportDelay1_IWORK . Head , & _rtDW -> TransportDelay1_IWORK . Last ,
simTime - _rtP -> P_5 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 ,
false , & _rtDW -> TransportDelay1_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
tBuffer ) [ _rtDW -> TransportDelay1_IWORK . Head ] = simTime ; ( * uBuffer )
[ _rtDW -> TransportDelay1_IWORK . Head ] = _rtB -> B_24_2138_0 ; } { real_T
* * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK . TUbufferPtrs [
0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK .
TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK . Head = ( ( _rtDW -> TransportDelay_IWORK . Head < (
_rtDW -> TransportDelay_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK
. Head == _rtDW -> TransportDelay_IWORK . Tail ) { if ( !
robot_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay_IWORK .
CircularBufSize , & _rtDW -> TransportDelay_IWORK . Tail , & _rtDW ->
TransportDelay_IWORK . Head , & _rtDW -> TransportDelay_IWORK . Last ,
simTime - _rtP -> P_7 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 ,
false , & _rtDW -> TransportDelay_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
tBuffer ) [ _rtDW -> TransportDelay_IWORK . Head ] = simTime ; ( * uBuffer )
[ _rtDW -> TransportDelay_IWORK . Head ] = _rtB -> B_24_2144_0 ; } { real_T *
* uBuffer = ( real_T * * ) & _rtDW -> TransportDelay7_PWORK . TUbufferPtrs [
0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay7_PWORK .
TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay7_IWORK . Head = ( ( _rtDW -> TransportDelay7_IWORK . Head < (
_rtDW -> TransportDelay7_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay7_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay7_IWORK . Head == _rtDW -> TransportDelay7_IWORK . Tail ) { if
( ! robot_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay7_IWORK
. CircularBufSize , & _rtDW -> TransportDelay7_IWORK . Tail , & _rtDW ->
TransportDelay7_IWORK . Head , & _rtDW -> TransportDelay7_IWORK . Last ,
simTime - _rtP -> P_9 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 ,
false , & _rtDW -> TransportDelay7_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
tBuffer ) [ _rtDW -> TransportDelay7_IWORK . Head ] = simTime ; ( * uBuffer )
[ _rtDW -> TransportDelay7_IWORK . Head ] = _rtB -> B_24_2150_0 ; } { real_T
* * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay2_PWORK . TUbufferPtrs
[ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay2_PWORK
. TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay2_IWORK . Head = ( ( _rtDW -> TransportDelay2_IWORK . Head < (
_rtDW -> TransportDelay2_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay2_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay2_IWORK . Head == _rtDW -> TransportDelay2_IWORK . Tail ) { if
( ! robot_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay2_IWORK
. CircularBufSize , & _rtDW -> TransportDelay2_IWORK . Tail , & _rtDW ->
TransportDelay2_IWORK . Head , & _rtDW -> TransportDelay2_IWORK . Last ,
simTime - _rtP -> P_11 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 ,
false , & _rtDW -> TransportDelay2_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
tBuffer ) [ _rtDW -> TransportDelay2_IWORK . Head ] = simTime ; ( * uBuffer )
[ _rtDW -> TransportDelay2_IWORK . Head ] = _rtB -> B_24_2120_0 ; } { real_T
* * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay3_PWORK . TUbufferPtrs
[ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay3_PWORK
. TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay3_IWORK . Head = ( ( _rtDW -> TransportDelay3_IWORK . Head < (
_rtDW -> TransportDelay3_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay3_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay3_IWORK . Head == _rtDW -> TransportDelay3_IWORK . Tail ) { if
( ! robot_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay3_IWORK
. CircularBufSize , & _rtDW -> TransportDelay3_IWORK . Tail , & _rtDW ->
TransportDelay3_IWORK . Head , & _rtDW -> TransportDelay3_IWORK . Last ,
simTime - _rtP -> P_13 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 ,
false , & _rtDW -> TransportDelay3_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
tBuffer ) [ _rtDW -> TransportDelay3_IWORK . Head ] = simTime ; ( * uBuffer )
[ _rtDW -> TransportDelay3_IWORK . Head ] = _rtB -> B_24_2126_0 ; } { real_T
* * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay4_PWORK . TUbufferPtrs
[ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay4_PWORK
. TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay4_IWORK . Head = ( ( _rtDW -> TransportDelay4_IWORK . Head < (
_rtDW -> TransportDelay4_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay4_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay4_IWORK . Head == _rtDW -> TransportDelay4_IWORK . Tail ) { if
( ! robot_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay4_IWORK
. CircularBufSize , & _rtDW -> TransportDelay4_IWORK . Tail , & _rtDW ->
TransportDelay4_IWORK . Head , & _rtDW -> TransportDelay4_IWORK . Last ,
simTime - _rtP -> P_15 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 ,
false , & _rtDW -> TransportDelay4_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
tBuffer ) [ _rtDW -> TransportDelay4_IWORK . Head ] = simTime ; ( * uBuffer )
[ _rtDW -> TransportDelay4_IWORK . Head ] = _rtB -> B_24_2132_0 ; } { real_T
* * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay5_PWORK . TUbufferPtrs
[ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay5_PWORK
. TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay5_IWORK . Head = ( ( _rtDW -> TransportDelay5_IWORK . Head < (
_rtDW -> TransportDelay5_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay5_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay5_IWORK . Head == _rtDW -> TransportDelay5_IWORK . Tail ) { if
( ! robot_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay5_IWORK
. CircularBufSize , & _rtDW -> TransportDelay5_IWORK . Tail , & _rtDW ->
TransportDelay5_IWORK . Head , & _rtDW -> TransportDelay5_IWORK . Last ,
simTime - _rtP -> P_17 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 ,
false , & _rtDW -> TransportDelay5_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
tBuffer ) [ _rtDW -> TransportDelay5_IWORK . Head ] = simTime ; ( * uBuffer )
[ _rtDW -> TransportDelay5_IWORK . Head ] = _rtB -> B_24_2102_0 ; } { real_T
* * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay6_PWORK . TUbufferPtrs
[ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay6_PWORK
. TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay6_IWORK . Head = ( ( _rtDW -> TransportDelay6_IWORK . Head < (
_rtDW -> TransportDelay6_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay6_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay6_IWORK . Head == _rtDW -> TransportDelay6_IWORK . Tail ) { if
( ! robot_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay6_IWORK
. CircularBufSize , & _rtDW -> TransportDelay6_IWORK . Tail , & _rtDW ->
TransportDelay6_IWORK . Head , & _rtDW -> TransportDelay6_IWORK . Last ,
simTime - _rtP -> P_19 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 ,
false , & _rtDW -> TransportDelay6_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
tBuffer ) [ _rtDW -> TransportDelay6_IWORK . Head ] = simTime ; ( * uBuffer )
[ _rtDW -> TransportDelay6_IWORK . Head ] = _rtB -> B_24_2108_0 ; } { real_T
* * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay8_PWORK . TUbufferPtrs
[ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay8_PWORK
. TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay8_IWORK . Head = ( ( _rtDW -> TransportDelay8_IWORK . Head < (
_rtDW -> TransportDelay8_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay8_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay8_IWORK . Head == _rtDW -> TransportDelay8_IWORK . Tail ) { if
( ! robot_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay8_IWORK
. CircularBufSize , & _rtDW -> TransportDelay8_IWORK . Tail , & _rtDW ->
TransportDelay8_IWORK . Head , & _rtDW -> TransportDelay8_IWORK . Last ,
simTime - _rtP -> P_21 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 ,
false , & _rtDW -> TransportDelay8_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
tBuffer ) [ _rtDW -> TransportDelay8_IWORK . Head ] = simTime ; ( * uBuffer )
[ _rtDW -> TransportDelay8_IWORK . Head ] = _rtB -> B_24_2114_0 ; } { real_T
* * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_m . TUbufferPtrs
[ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_m
. TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_a . Head = ( ( _rtDW -> TransportDelay_IWORK_a . Head <
( _rtDW -> TransportDelay_IWORK_a . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay_IWORK_a . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK_a . Head == _rtDW -> TransportDelay_IWORK_a . Tail ) {
if ( ! robot_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_a . CircularBufSize , & _rtDW -> TransportDelay_IWORK_a
. Tail , & _rtDW -> TransportDelay_IWORK_a . Head , & _rtDW ->
TransportDelay_IWORK_a . Last , simTime - _rtP -> P_52 , tBuffer , uBuffer ,
( NULL ) , ( boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_a .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , "tdelay memory allocation error" )
; return ; } } ( * tBuffer ) [ _rtDW -> TransportDelay_IWORK_a . Head ] =
simTime ; ( * uBuffer ) [ _rtDW -> TransportDelay_IWORK_a . Head ] = _rtB ->
B_24_5045_0 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay2_PWORK_d . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & _rtDW -> TransportDelay2_PWORK_d . TUbufferPtrs [ 1 ] ; real_T
simTime = ssGetT ( S ) ; _rtDW -> TransportDelay2_IWORK_n . Head = ( ( _rtDW
-> TransportDelay2_IWORK_n . Head < ( _rtDW -> TransportDelay2_IWORK_n .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay2_IWORK_n . Head + 1 ) : 0
) ; if ( _rtDW -> TransportDelay2_IWORK_n . Head == _rtDW ->
TransportDelay2_IWORK_n . Tail ) { if ( !
robot_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay2_IWORK_n .
CircularBufSize , & _rtDW -> TransportDelay2_IWORK_n . Tail , & _rtDW ->
TransportDelay2_IWORK_n . Head , & _rtDW -> TransportDelay2_IWORK_n . Last ,
simTime - _rtP -> P_54 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 ,
false , & _rtDW -> TransportDelay2_IWORK_n . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
tBuffer ) [ _rtDW -> TransportDelay2_IWORK_n . Head ] = simTime ; ( * uBuffer
) [ _rtDW -> TransportDelay2_IWORK_n . Head ] = _rtB -> B_24_5051_0 ; } {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay3_PWORK_n .
TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
TransportDelay3_PWORK_n . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S )
; _rtDW -> TransportDelay3_IWORK_g . Head = ( ( _rtDW ->
TransportDelay3_IWORK_g . Head < ( _rtDW -> TransportDelay3_IWORK_g .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay3_IWORK_g . Head + 1 ) : 0
) ; if ( _rtDW -> TransportDelay3_IWORK_g . Head == _rtDW ->
TransportDelay3_IWORK_g . Tail ) { if ( !
robot_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay3_IWORK_g .
CircularBufSize , & _rtDW -> TransportDelay3_IWORK_g . Tail , & _rtDW ->
TransportDelay3_IWORK_g . Head , & _rtDW -> TransportDelay3_IWORK_g . Last ,
simTime - _rtP -> P_56 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 ,
false , & _rtDW -> TransportDelay3_IWORK_g . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
tBuffer ) [ _rtDW -> TransportDelay3_IWORK_g . Head ] = simTime ; ( * uBuffer
) [ _rtDW -> TransportDelay3_IWORK_g . Head ] = _rtB -> B_24_5057_0 ; } {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay4_PWORK_g .
TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
TransportDelay4_PWORK_g . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S )
; _rtDW -> TransportDelay4_IWORK_p . Head = ( ( _rtDW ->
TransportDelay4_IWORK_p . Head < ( _rtDW -> TransportDelay4_IWORK_p .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay4_IWORK_p . Head + 1 ) : 0
) ; if ( _rtDW -> TransportDelay4_IWORK_p . Head == _rtDW ->
TransportDelay4_IWORK_p . Tail ) { if ( !
robot_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay4_IWORK_p .
CircularBufSize , & _rtDW -> TransportDelay4_IWORK_p . Tail , & _rtDW ->
TransportDelay4_IWORK_p . Head , & _rtDW -> TransportDelay4_IWORK_p . Last ,
simTime - _rtP -> P_58 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 ,
false , & _rtDW -> TransportDelay4_IWORK_p . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
tBuffer ) [ _rtDW -> TransportDelay4_IWORK_p . Head ] = simTime ; ( * uBuffer
) [ _rtDW -> TransportDelay4_IWORK_p . Head ] = _rtB -> B_24_5027_0 ; } {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay5_PWORK_h .
TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
TransportDelay5_PWORK_h . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S )
; _rtDW -> TransportDelay5_IWORK_i . Head = ( ( _rtDW ->
TransportDelay5_IWORK_i . Head < ( _rtDW -> TransportDelay5_IWORK_i .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay5_IWORK_i . Head + 1 ) : 0
) ; if ( _rtDW -> TransportDelay5_IWORK_i . Head == _rtDW ->
TransportDelay5_IWORK_i . Tail ) { if ( !
robot_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay5_IWORK_i .
CircularBufSize , & _rtDW -> TransportDelay5_IWORK_i . Tail , & _rtDW ->
TransportDelay5_IWORK_i . Head , & _rtDW -> TransportDelay5_IWORK_i . Last ,
simTime - _rtP -> P_60 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 ,
false , & _rtDW -> TransportDelay5_IWORK_i . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
tBuffer ) [ _rtDW -> TransportDelay5_IWORK_i . Head ] = simTime ; ( * uBuffer
) [ _rtDW -> TransportDelay5_IWORK_i . Head ] = _rtB -> B_24_5033_0 ; } {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay6_PWORK_j .
TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
TransportDelay6_PWORK_j . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S )
; _rtDW -> TransportDelay6_IWORK_k . Head = ( ( _rtDW ->
TransportDelay6_IWORK_k . Head < ( _rtDW -> TransportDelay6_IWORK_k .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay6_IWORK_k . Head + 1 ) : 0
) ; if ( _rtDW -> TransportDelay6_IWORK_k . Head == _rtDW ->
TransportDelay6_IWORK_k . Tail ) { if ( !
robot_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay6_IWORK_k .
CircularBufSize , & _rtDW -> TransportDelay6_IWORK_k . Tail , & _rtDW ->
TransportDelay6_IWORK_k . Head , & _rtDW -> TransportDelay6_IWORK_k . Last ,
simTime - _rtP -> P_62 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 ,
false , & _rtDW -> TransportDelay6_IWORK_k . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
tBuffer ) [ _rtDW -> TransportDelay6_IWORK_k . Head ] = simTime ; ( * uBuffer
) [ _rtDW -> TransportDelay6_IWORK_k . Head ] = _rtB -> B_24_5039_0 ; } {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay7_PWORK_g .
TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
TransportDelay7_PWORK_g . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S )
; _rtDW -> TransportDelay7_IWORK_e . Head = ( ( _rtDW ->
TransportDelay7_IWORK_e . Head < ( _rtDW -> TransportDelay7_IWORK_e .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay7_IWORK_e . Head + 1 ) : 0
) ; if ( _rtDW -> TransportDelay7_IWORK_e . Head == _rtDW ->
TransportDelay7_IWORK_e . Tail ) { if ( !
robot_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay7_IWORK_e .
CircularBufSize , & _rtDW -> TransportDelay7_IWORK_e . Tail , & _rtDW ->
TransportDelay7_IWORK_e . Head , & _rtDW -> TransportDelay7_IWORK_e . Last ,
simTime - _rtP -> P_64 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 ,
false , & _rtDW -> TransportDelay7_IWORK_e . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
tBuffer ) [ _rtDW -> TransportDelay7_IWORK_e . Head ] = simTime ; ( * uBuffer
) [ _rtDW -> TransportDelay7_IWORK_e . Head ] = _rtB -> B_24_5009_0 ; } {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay8_PWORK_j .
TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
TransportDelay8_PWORK_j . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S )
; _rtDW -> TransportDelay8_IWORK_m . Head = ( ( _rtDW ->
TransportDelay8_IWORK_m . Head < ( _rtDW -> TransportDelay8_IWORK_m .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay8_IWORK_m . Head + 1 ) : 0
) ; if ( _rtDW -> TransportDelay8_IWORK_m . Head == _rtDW ->
TransportDelay8_IWORK_m . Tail ) { if ( !
robot_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay8_IWORK_m .
CircularBufSize , & _rtDW -> TransportDelay8_IWORK_m . Tail , & _rtDW ->
TransportDelay8_IWORK_m . Head , & _rtDW -> TransportDelay8_IWORK_m . Last ,
simTime - _rtP -> P_66 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 ,
false , & _rtDW -> TransportDelay8_IWORK_m . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
tBuffer ) [ _rtDW -> TransportDelay8_IWORK_m . Head ] = simTime ; ( * uBuffer
) [ _rtDW -> TransportDelay8_IWORK_m . Head ] = _rtB -> B_24_5015_0 ; } {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay9_PWORK .
TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & _rtDW ->
TransportDelay9_PWORK . TUbufferPtrs [ 1 ] ; real_T simTime = ssGetT ( S ) ;
_rtDW -> TransportDelay9_IWORK . Head = ( ( _rtDW -> TransportDelay9_IWORK .
Head < ( _rtDW -> TransportDelay9_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW
-> TransportDelay9_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay9_IWORK . Head == _rtDW -> TransportDelay9_IWORK . Tail ) { if
( ! robot_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay9_IWORK
. CircularBufSize , & _rtDW -> TransportDelay9_IWORK . Tail , & _rtDW ->
TransportDelay9_IWORK . Head , & _rtDW -> TransportDelay9_IWORK . Last ,
simTime - _rtP -> P_68 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 ,
false , & _rtDW -> TransportDelay9_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , "tdelay memory allocation error" ) ; return ; } } ( *
tBuffer ) [ _rtDW -> TransportDelay9_IWORK . Head ] = simTime ; ( * uBuffer )
[ _rtDW -> TransportDelay9_IWORK . Head ] = _rtB -> B_24_5021_0 ; }
UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdateTID2 ( SimStruct * S , int_T tid ) { UNUSED_PARAMETER (
tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { ssCallAccelRunBlock ( S , 24 ,
2066 , SS_CALL_MDL_DERIVATIVES ) ; }
#define MDL_PROJECTION
static void mdlProjection ( SimStruct * S ) { ssCallAccelRunBlock ( S , 24 ,
2066 , SS_CALL_MDL_PROJECTION ) ; } static void mdlInitializeSizes (
SimStruct * S ) { ssSetChecksumVal ( S , 0 , 114969092U ) ; ssSetChecksumVal
( S , 1 , 161888628U ) ; ssSetChecksumVal ( S , 2 , 1790514783U ) ;
ssSetChecksumVal ( S , 3 , 1742740468U ) ; { mxArray * slVerStructMat = NULL
; mxArray * slStrMat = mxCreateString ( "simulink" ) ; char slVerChar [ 10 ]
; int status = mexCallMATLAB ( 1 , & slVerStructMat , 1 , & slStrMat , "ver"
) ; if ( status == 0 ) { mxArray * slVerMat = mxGetField ( slVerStructMat , 0
, "Version" ) ; if ( slVerMat == NULL ) { status = 1 ; } else { status =
mxGetString ( slVerMat , slVerChar , 10 ) ; } } mxDestroyArray ( slStrMat ) ;
mxDestroyArray ( slVerStructMat ) ; if ( ( status == 1 ) || ( strcmp (
slVerChar , "9.0" ) != 0 ) ) { return ; } } ssSetOptions ( S ,
SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork ( S ) != sizeof (
DW_robot_T ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file." ) ; } if ( ssGetSizeofGlobalBlockIO ( S
) != sizeof ( B_robot_T ) ) { ssSetErrorStatus ( S ,
"Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file." ) ; } { int ssSizeofParams ;
ssGetSizeofParams ( S , & ssSizeofParams ) ; if ( ssSizeofParams != sizeof (
P_robot_T ) ) { static char msg [ 256 ] ; sprintf ( msg ,
"Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file." ) ; } } _ssSetModelRtp ( S , ( real_T *
) & robot_rtDefaultP ) ; rt_InitInfAndNaN ( sizeof ( real_T ) ) ; } static
void mdlInitializeSampleTimes ( SimStruct * S ) { { SimStruct * childS ;
SysOutputFcn * callSysFcns ; childS = ssGetSFunction ( S , 0 ) ; callSysFcns
= ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 1 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 2 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 3 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 4 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 5 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 6 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 7 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 8 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 9 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 10 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 11 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 12 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 13 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 14 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 15 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 16 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 17 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 18 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 19 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 20 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 21 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 22 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 23 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; } slAccRegPrmChangeFcn ( S , mdlOutputsTID2 ) ; }
static void mdlTerminate ( SimStruct * S ) { }
#include "simulink.c"
