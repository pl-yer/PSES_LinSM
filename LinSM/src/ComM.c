#include "ComM.h"

static NetworkHandleType Channel_mock;
static ComM_ModeType ComMode_mock;

/**
 * @brief Indication of the actual bus mode by the corresponding Bus State Manager. 
 * 
 * ComM shall propagate the indicated state to the users with means of the 
 * RTE andBswM
 * 
 * \param Channel 
 * \param ComMode
*/
void ComM_BusSM_ModeIndication(NetworkHandleType Channel, ComM_ModeType ComMode){
    Channel_mock = Channel;
    ComMode_mock = ComMode;
}


