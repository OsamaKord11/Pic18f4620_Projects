/* 
 * File:   mcal_interuupt_manager.h
 * Author: intel
 *
 * Created on July 31, 2023, 10:15 PM
 */

#ifndef MCAL_INTERUUPT_MANAGER_H
#define	MCAL_INTERUUPT_MANAGER_H

/*Section  : Includes */
#include "mcal_interuupt_config.h"
/*Section  : Macro Declarations */

/*Section  : Macro Functions Declarations */

/*Section  : Data Type Declarations */

/*Section  : Function Declarations */
void INT0_ISR(void);
void INT1_ISR(void);
void INT2_ISR(void);

void RB4_ISR(uint8 RB4_Source);
void RB5_ISR(uint8 RB5_Source);
void RB6_ISR(uint8 RB6_Source);
void RB7_ISR(uint8 RB7_Source);

 void ADC_ISR (void);
#endif	/* MCAL_INTERUUPT_MANAGER_H */

