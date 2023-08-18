/* 
 * File:   dc_motor.h
 * Author: intel
 *
 * Created on July 15, 2023, 5:48 PM
 */

#ifndef DC_MOTOR_H
#define	DC_MOTOR_H

/*Section  : Includes */

#include "dc_motor_cfg.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/*Section  : Macro Declarations */

#define DC_MOTOR_ON_STATUS   0x01
#define DC_MOTOR_OFF_STATUS  0x00

/*Section  : Macro Functions Declarations */

/*Section  : Data Type Declarations */
typedef struct {
    pin_config_t dc_motor_pin[2] ;
}dc_motor_t;

/*Section  : Function Declarations */
Std_ReturnType dc_motor_initialize(dc_motor_t * _dc_motor);
Std_ReturnType dc_motor_move_right(dc_motor_t * _dc_motor);
Std_ReturnType dc_motor_move_left(dc_motor_t * _dc_motor);
Std_ReturnType dc_motor_stop(dc_motor_t * _dc_motor);
#endif	/* DC_MOTOR_H */

