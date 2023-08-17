/* 
 * File:   ecu_button.c
 * Author: intel
 *
 * Created on June 13, 2023, 7:41 PM
 */
#include "ecu_button.h"

/**
 * @brief : initialize a button 
 * @param btn : a pointer to a const data of type struct (button_t) ,to use it to reach the struct to configure its members
 * @return : status of the function
 *        (E_OK)     : the function done successfully 
 *        (E_NOT_OK) : the function has issue(s) to perform this action  
 */

Std_ReturnType button_initialize(const button_t *btn) {
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == btn) {

        ret = E_NOT_OK;
    } else {
        ret = gpio_pin_direction_initialize(&(btn->button_pin));
    }
    return ret;
}

/**
 * @brief :to read the status of a button if it is pressed or released  
 * @param btn : a pointer to a const data of type struct (button_t) ,to use it to reach the struct to configure its members
 * @param btn_state : a pinter used to return the state of a button(indirectly) if pressed or released 
 * @return : status of the function
 *        (E_OK)     : the function done successfully 
 *        (E_NOT_OK) : the function has issue(s) to perform this action   
 */

Std_ReturnType button_read_state(const button_t *btn, button_state_t *btn_state) {
    Std_ReturnType ret = E_NOT_OK;
    if ((NULL == btn) || (NULL == btn_state)) {
        ret = E_NOT_OK;
    } else {
        logic_t btn_logic = GPIO_LOW;
        gpio_pin_read_logic(&(btn->button_pin), &btn_logic);
        if (BUTTON_ACTIVE_HIGH == btn->button_connection) {
            if (GPIO_HIGH == btn_logic) {
                *btn_state = BUTTON_PRESSED;
            } else if (GPIO_LOW == btn_logic) {
                *btn_state = BUTTON_RELEASED;
            } else {
                /*NO THING*/
            }
        } else if (BUTTON_ACTIVE_LOW == btn->button_connection) {
            if (GPIO_HIGH == btn_logic) {
                *btn_state = BUTTON_RELEASED;
            } else if (GPIO_LOW == btn_logic) {
                *btn_state = BUTTON_PRESSED;
            } else {
                /*NO THING*/
            }
        } else {
            /*NO THING*/
        }
        ret = E_OK;
    }
    return ret;
}