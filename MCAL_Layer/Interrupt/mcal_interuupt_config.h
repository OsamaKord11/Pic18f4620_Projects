/* 
 * File:   mcal_interuupt_config.h
 * Author: intel
 *
 * Created on July 31, 2023, 10:16 PM
 */

#ifndef MCAL_INTERUUPT_CONFIG_H
#define	MCAL_INTERUUPT_CONFIG_H

/*Section  : Includes */
#include <pic18f4620.h>
#include "../mcal_std-types.h"
#include "mcal_interuupt_gen_cfg.h"
#include "../GPIO/hal_gpio.h"
/*Section  : Macro Declarations */

#define INTERRUPT_ENABLE           1
#define INTERRUPT_DISABLE          0
#define INTERRUPT_OCCUR            1
#define INTERRUPT_NOT_OCCUR        0
#define INTERRUPT_PRIORITY_ENABLE  1
#define INTERRUPT_PRIORITY_DISABLE 0

/*Section  : Macro Functions Declarations */

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE

#define INTERRUPT_PRIORITYLEVELSENABLE() (RCONbits.IPEN = 1)
#define INTERRUPT_PRIORITYLEVELSDISABLE() (RCONbits.IPEN = 0)

#define INTERRUPT_GLOBALINTERRUPTHIGHENABLE() (INTCONbits.GIEH = 1)
#define INTERRUPT_GLOBALINTERRUPTHIGHDISABLE() (INTCONbits.GIEH = 0)

#define INTERRUPT_GLOBALINTERRUPTLOWENABLE() (INTCONbits.GIEL = 1)
#define INTERRUPT_GLOBALINTERRUPTLOWDISABLE() (INTCONbits.GIEL = 0)

#else

#define INTERRUPT_GLOBALINTERRUPTENABLE() (INTCONbits.GIE = 1)
#define INTERRUPT_GLOBALINTERRUPTDISABLE() (INTCONbits.GIE = 0)

#define INTERRUPT_PERIPHERALINTERRUPTENABLE() (INTCONbits.PEIE = 1)
#define INTERRUPT_PERIPHERALINTERRUPTDISABLE() (INTCONbits.PEIE = 0)
#endif



/*Section  : Data Type Declarations */
typedef enum{
    INTERRUPT_LOW_PRIORITY = 0 ,
    INTERRUPT_HIGH_PRIORITY
}interrupt_priority_cfg;
/*Section  : Function Declarations */



#endif	/* MCAL_INTERUUPT_CONFIG_H */

