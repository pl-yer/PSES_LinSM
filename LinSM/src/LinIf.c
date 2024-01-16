/**===================================================================================================================*\
  @file LinIf.c

  @brief Lin Interface module
  
  Model of LinIf module.

  @see AUTOSAR_SWS_LINStateManager.pdf
\*====================================================================================================================*/

#include "LinIf.h"

/**
 * @brief Initiates a transition into the Sleep Mode on the selected channel
 * 
 * \param Channel 
*/
Std_ReturnType LinIf_GotoSleep(NetworkHandleType Channel){
    
    return E_OK;
}

/**
 * @brief Requests a schedule table to be executed
 * Only used for LIN master nodes.
 * 
 * \param Network
 * \param Schedule 
*/
Std_ReturnType LinIf_ScheduleRequest(NetworkHandleType Network, LinIf_SchHandleType Schedule){

    return E_OK;
}

/**
 * @brief Initiates the wake up process
 * 
 * \param Channel
*/
Std_ReturnType LinIf_WakeUp(NetworkHandleType Channel){

  return E_OK;
}
