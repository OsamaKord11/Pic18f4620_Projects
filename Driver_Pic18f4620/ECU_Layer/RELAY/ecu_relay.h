/* 
 * File:   ecu_relay.h
 * Author: intel
 *
 * Created on July 14, 2023, 6:23 PM
 */



#ifndef ECU_RELAY_H
#define	ECU_RELAY_H

/*Section  : Includes */
#include "ecu_relay_cfg.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"


/*Section  : Macro Declarations */
#define RELAY_ON_STATUS   0x01
#define RELAY_OFF_STATUS  0x00
/*Section  : Macro Functions Declarations */


/*Section  : Data Type Declarations */

typedef struct{
    uint8 relay_port : 3 ; 
    uint8 relay_pin : 3 ;   
    uint8 relay_status : 1 ; 
    uint8 reserved : 1 ; 
}relay_t;

/*Section  : Function Declarations */

Std_ReturnType relay_initialize(const relay_t * relay);
Std_ReturnType relay_turn_on(const relay_t * relay);
Std_ReturnType relay_turn_off(const relay_t * relay);

#endif	/* ECU_RELAY_H */

