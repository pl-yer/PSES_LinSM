/** ==================================================================================================================*\
  @file LinSMtest.c

  @brief Unit tests of LinSM
\*====================================================================================================================*/
#include "Std_Types.h"

#include <stdio.h>
#include "acutest.h"
#include "fff.h"

#include "LinSM.c"

DEFINE_FFF_GLOBALS;

/*==================[Fake functions]============================================*/
FAKE_VALUE_FUNC(Std_ReturnType, Det_ReportError, uint16, uint8, uint8, uint8);
FAKE_VOID_FUNC(BswM_LinSM_CurrentSchedule, NetworkHandleType, LinIf_SchHandleType);
FAKE_VALUE_FUNC(Std_ReturnType, LinIf_ScheduleRequest, NetworkHandleType, LinIf_SchHandleType);

/*==================[ Custom fakes ]============================================*/


/*==================[Test functions]============================================*/
/**
 * @brief LinSM_ScheduleRequest tests
 * 
*/
void Test_LinSM_ScheduleRequest(void) {

    Std_ReturnType rv;
    Std_ReturnType SequenceBuffer [2] = {E_OK, E_NOT_OK};

    NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	LinIf_SchHandleType schedule = LINIF_SCH_CNT - 1;
    LinSMStatus = LINSM_INIT;
    LinSMChannelStatus[0] = LINSM_NO_COM;
    rv = LinSM_ScheduleRequest(network, schedule);
    TEST_CHECK(rv == E_NOT_OK);

    LinSMStatus = LINSM_INIT;
    LinSMChannelStatus[0] = LINSM_FULL_COM;
    rv = LinSM_ScheduleRequest(network, schedule);
    TEST_CHECK(rv == E_OK);

    LinSMStatus = LINSM_UNINIT;
    LinSMChannelStatus[0] = LINSM_FULL_COM;
    rv = LinSM_ScheduleRequest(network, schedule);
    TEST_CHECK(rv == E_NOT_OK);

    LinSMStatus = LINSM_UNINIT;
    LinSMChannelStatus[0] = LINSM_NO_COM;
    rv = LinSM_ScheduleRequest(network, schedule);
    TEST_CHECK(rv == E_NOT_OK);

    network = LINIF_CONTROLLER_CNT;
    schedule = LINIF_SCH_CNT - 1;
    LinSMStatus = LINSM_INIT;
    LinSMChannelStatus[0] = LINSM_FULL_COM;
    rv = LinSM_ScheduleRequest(network, schedule);
    TEST_CHECK(rv == E_NOT_OK);

    network = LINIF_CONTROLLER_CNT - 1;
    schedule = LINIF_SCH_CNT;
    LinSMStatus = LINSM_INIT;
    LinSMChannelStatus[0] = LINSM_FULL_COM;
    rv = LinSM_ScheduleRequest(network, schedule);
    TEST_CHECK(rv == E_NOT_OK);


    SET_RETURN_SEQ(LinIf_ScheduleRequest, SequenceBuffer, 2);
    network = LINIF_CONTROLLER_CNT - 1;
    schedule = LINIF_SCH_CNT - 1;
    LinSMStatus = LINSM_INIT;
    LinSMChannelStatus[0] = LINSM_FULL_COM;
    rv = LinSM_ScheduleRequest(network, schedule);
    TEST_CHECK(rv == E_OK);
    TEST_CHECK(BswM_LinSM_CurrentSchedule_fake.call_count == 0);

    network = LINIF_CONTROLLER_CNT - 1;
    schedule = LINIF_SCH_CNT - 1;
    LinSMStatus = LINSM_INIT;
    LinSMChannelStatus[0] = LINSM_FULL_COM;
    rv = LinSM_ScheduleRequest(network, schedule);
    TEST_CHECK(rv == E_NOT_OK);
    TEST_CHECK(BswM_LinSM_CurrentSchedule_fake.call_count == 1);
}

TEST_LIST = { 
    { "Test of LinSM_ScheduleRequest", Test_LinSM_ScheduleRequest },
    { NULL, NULL }                                     
};
    