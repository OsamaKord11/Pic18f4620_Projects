/* 
 * File:   dc_motor.c
 * Author: intel
 *
 * Created on July 15, 2023, 5:48 PM
 */

#include "dc_motor.h"

Std_ReturnType dc_motor_initialize(dc_motor_t * _dc_motor){
    Std_ReturnType ret = E_NOT_OK ;
    if (NULL == _dc_motor){
        
    }
    else{
        gpio_pin_initialize(&(_dc_motor->dc_motor_pin[0]));
        gpio_pin_initialize(&(_dc_motor->dc_motor_pin[1]));
    }
    return ret ;
}
Std_ReturnType dc_motor_move_right(dc_motor_t * _dc_motor){
    Std_ReturnType ret = E_NOT_OK ;
    if (NULL == _dc_motor){
        
    }
    else{
        gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[0]),GPIO_HIGH);
        gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[1]),GPIO_LOW);
    }
    return ret ;
}
Std_ReturnType dc_motor_move_left(dc_motor_t * _dc_motor){
    Std_ReturnType ret = E_NOT_OK ;
    if (NULL == _dc_motor){
        
    }
    else{
        gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[0]),GPIO_LOW);
        gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[1]),GPIO_HIGH);
    }
    return ret ;
}
Std_ReturnType dc_motor_stop(dc_motor_t * _dc_motor){
    Std_ReturnType ret = E_NOT_OK ;
    if (NULL == _dc_motor){
        
    }
    else{
        gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[0]),GPIO_LOW);
        gpio_pin_write_logic(&(_dc_motor->dc_motor_pin[1]),GPIO_LOW);
    }
    return ret ;
}