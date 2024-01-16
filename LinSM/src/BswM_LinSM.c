#include "BswM_LinSM.h"

static NetworkHandleType Network_mock;
static LinIf_SchHandleType CurrentSchedule_mock;

/**
 * @brief Function called by LinSM to indicate the currently active schedule table for a specific LIN channel
 * 
 * \param Network 
 * \param CurrentSchedule
*/
void BswM_LinSM_CurrentSchedule(NetworkHandleType Network, LinIf_SchHandleType CurrentSchedule){
    Network_mock = Network;
    CurrentSchedule_mock = CurrentSchedule;
}

/**
 * @brief Function called by LinSM to indicate its current state
 * 
 * \param Network 
 * \param CurrentSchedule
*/
void BswM_LinSM_CurrentState(NetworkHandleType Network, LinSM_ModeType CurrentState){
    (void)Network;
    (void)CurrentState;
}

