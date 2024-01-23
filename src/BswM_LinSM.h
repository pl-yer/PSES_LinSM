#ifndef BSWM_LINSM_H
#define BSWM_LINSM_H

#include "Std_Types.h"
#include "ComStackTypes.h"
#include "LinIf.h"
#include "LinSM.h"

/** @req SWS_LinSM_00229 */
void BswM_LinSM_CurrentSchedule(NetworkHandleType Network,LinIf_SchHandleType CurrentSchedule);
void BswM_LinSM_CurrentState(NetworkHandleType Network,LinSM_ModeType CurrentState);

#endif
