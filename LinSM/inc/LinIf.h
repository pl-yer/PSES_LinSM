#ifndef LINIF_H
#define LINIF_H

#include "Std_Types.h"
#include "ComStackTypes.h"

typedef uint8 LinIf_SchHandleType;

/** @req SWS_LinSM_00229 */
Std_ReturnType LinIf_GotoSleep(NetworkHandleType Channel);
Std_ReturnType LinIf_WakeUp(NetworkHandleType Channel);
Std_ReturnType LinIf_ScheduleRequest(NetworkHandleType network, LinIf_SchHandleType schedule);


void resetLinIf_ScheduleRequest(void);
int getLinIfScheduleRequestCallCounter(void);

#endif