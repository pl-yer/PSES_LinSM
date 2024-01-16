#ifndef COMM_H
#define COMM_H

#include "Std_Types.h"
#include "Rte_ComM_Type.h"
#include "ComStackTypes.h"

/** @req SWS_LinSM_00229 */
void ComM_BusSM_ModeIndication(NetworkHandleType Channel, ComM_ModeType ComMode);
/* Only applicable for LIN slave node. 
void ComM_BusSM_BusSleepMode()
*/

#endif
