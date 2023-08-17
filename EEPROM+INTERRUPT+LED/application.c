/* 
 * File:   application.c
 * Author: Osama Kord
 * www.linkedin.com/in/osama-kord-2080a7271
 * Created on Aug 11, 2023, 8:43 PM
 */

#include "application.h"
uint8 Program_Selected = 0;

void Int0_APP_ISR(void){
    Std_ReturnType ret = E_NOT_OK;
    Program_Selected++;
    if(3 == Program_Selected){
        Program_Selected = 0;
    }
    else{}
    ret = Data_EEPROM_WriteByte(0x3ff, Program_Selected);
}
led_t led1 = {.port= PORTC_INDEX, .pin = GPIO_PIN0, .led_status = GPIO_LOW};
led_t led2 = {.port= PORTC_INDEX, .pin = GPIO_PIN1, .led_status = GPIO_LOW};

interrupt_INTx_t int0_obj = {
  .EXT_Interrupt_Handler =  Int0_APP_ISR,
  .edge = INTERRUPT_RISING_EDGE,
  .priority = INTERRUPT_HIGH_PRIORITY,
  .source = INTERRUPT_EXTERNAL_INT0,
  .pin.port = PORTB_INDEX,
  .pin.pin = GPIO_PIN0,
  .pin.direction = GPIO_DIRECTION_INPUT
};



void program_1(void){
    led_turn_on(&led1);
    __delay_ms(250);
    led_turn_off(&led1);
    __delay_ms(250);
}

void program_2(void){
    led_turn_on(&led2);
    __delay_ms(250);
    led_turn_off(&led2);
    __delay_ms(250);
}

int main() { 
    Std_ReturnType ret = E_NOT_OK;

    ret = Interrupt_INTx_Init(&int0_obj);
    ret = led_initialize(&led1);
    ret = led_initialize(&led2);
    
    ret = Data_EEPROM_ReadByte(0x3ff, &Program_Selected);

    while(1){
        if(1 == Program_Selected){
            program_1();
        }
        else if(2 == Program_Selected){
            program_2();
        }
        else{
            led_turn_off(&led1);
            led_turn_off(&led2);
        }
    }
    return (EXIT_SUCCESS);
}



