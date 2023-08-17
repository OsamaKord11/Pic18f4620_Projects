/* 
 * File:   ecu_relay.c
 * Author: intel
 *
 * Created on July 14, 2023, 6:23 PM
 */


#include "ecu_relay.h"


/**
 * @brief : initialize the pin direction and write an initial logic on it in start
 * @param relay : relay : a pointer to struct to access (relay_t) to configure the relay
 * @return status of the function
 *        (E_OK)     : the function done successfully 
 *        (E_NOT_OK) : the function has issue(s) to perform this action
 */
Std_ReturnType relay_initialize(const relay_t * relay){
    Std_ReturnType ret = E_NOT_OK ; 
        if (relay == NULL){
            ret = E_NOT_OK ; 
        }
        else{
            pin_config_t relay_obj = {.port = relay->relay_port, .pin = relay->relay_pin,
                                      .direction = GPIO_DIRECTION_OUTPUT , .logic = relay->relay_status};                            
                gpio_pin_initialize(&relay_obj);
        }
    return ret ;
}
/**
 * @brief turn on the relay
 * @param relay : relay : a pointer to struct to access (relay_t) to configure the relay 
 * @return status of the function
 *        (E_OK)     : the function done successfully 
 *        (E_NOT_OK) : the function has issue(s) to perform this action
 */
Std_ReturnType relay_turn_on(const relay_t * relay){
    Std_ReturnType ret = E_NOT_OK ; 
        if (relay == NULL){
            ret = E_NOT_OK ; 
        }
        else{
            pin_config_t relay_obj = {.port = relay->relay_port, .pin = relay->relay_pin,
                                      .direction = GPIO_DIRECTION_OUTPUT , .logic = relay->relay_status};                            
                  gpio_pin_write_logic(&relay_obj ,GPIO_HIGH);
        }
    return ret ;
}
/**
 * @brief turn of the relay
 * @param relay : a pointer to struct to access (relay_t) to configure the relay 
 * @return status of the function
 *        (E_OK)     : the function done successfully 
 *        (E_NOT_OK) : the function has issue(s) to perform this action
 */
Std_ReturnType relay_turn_off(const relay_t * relay){
    Std_ReturnType ret = E_NOT_OK ; 
        if (relay == NULL){
            ret = E_NOT_OK ; 
        }
        else{
            pin_config_t relay_obj = {.port = relay->relay_port, .pin = relay->relay_pin,
                                      .direction = GPIO_DIRECTION_OUTPUT , .logic = relay->relay_status};                            
                  gpio_pin_write_logic(&relay_obj ,GPIO_LOW);
        }
    return ret ;   
}


