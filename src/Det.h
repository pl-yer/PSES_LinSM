#ifndef DET_H
#define DET_H

#include "Std_Types.h"

/** @req SWS_LinSM_00229 */
Std_ReturnType Det_ReportError (uint16 ModuleId, uint8 Instance_Id, uint8 ApiId, uint8 ErrorId);
Std_ReturnType Det_ReportRuntimeError (uint16 ModuleId, uint8 Instance_Id, uint8 ApiId, uint8 ErrorId);

#endif