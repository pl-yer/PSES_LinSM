/**===================================================================================================================*\
  @file LinSM.c

  @brief Lin State Manager model
  
  Implementation of LinSM master node with basic interface.

  @see AUTOSAR_SWS_LINStateManager.pdf
\*====================================================================================================================*/

/** @req SWS_LinSM_00001 */
#include "LinSM.h"    
#include "LinIf.h" 
/** @req SWS_LinSM_00105 */
#include "ComM.h"
/** @req SWS_LinSM_00196 */
#include "BswM_LinSM.h"
/** @req SWS_LinSM_00086 */
#include "Det.h"

#include <stddef.h>

#define NULL_SCHEDULE 0

#define LINIF_CONTROLLER_CNT 1

/*==================[macros]==================================================*/
/* Development error macros. */
#if ( LINSM_DEV_ERROR_DETECT == STD_ON )
#define VALIDATE(_exp,_api,_err ) \
        if( !(_exp) ) { \
          Det_ReportError(MODULE_ID_LINSM,0,_api,_err); \
          return; \
        }

#define VALIDATE_W_RV(_exp,_api,_err,_rv ) \
        if( !(_exp) ) { \
          Det_ReportError(MODULE_ID_LINSM,0,_api,_err); \
          return (_rv); \
        }
#else
#define VALIDATE(_exp,_api,_err )
#define VALIDATE_W_RV(_exp,_api,_err,_rv )
#endif

/*==================[internal data]===========================================*/
/** @req SWS_LinSM_00175 */
/* Timer global values for every channel(network). */
static uint16 ScheduleRequestTimer [LINIF_CONTROLLER_CNT];
static uint16 GoToSleepTimer       [LINIF_CONTROLLER_CNT];
static uint16 WakeUpTimer          [LINIF_CONTROLLER_CNT];

/* LinSM module state */
/** @req SWS_LinSM_00022 */
/** @req SWS_LinSM_00161 */
static LinSM_StatusType LinSMStatus = LINSM_UNINIT;

/** Network states */
static LinSM_StatusType LinSMChannelStatus	[LINIF_CONTROLLER_CNT];
static LinIf_SchHandleType LinSMSchTablCurr[LINIF_CONTROLLER_CNT], LinSMSchTablNew[LINIF_CONTROLLER_CNT];

/*==================[API services]============================================*/
/**
 * @brief LinSM Init function
 * This function initializes the LinSM
 * @req SWS_LinSM_00155
 *  
 * \param ConfigPtr Pointer to the LinSM post-build configuration data 
*/
void LinSM_Init(const LinSM_ConfigType* ConfigPtr){
    /** @req SWS_LinSM_00151 */

    VALIDATE( (ConfigPtr!=NULL), LINSM_INIT_SERVICE_ID, LINSM_E_PARAMETER_POINTER );
	uint8 i;

	for (i=0; i < LINIF_CONTROLLER_CNT; i++)
	{
		/** @req SWS_LinSM_00152 */
		/** @req SWS_LinSM_00160 */
		LinSMChannelStatus[i] = LINSM_NO_COMMUNICATION;
		/** @req SWS_LinSM_00043 */
		/** @req SWS_LinSM_00216 */
		LinSMSchTablCurr[i] = NULL_SCHEDULE; 
		LinSMSchTablNew[i] = 0;
		ScheduleRequestTimer[i] = 0;
		GoToSleepTimer[i] = 0;
		WakeUpTimer[i] = 0;
	}
	LinSMStatus = LINSM_INIT;
}

/**
 * @brief The upper layer requests a schedule table to be changed on one LIN network
 * @req SWS_LinSM_00113
 * 
 * \param Network Identification of the LIN channel
 * \param Schedule Pointer to the new Schedule table
 * \return Std_ReturnType 
*/
Std_ReturnType LinSM_ScheduleRequest(NetworkHandleType network, LinIf_SchHandleType schedule){

    Std_ReturnType rv;

    /** @req SWS_LinSM_00114 */
    VALIDATE_W_RV( (network < LINIF_CONTROLLER_CNT), LINSM_SCHEDULE_REQUEST_SERVICE_ID, LINSM_E_NONEXISTENT_NETWORK, E_NOT_OK);
    /** @req SWS_LinSM_00115 */
    VALIDATE_W_RV( (schedule < LINIF_SCH_CNT), LINSM_SCHEDULE_REQUEST_SERVICE_ID, LINSM_E_PARAMETER, E_NOT_OK);
    /** @req SWS_LinSM_00116 */
    VALIDATE_W_RV( (LinSMStatus != LINSM_UNINIT), LINSM_SCHEDULE_REQUEST_SERVICE_ID, LINSM_E_UNINIT, E_NOT_OK);
    /** @req SWS_LinSM_10211 */
	if (LinSMChannelStatus[network] != LINSM_FULL_COM){
		return E_NOT_OK;
	} else {
        /** @req SWS_LinSM_00079 */
        /** @req SWS_LinSM_00168 */
        ScheduleRequestTimer[network] = LINSM_SCHEDULE_REQUEST_TIMEOUT;
        rv = LinIf_ScheduleRequest(network, schedule);
        if (rv != E_OK) {
            /** @req SWS_LinSM_00213 */
            BswM_LinSM_CurrentSchedule(network, LinSMSchTablCurr[network]);
            ScheduleRequestTimer[network] = 0;
        }
        return rv;
    }
}

/**
 * @brief Function returns the configuration information.
 * @req SWS_LinSM_00117 
 * 
 * \param VersionInfo Pointer to where to store the version information of this module
*/
void LinSM_GetVersionInfo(Std_VersionInfoType *versioninfo){
    /* @req SWS_LinSM_00119 */	
    VALIDATE( (versioninfo != NULL), LINSM_SCHEDULE_REQUEST_SERVICE_ID, LINSM_E_PARAMETER_POINTER );
    versioninfo->vendorID = LIN_VENDOR_ID;
    versioninfo->moduleID = LIN_MODULE_ID;
    versioninfo->sw_major_version = LIN_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = LIN_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = LIN_SW_PATCH_VERSION;
}

/**
 * @brief Function to query the current communication mode
 * @req SWS_LinSM_00122
 * 
 * \param Network Identification of the LIN channel
 * \param Mode Returns the active mode, see ComM_ModeType for descriptions of the modes
 * \return Std_ReturnType 
*/
Std_ReturnType LinSM_GetCurrentComMode(NetworkHandleType network, ComM_ModeType* mode){
    /* @req SWS_LinSM_00123 */
	VALIDATE_W_RV((network < LINIF_CONTROLLER_CNT), LINSM_GET_CURRENT_COM_MODE_SERVICE_ID, LINSM_E_NONEXISTENT_NETWORK, E_NOT_OK);
	/* @req SWS_LinSM_00124 */
	VALIDATE_W_RV((mode != NULL), LINSM_GET_CURRENT_COM_MODE_SERVICE_ID, LINSM_E_PARAMETER_POINTER, E_NOT_OK);

	switch (LinSMStatus){
		/* @req SWS_LinSM_00182 */
        case LINSM_UNINIT:
            *mode = COMM_NO_COMMUNICATION;
            /* @req SWS_LinSM_00125 */
            Det_ReportError(MODULE_ID_LINSM, 0, LINSM_GET_CURRENT_COM_MODE_SERVICE_ID, LINSM_E_UNINIT);
            return E_NOT_OK;
            break;
        /* @req SWS_LinSM_00181 */
        case LINSM_FULL_COMMUNICATION:
            *mode = COMM_FULL_COMMUNICATION;
            break;
        /* @req SWS_LinSM_00180 */
        case LINSM_NO_COMMUNICATION:
            *mode = COMM_NO_COMMUNICATION;
            break;
        default:
        // undefined state
            *mode = COMM_NO_COMMUNICATION;
            break;
	}
	return E_OK;
}

/**
 * @brief Requesting of a communication mode.
 * The mode switch will not be made instant. The LinSM will notify the caller when
 * mode transition is made
 * @req SWS_LinSM_00126
 * 
 * \param Network Identification of the LIN channel
 * \param Mode Request mode
 * \return Std_ReturnType 
*/
Std_ReturnType LinSM_RequestComMode(NetworkHandleType network, ComM_ModeType mode){
    Std_ReturnType res = E_NOT_OK;
    /* @req SWS_LinSM_00223 */
    ComM_ModeType mode_mem;
    
    /* @req SWS_LinSM_00127 */
	VALIDATE_W_RV((network < LINIF_CONTROLLER_CNT), LINSM_REQUEST_COM_MODE_SERVICE_ID, LINSM_E_NONEXISTENT_NETWORK, E_NOT_OK);
	/* @req SWS_LinSM_00191 */
	VALIDATE_W_RV((mode <= COMM_FULL_COMMUNICATION), LINSM_REQUEST_COM_MODE_SERVICE_ID, LINSM_E_PARAMETER, E_NOT_OK);
	/* @req SWS_LinSM_00128 */
	VALIDATE_W_RV((LinSMStatus != LINSM_UNINIT), LINSM_REQUEST_COM_MODE_SERVICE_ID, LINSM_E_UNINIT, E_NOT_OK);

    switch (mode){
        case COMM_SILENT_COMMUNICATION:
            /* @req SWS_LinSM_00183 */
            return E_NOT_OK;
        case COMM_NO_COMMUNICATION:
            if (LinIf_GotoSleep(network) == E_OK){
                LinSMChannelStatus[network] = LINSM_GOTO_SLEEP;
                GoToSleepTimer[network] = LINSM_GOTO_SLEEP_TIMEOUT;
                res = E_OK;
                mode_mem = mode;
            } break;
        case COMM_FULL_COMMUNICATION:
            WakeUpTimer[network] = LINSM_WAKEUP_TIMEOUT; //should be done here since some implementations will confirm immediatly
            if (LinIf_WakeUp(network) == E_OK){
                res = E_OK;
                mode_mem = mode;
            } else {
                WakeUpTimer[network] = 0;
            } break;
	}
    return res;
}

/**
 * @brief Periodic function that runs the timers of different request timeouts
 * 
 * (available via SchM_LinSM.h)
 * @req SWS_LinSM_00156
*/
void LinSM_MainFunction(void){
	/** @req SWS_LinSM_00157 Timers Handling */
}

/**
 * @brief The LinIf module will call this callback when the new requested schedule table is
 * active
 * @req SWS_LinSM_00129
 * 
 * \param Network Identification of the LIN channel
 * \param Schedule Pointer to the new Schedule table
*/
void LinSM_ScheduleRequestConfirmation(NetworkHandleType network, LinIf_SchHandleType schedule){
    /* @req SWS_LinSM_00242 */
	/* @req SWS_LinSM_00130 */
	VALIDATE((network < LINIF_CONTROLLER_CNT), LINSM_SCHEDULE_REQUEST_CONF_SERVICE_ID, LINSM_E_NONEXISTENT_NETWORK);
	/* @req SWS_LinSM_00131 */
	VALIDATE((LinSMStatus != LINSM_UNINIT), LINSM_SCHEDULE_REQUEST_CONF_SERVICE_ID, LINSM_E_UNINIT);

	if (ScheduleRequestTimer[network] != 0){
		BswM_LinSM_CurrentSchedule(network, schedule);
        ScheduleRequestTimer[0] = 0;
	}
}

/**
 * @brief The LinIf will call this callback when the go to sleep command is received on the
 * network or a bus idle timeout occurs.
 * Only applicable for LIN slave nodes - NOT IMPLEMENTED as node config is master
 * @req SWS_LinSM_91000
 * 
 * \param Channel Identification of the LIN channel
*/
void LinSM_GotoSleepIndication(NetworkHandleType channel){
    /* @req SWS_LinSM_00243 */
    /* @req SWS_LinSM_00239 */
    VALIDATE((channel < LINIF_CONTROLLER_CNT), LINSM_SCHEDULE_REQUEST_CONF_SERVICE_ID, LINSM_E_NONEXISTENT_NETWORK);
    /* @req SWS_LinSM_00240 */
    VALIDATE((LinSMStatus != LINSM_UNINIT), LINSM_SCHEDULE_REQUEST_CONF_SERVICE_ID, LINSM_E_UNINIT);
}

/**
 * @brief The LinIf will call this callback when the go to sleep command is sent successfully
 * or not sent successfully on the network
 * @req SWS_LinSM_00135
 * 
 * \param Network Identification of the LIN channel
 * \param Success True if goto sleep was successfully sent, false otherwise
*/
void LinSM_GotoSleepConfirmation(NetworkHandleType network, boolean success){
    /* @req SWS_LinSM_00137 */
	VALIDATE((LinSMStatus != LINSM_UNINIT), LINSM_GOTO_SLEEP_CONF_SERVICE_ID, LINSM_E_UNINIT);
	/* @req SWS_LinSM_00136 */
	VALIDATE((network < LINIF_CONTROLLER_CNT), LINSM_GOTO_SLEEP_CONF_SERVICE_ID, LINSM_E_NONEXISTENT_NETWORK);

	if (TRUE == success){
		if (GoToSleepTimer[network] != 0){
			GoToSleepTimer[network] = 0;
			/* @req SWS_LinSM_00035 */
			LinSMChannelStatus[network] = LINSM_NO_COMMUNICATION;
			/* @req SWS_LinSM_00027 */
			/* @req SWS_LinSM_00193 */
			ComM_BusSM_ModeIndication(network, COMM_NO_COMMUNICATION);
		}
	}
}

/**
 * @brief The LinIf will call this callback when the wake up signal command is sent not
 * successfully/successfully on the network
 * @req SWS_LinSM_00132
 * 
 * \param Network Identification of the LIN channel
 * \param Success True if wakeup was successfully sent, false otherwise
*/
void LinSM_WakeUpConfirmation(NetworkHandleType network, boolean success){

}
