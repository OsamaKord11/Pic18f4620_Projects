/* 
 * File:   application.h
 * Author: intel
 *
 * Created on June 28, 2023, 4:41 PM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H

/*Section  : Includes */


#include "ECU_Layer/LED/ecu_led.h"
#include "ECU_Layer/button/ecu_button.h"
#include "ECU_Layer/RELAY/ecu_relay.h"
#include "ECU_Layer/DC_MOTOR/dc_motor.h"
#include "ECU_Layer/seven_segment/seven_seg.h"
#include "ECU_Layer/KEYPAD/ecu_keypad.h"
#include "ECU_Layer/LCD/ecu_chr_lcd.h"
#include "MCAL_Layer/Interrupt/mcal_external_interuupt.h"
#include "MCAL_Layer/EEPROM/hal_eeprom.h"
#include "MCAL_Layer/ADC/hal_adc.h"

/*Section  : Macro Declarations */

/*Section  : Macro Functions Declarations */

/*Section  : Data Type Declarations */

/*Section  : Function Declarations */
void ADC_Default_Interrupt_Handler(void);

#endif	/* APPLICATION_H */

