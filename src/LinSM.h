#ifndef LINSM_H_
#define LINSM_H_ 
/** @req SWS_LinSM_00005 */

/** @req SWS_LinSM_00219 */
#include "Rte_ComM_Type.h"
#include "ComStackTypes.h"
#include "LinIf.h"
#include "Std_Types.h"

#include "LinSMConfigSet.h"

#define LINSM_INIT_SERVICE_ID                   0x01
#define LINSM_GET_CURRENT_COM_MODE_SERVICE_ID   0x11
#define LINSM_REQUEST_COM_MODE_SERVICE_ID       0x12
#define LINSM_SCHEDULE_REQUEST_SERVICE_ID       0x10
#define LINSM_SCHEDULE_REQUEST_CONF_SERVICE_ID  0x20
#define LINSM_WAKEUP_CONF_SERVICE_ID            0x21
#define LINSM_GOTO_SLEEP_CONF_SERVICE_ID        0x22
#define LINSM_MAIN_FUNCTION_SERVICE_ID          0x30

/** @req SWS_LinSM_00053 Development Errors */
#define LINSM_E_UNINIT                  0x00
#define LINSM_E_NONEXISTENT_NETWORK		0x20
#define LINSM_E_PARAMETER				0x30
#define LINSM_E_PARAMETER_POINTER		0x40
#define LINSM_E_INIT_FAILED             0x50

/** @req SWS_LinSM_00224 Runtime Errors */
#define LINSM_E_CONFIRMATION_TIMEOUT 0x00

/** @req SWS_LinSM_00220 */
typedef enum {
    LINSM_FULL_COM  = 0x01,
    LINSM_NO_COM    = 0x02
} LinSM_ModeType; 

typedef enum {
    LINSM_FULL_COMMUNICATION,
    LINSM_NO_COMMUNICATION,
    LINSM_UNINIT,             
    LINSM_INIT,               
    LINSM_RUN_COMMUNICATION,
    LINSM_GOTO_SLEEP
} LinSM_StatusType;

/** @req SWS_LinSM_00221 */
/* Container modelling post-build configuration parameters */
typedef struct {
    const uint32 ConfigParams;
} LinSM_ConfigType;

/* 8.3 LinSM API: This is a list of API calls provided for upper layer modules */
void LinSM_Init(const LinSM_ConfigType* ConfigPtr);
Std_ReturnType LinSM_ScheduleRequest(NetworkHandleType network, LinIf_SchHandleType schedule);
void LinSM_GetVersionInfo(Std_VersionInfoType* versioninfo);
Std_ReturnType LinSM_GetCurrentComMode(NetworkHandleType network, ComM_ModeType* mode);
Std_ReturnType LinSM_RequestComMode(NetworkHandleType network, ComM_ModeType mode);

/* 8.5 LinSM callbacks */
void LinSM_ScheduleRequestConfirmation(NetworkHandleType network, LinIf_SchHandleType schedule);
void LinSM_GotoSleepIndication(NetworkHandleType Channel);
void LinSM_GotoSleepConfirmation(NetworkHandleType network, boolean success);
void LinSM_WakeupConfirmation(NetworkHandleType network, boolean success);

#endif