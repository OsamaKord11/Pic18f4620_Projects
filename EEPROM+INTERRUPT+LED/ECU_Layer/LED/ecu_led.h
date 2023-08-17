/* 
 * File:   ecu_led.h
 * Author: intel
 *
 * Created on June 28, 2023, 4:31 PM
 */

#ifndef ECU_LED_H
#define	ECU_LED_H

/*Section  : Includes */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "ecu_led_cfg.h" 

/*Section  : Macro Declarations */

/*Section  : Macro Functions Declarations */

/*Section  : Data Type Declarations */

typedef enum {
    LED_OFF = 0 ,
    LED_ON
}led_status_t;                          // these also can be represented by two #define 

typedef struct {
    uint8 port : 3 ;
    uint8 pin  : 3 ;
    uint8 led_status : 1 ;
    uint8 reserved   : 1 ;               // for future use , i do not need it now
}led_t;


/*Section  : Function Declarations */
Std_ReturnType led_initialize(const led_t * led);
Std_ReturnType led_turn_on(const led_t * led);
Std_ReturnType led_turn_off(const led_t * led);
Std_ReturnType led_turn_toggle(const led_t * led);

#endif	/* ECU_LED_H */

