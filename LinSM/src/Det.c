/**===================================================================================================================*\
  @file Det.c

  @brief Default Error Tracer module
  
  Model of DET module needed by LinSM master node.

  @see AUTOSAR_SWS_DefaultErrorTracer.pdf
\*====================================================================================================================*/

#include "Det.h"

/**
 * @brief Service to report runtime errors. 
 * If a callout has been configured then this callout shall be called
 * 
 * \param ModuleId 
 * \param Instance_Id
 * \param ApiId
 * \param ErrorId
*/
Std_ReturnType Det_ReportRuntimeError (uint16 ModuleId, uint8 Instance_Id, uint8 ApiId, uint8 ErrorId){
    
    return E_OK;
}

