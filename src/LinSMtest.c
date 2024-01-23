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

FAKE_VALUE_FUNC(Std_ReturnType, Det_ReportError, uint16, uint8, uint8, uint8);
FAKE_VOID_FUNC(BswM_LinSM_CurrentSchedule, NetworkHandleType, LinIf_SchHandleType);
FAKE_VALUE_FUNC(Std_ReturnType, LinIf_ScheduleRequest, NetworkHandleType, LinIf_SchHandleType);


// int main (int argc, char **argv) {
//     printf("Hello World!");
// }


TEST_LIST = { 
    { NULL, NULL }                                     
};
    