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

FAKE_VALUE_FUNC(Std_ReturnType, LinIf_GotoSleep, NetworkHandleType);
FAKE_VALUE_FUNC(Std_ReturnType, LinIf_WakeUp, NetworkHandleType);

FAKE_VOID_FUNC(ComM_BusSM_ModeIndication, NetworkHandleType, ComM_ModeType);

/*==================[ Custom fakes ]============================================*/
NetworkHandleType Network_mock = 0xFFU;
LinIf_SchHandleType Schedule_mock = 0xFFU;
ComM_ModeType ComMode_mock = 0xFFU;

void Spoof_BswM_LinSM_CurrentSchedule(NetworkHandleType network, LinIf_SchHandleType schedule){
    Network_mock = network;
    Schedule_mock = schedule;
}

void Spoof_ComM_BusSM_ModeIndication(NetworkHandleType network, ComM_ModeType ComMode){
    Network_mock = network;
    ComMode_mock = ComMode;
}
/*==================[Test functions]============================================*/
/**
 * @brief LinSM_Init tests
*/
void Test_LinSM_Init(void) {
    LinSM_ConfigType CfgType;
    CfgType.ConfigParams = 0;

    LinSM_Init(&CfgType);
    TEST_CHECK(LinSMChannelStatus[LINIF_CONTROLLER_CNT-1] == LINSM_NO_COMMUNICATION);
    TEST_CHECK(ScheduleRequestTimer[LINIF_CONTROLLER_CNT-1] == 0);
    TEST_CHECK(GoToSleepTimer[LINIF_CONTROLLER_CNT-1] == 0);
    TEST_CHECK(WakeUpTimer[LINIF_CONTROLLER_CNT-1] == 0);
    TEST_CHECK(LinSMSchTablCurr[LINIF_CONTROLLER_CNT-1] == NULL_SCHEDULE);
    TEST_CHECK(LinSMSchTablNew[LINIF_CONTROLLER_CNT-1] == 0);
    TEST_CHECK(LinSMStatus == LINSM_INIT);

    LinSM_Init(0);
    TEST_CHECK(Det_ReportError_fake.call_count == 1);
}

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

    Det_ReportError_fake.call_count = 0;
    network = LINIF_CONTROLLER_CNT + 1;
    LinSMStatus = LINSM_INIT;
    LinSMChannelStatus[0] = LINSM_FULL_COM;
    rv = LinSM_ScheduleRequest(network, schedule);
    TEST_CHECK(Det_ReportError_fake.call_count == 1);
}

/** 
 * @brief LinSM_GetVersionInfo tests
*/
void Test_LinSM_GetVersionInfo(void) {
    Std_VersionInfoType versioninfo = { 0, 0, 0, 0, 0 };

    LinSM_GetVersionInfo(0);
    TEST_CHECK(Det_ReportError_fake.call_count == 1);

    LinSM_GetVersionInfo(&versioninfo);
    TEST_CHECK(versioninfo.vendorID == 1);
	TEST_CHECK(versioninfo.moduleID == 11);
	TEST_CHECK(versioninfo.sw_major_version == 21);
	TEST_CHECK(versioninfo.sw_minor_version == 31);
	TEST_CHECK(versioninfo.sw_patch_version == 41);
}

/** 
 * @brief LinSM_GetCurrentComMode tests
*/
void Test_LinSM_GetCurrentComMode(void) {
    Std_ReturnType rv;
	ComM_ModeType mode;

    NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
    LinSMStatus = LINSM_INIT;
    rv = LinSM_GetCurrentComMode(network, &mode);
    TEST_CHECK(rv == E_OK);

    network = LINIF_CONTROLLER_CNT - 1;
    LinSMStatus = LINSM_UNINIT;
    rv = LinSM_GetCurrentComMode(network, &mode);
    TEST_CHECK(rv == E_NOT_OK);
    TEST_CHECK(mode == COMM_NO_COMMUNICATION);

    network = LINIF_CONTROLLER_CNT - 1;
    LinSMStatus = LINSM_NO_COMMUNICATION;
    rv = LinSM_GetCurrentComMode(network, &mode);
    TEST_CHECK(rv == E_OK);
    TEST_CHECK(mode == COMM_NO_COMMUNICATION);

    network = LINIF_CONTROLLER_CNT - 1;
    LinSMStatus = LINSM_FULL_COMMUNICATION;
    rv = LinSM_GetCurrentComMode(network, &mode);
    TEST_CHECK(rv == E_OK);
    TEST_CHECK(mode == COMM_FULL_COMMUNICATION);

    network = LINIF_CONTROLLER_CNT - 1;
    LinSMStatus = LINSM_GOTO_SLEEP;
    rv = LinSM_GetCurrentComMode(network, &mode);
    TEST_CHECK(rv == E_OK);
    TEST_CHECK(mode == COMM_NO_COMMUNICATION);

    Det_ReportError_fake.call_count = 0;
    network = LINIF_CONTROLLER_CNT + 1;
    rv = LinSM_GetCurrentComMode(network, &mode);
    TEST_CHECK(rv == E_NOT_OK);
    TEST_CHECK(Det_ReportError_fake.call_count == 1);
}

/**
 * @brief LinSM_RequestComMode tests
*/
void Test_LinSM_RequestComMode(void) {
    Std_ReturnType rv;
    Std_ReturnType SequenceBuffer [2] = {E_OK, E_NOT_OK};

    NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
	ComM_ModeType mode = COMM_FULL_COMMUNICATION;
    LinSMStatus = LINSM_UNINIT;
    rv = LinSM_RequestComMode(network, mode);
    TEST_CHECK(rv == E_NOT_OK);
    TEST_CHECK(Det_ReportError_fake.call_count == 1);

    network = LINIF_CONTROLLER_CNT - 1;
	mode = COMM_FULL_COMMUNICATION;
    LinSMStatus = LINSM_INIT;
    rv = LinSM_RequestComMode(network, mode);
    TEST_CHECK(rv == E_OK);

    Det_ReportError_fake.call_count = 0;
    network = LINIF_CONTROLLER_CNT;
	mode = COMM_FULL_COMMUNICATION;
    LinSMStatus = LINSM_INIT;
    rv = LinSM_RequestComMode(network, mode);
    TEST_CHECK(rv == E_NOT_OK);
    TEST_CHECK(Det_ReportError_fake.call_count == 1);

    Det_ReportError_fake.call_count = 0;
    network = LINIF_CONTROLLER_CNT - 1;
	mode = COMM_SILENT_COMMUNICATION;
    LinSMStatus = LINSM_INIT;
    rv = LinSM_RequestComMode(network, mode);
    TEST_CHECK(rv == E_NOT_OK);
    TEST_CHECK(Det_ReportError_fake.call_count == 0);

    Det_ReportError_fake.call_count = 0;
    network = LINIF_CONTROLLER_CNT - 1;
	mode = COMM_NO_COMMUNICATION;
    LinSMStatus = LINSM_INIT;
    SET_RETURN_SEQ(LinIf_GotoSleep, SequenceBuffer, 2);
    rv = LinSM_RequestComMode(network, mode);
    TEST_CHECK(rv == E_OK);
    rv = LinSM_RequestComMode(network, mode);
    TEST_CHECK(rv == E_NOT_OK);

    mode = COMM_FULL_COMMUNICATION;
    SET_RETURN_SEQ(LinIf_WakeUp, SequenceBuffer, 2);
    rv = LinSM_RequestComMode(network, mode);
    TEST_CHECK(rv == E_OK);
    rv = LinSM_RequestComMode(network, mode);
    TEST_CHECK(rv == E_NOT_OK);
}

/**
 * @brief LinSM_ScheduleRequestConfirmation tests
*/
void Test_LinSM_ScheduleRequestConfirmation(void) {
    BswM_LinSM_CurrentSchedule_fake.custom_fake = Spoof_BswM_LinSM_CurrentSchedule;
    
    LinSMStatus = LINSM_UNINIT;
    NetworkHandleType network = LINIF_CONTROLLER_CNT - 1;
    LinIf_SchHandleType schedule = LINIF_SCH_CNT - 1;
    ScheduleRequestTimer[network] = 0;
    LinSM_ScheduleRequestConfirmation(network, schedule);
    TEST_CHECK(Det_ReportError_fake.call_count == 1);
    TEST_CHECK(BswM_LinSM_CurrentSchedule_fake.call_count == 0);

    Det_ReportError_fake.call_count = 0;
    network = LINIF_CONTROLLER_CNT;
    LinSM_ScheduleRequestConfirmation(network, schedule);
    TEST_CHECK(Det_ReportError_fake.call_count == 1);
    TEST_CHECK(BswM_LinSM_CurrentSchedule_fake.call_count == 0);

    Det_ReportError_fake.call_count = 0;
    LinSMStatus = LINSM_INIT;
    network = LINIF_CONTROLLER_CNT - 1;
    ScheduleRequestTimer[network] = 1;
    LinSM_ScheduleRequestConfirmation(network, schedule);
    TEST_CHECK(Det_ReportError_fake.call_count == 0);
    TEST_CHECK(BswM_LinSM_CurrentSchedule_fake.call_count == 1);
    TEST_CHECK(Network_mock == network);
    TEST_CHECK(Schedule_mock == schedule);
    TEST_CHECK(ScheduleRequestTimer[network] == 0);
}

/**
 * @brief LinSM_GotoSleepIndication tests
*/
void Test_LinSM_GotoSleepIndication(void) {

    NetworkHandleType network = LINIF_CONTROLLER_CNT;
    LinSM_GotoSleepIndication(network);
    TEST_CHECK(Det_ReportError_fake.call_count == 1);

    Det_ReportError_fake.call_count = 0;
    network = LINIF_CONTROLLER_CNT - 1;
    LinSMStatus = LINSM_UNINIT;
    LinSM_GotoSleepIndication(network);
    TEST_CHECK(Det_ReportError_fake.call_count == 1);

    Det_ReportError_fake.call_count = 0;
    network = LINIF_CONTROLLER_CNT - 1;
    LinSMStatus = LINSM_INIT;
    LinSM_GotoSleepIndication(network);
    TEST_CHECK(Det_ReportError_fake.call_count == 0);
}

/** 
 * @brief LinSM_GotoSleepConfirmation tests
*/
void Test_LinSM_GotoSleepConfirmation(void) {
    ComM_BusSM_ModeIndication_fake.custom_fake = Spoof_ComM_BusSM_ModeIndication;

    NetworkHandleType network = LINIF_CONTROLLER_CNT;
    boolean success = TRUE;
    LinSMStatus = LINSM_UNINIT;
    LinSM_GotoSleepConfirmation(network, success);
    TEST_CHECK(Det_ReportError_fake.call_count == 1);

    Det_ReportError_fake.call_count = 0;
    network = LINIF_CONTROLLER_CNT - 1;
    LinSMStatus = LINSM_UNINIT;
    LinSM_GotoSleepConfirmation(network, success);
    TEST_CHECK(Det_ReportError_fake.call_count == 1);

    Det_ReportError_fake.call_count = 0;
    network = LINIF_CONTROLLER_CNT - 1;
    LinSMStatus = LINSM_INIT;
    success = FALSE;
    LinSM_GotoSleepConfirmation(network, success);
    TEST_CHECK(Det_ReportError_fake.call_count == 0);
    TEST_CHECK(ComM_BusSM_ModeIndication_fake.call_count == 0);
    TEST_CHECK(Network_mock != network);
    TEST_CHECK(ComMode_mock != COMM_NO_COMMUNICATION);

    Det_ReportError_fake.call_count = 0;
    network = LINIF_CONTROLLER_CNT - 1;
    LinSMStatus = LINSM_INIT;
    success = TRUE;
    GoToSleepTimer[network] == 1;
    LinSM_GotoSleepConfirmation(network, success);
    TEST_CHECK(Det_ReportError_fake.call_count == 0);
    TEST_CHECK(ComM_BusSM_ModeIndication_fake.call_count == 1);
    // TEST_CHECK(LinSMChannelStatus[network] == LINSM_NO_COMMUNICATION);
    // TEST_CHECK(Network_mock == network);
    // TEST_CHECK(ComMode_mock == COMM_NO_COMMUNICATION);
}

TEST_LIST = { 
    { "Test of LinSM_Init", Test_LinSM_Init },
    { "Test of LinSM_ScheduleRequest", Test_LinSM_ScheduleRequest },
    { "Test of LinSM_GetVersion", Test_LinSM_GetVersionInfo },
    { "Test of LinSM_GetCurrentComMode", Test_LinSM_GetCurrentComMode },
    { "Test of LinSM_RequestComMode", Test_LinSM_RequestComMode },
    { "Test of LinSM_ScheduleRequestConfirmation", Test_LinSM_ScheduleRequestConfirmation },
    { "Test of LinSM_GotoSleepIndication", Test_LinSM_GotoSleepIndication },
    { "Test of LinSM_GotoSleepConfirmation", Test_LinSM_GotoSleepConfirmation },
    { NULL, NULL }                                     
};
    