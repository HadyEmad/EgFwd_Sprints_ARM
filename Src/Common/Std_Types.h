/******************************************************************************
 *
 * File Name: Std_types.h
 *
 * Description: This file specifies the AUTOSAR standard types header file.
 *
 * Authors: Hady Emad
 *
 *******************************************************************************/

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

#include "Compiler.h"
#include "Platform_Types.h"
/*
 * Name: E_OK, E_NOT_OK
 * SWS_Item: SWS_Std_00006
 */
#ifndef STATUSTYPEDEFINED
#define STATUSTYPEDEFINED
#define E_OK 0x00u
typedef unsigned char StatusType; /* OSEK compliance */
#endif
#define E_NOT_OK 0x01u

/*
 * Name: STD_HIGH, STD_LOW
 * SWS_Item: SWS_Std_00007
 */
#define STD_HIGH 0x01u /* Physical state 5V or 3.3V */
#define STD_LOW 0x00u  /* Physical state 0V */

/*
 * Name: STD_ACTIVE, STD_IDLE
 * SWS_Item: SWS_Std_00013
 */
#define STD_ACTIVE 0x01u /* Logical state active */
#define STD_IDLE 0x00u /* Logical state idle */

/*
 * Name: STD_ON, STD_OFF
 * SWS_Item: SWS_Std_00010
 */
#define STD_ON 0x01u
#define STD_OFF 0x00u



/*
 * Name: Std_ReturnType
 * SWS_Item: SWS_Std_00005
 * Description: This type can be used as standard API return type
 * which is shared between the RTE and the BSW modules.
 * Range: E_OK = 0, E_NOT_OK = 1, 0x02-0x3F = 2
 *
 */
typedef uint8 Std_ReturnType;

/*
 * Name: Std_VersionInfoType
 * SWS_Item: SWS_Std_00015
 * Description: This type shall be used to request the version of a BSW module
 *  using the <Module name>_GetVersionInfo() function.
 */
typedef struct{

    uint16 vendorID;
    uint16 moduleID;
    uint8 sw_major_version;
    uint8 sw_minor_version;
    uint8 sw_patch_version;

}Std_VersionInfoType;


#endif /* STD_TYPES_H_ */

