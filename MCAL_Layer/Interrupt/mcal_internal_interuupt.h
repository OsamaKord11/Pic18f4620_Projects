/* 
 * File:   mcal_internal_interuupt.h
 * Author: intel
 *
 * Created on July 31, 2023, 10:14 PM
 */

#ifndef MCAL_INTERNAL_INTERUUPT_H
#define	MCAL_INTERNAL_INTERUUPT_H

/*Section  : Includes */
#include "mcal_interuupt_config.h"
/*Section  : Macro Declarations */

/*Section  : Macro Functions Declarations */
#if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
/*enable and disable the ADC interrupt*/     
#define ADC_INTERRUPTENABLE()         (PIE1bits.ADIE = 1)
#define ADC_INTERRUPTDISABLE()        (PIE1bits.ADIE = 0)
/*Clear and ADC interrupt flag*/ 
#define ADC_INTERRUPTFLAGCLEAR()      (PIR1bits.ADIF = 0)

/*In case of the priority is enabled */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
/*Global interrupt enable switch*/
#define INTERRUPT_GLOBALINTERRUPTENABLE() (INTCONbits.GIE = 1)
/*peripheral interrupt enable switch*/
#define INTERRUPT_PERIPHERALINTERRUPTENABLE() (INTCONbits.PEIE = 1)
/*ADC priority switch*/
#define ADC_HIGHPRIORITYSET()   (IPR1bits.ADIP = 1)
#define ADC_LOWPRIORITYSET()    (IPR1bits.ADIP = 0)


#endif
#endif
/*Section  : Data Type Declarations */

/*Section  : Function Declarations */




#endif	/* MCAL_INTERNAL_INTERUUPT_H */

