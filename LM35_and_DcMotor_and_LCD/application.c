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





#include "application.h"


//void ADC_DefaultInterruptHandler(void);


chr_lcd_4bit_t lcd_1 = {
    .lcd_rs.port = PORTC_INDEX,
    .lcd_rs.pin = GPIO_PIN0,
    .lcd_rs.direction = GPIO_DIRECTION_OUTPUT,
    .lcd_rs.logic = GPIO_LOW,
    .lcd_en.port = PORTC_INDEX,
    .lcd_en.pin = GPIO_PIN1,
    .lcd_en.direction = GPIO_DIRECTION_OUTPUT,
    .lcd_en.logic = GPIO_LOW,
    .lcd_data[0].port = PORTC_INDEX,
    .lcd_data[0].pin = GPIO_PIN2,
    .lcd_data[0].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[0].logic = GPIO_LOW,
    .lcd_data[1].port = PORTC_INDEX,
    .lcd_data[1].pin = GPIO_PIN3,
    .lcd_data[1].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[1].logic = GPIO_LOW,
    .lcd_data[2].port = PORTC_INDEX,
    .lcd_data[2].pin = GPIO_PIN4,
    .lcd_data[2].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[2].logic = GPIO_LOW,
    .lcd_data[3].port = PORTC_INDEX,
    .lcd_data[3].pin = GPIO_PIN5,
    .lcd_data[3].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[3].logic = GPIO_LOW
};

adc_conf_t adc_1 = {
    .ADC_Interrupt_handler = NULL,
    .acquisition_time = ADC_12_TAD,
    .channel = ADC_CHANNEL0_AN0,
    .conversion_clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    .result_format = ADC_RESULT_RIGHT,
    .voltage_reference = ADC_VOLTAGEREFERENCEDISABLE
};

dc_motor_t motor1 = {
    .dc_motor_pin[0].port = PORTD_INDEX,
    .dc_motor_pin[0].pin = GPIO_PIN0,
    .dc_motor_pin[0].direction = GPIO_DIRECTION_OUTPUT,
    .dc_motor_pin[0].logic = DC_MOTOR_OFF_STATUS,
    .dc_motor_pin[1].port = PORTD_INDEX,
    .dc_motor_pin[1].pin = GPIO_PIN1,
    .dc_motor_pin[1].direction = GPIO_DIRECTION_OUTPUT,
    .dc_motor_pin[1].logic = DC_MOTOR_OFF_STATUS
};

dc_motor_t motor2 = {
    .dc_motor_pin[0].port = PORTD_INDEX,
    .dc_motor_pin[0].pin = GPIO_PIN2,
    .dc_motor_pin[0].direction = GPIO_DIRECTION_OUTPUT,
    .dc_motor_pin[0].logic = DC_MOTOR_OFF_STATUS,
    .dc_motor_pin[1].port = PORTD_INDEX,
    .dc_motor_pin[1].pin = GPIO_PIN3,
    .dc_motor_pin[1].direction = GPIO_DIRECTION_OUTPUT,
    .dc_motor_pin[1].logic = DC_MOTOR_OFF_STATUS
};

uint16 lm35_res_1 = 0, lm35_res_2 = 0 ,lm35_res_1_celisious_mv= 0,lm35_res_2_celisious_mv = 0;
uint8 lm35_res_1_txt[7], lm35_res_2_txt[7];
uint8 ADC_Req = 0;

int main() { 
    Std_ReturnType ret = E_NOT_OK;
    
    ret = ADC_Initialize(&adc_1);
    ret = lcd_4bit_initialize(&lcd_1);
    ret = dc_motor_initialize(&motor1);
    ret = dc_motor_initialize(&motor2);
    
    ret = lcd_4bit_send_string_pos(&lcd_1, 1, 7, "LM35 Test");
    ret = lcd_4bit_send_string_pos(&lcd_1, 2, 1, "Temp1: ");
    ret = lcd_4bit_send_string_pos(&lcd_1, 3, 1, "Temp2: ");
    
    while(1){  

        ret = ADC_Get_Conversion_Blocking(&adc_1, ADC_CHANNEL0_AN0, &lm35_res_1);
        ret = ADC_Get_Conversion_Blocking(&adc_1, ADC_CHANNEL1_AN1 ,&lm35_res_2);
        
        lm35_res_1_celisious_mv = lm35_res_1 * 4.88f ;    // 4.88mv = vref(5000mv)/resolution(2^10)
        lm35_res_2_celisious_mv = lm35_res_2 * 4.88f ;    // 4.88mv = vref(5000mv)/resolution(2^10)
        
        lm35_res_1_celisious_mv /= 10 ; // from data sheet of LM35 temperature sensor --> every 1 c? = 10 mv ; 
        lm35_res_2_celisious_mv /= 10 ; // from data sheet of LM35 temperature sensor --> every 1 c? = 10 mv ; 
     
        ret = convert_uint16_to_string(lm35_res_1_celisious_mv, lm35_res_1_txt); // I want to display (lm35_res_1_celisious_mv) ,so it must be converted to string
        ret = convert_uint16_to_string(lm35_res_2_celisious_mv, lm35_res_2_txt); // I want to display (lm35_res_1_celisious_mv) ,so it must be converted to string

        ret = lcd_4bit_send_string_pos(&lcd_1, 2, 8, lm35_res_1_txt);
        ret = lcd_4bit_send_string_pos(&lcd_1, 3, 8, lm35_res_2_txt); 
        
        if(lm35_res_1_celisious_mv > 25){
            dc_motor_move_right(&motor1);
        }
        else{
             dc_motor_stop(&motor1);
        }
        if(lm35_res_2_celisious_mv > 25){
            dc_motor_move_left(&motor2);
        }
        else{
             dc_motor_stop(&motor2);
        }
    }
    return (EXIT_SUCCESS);
}

//
//void ADC_DefaultInterruptHandler(void){
//    Std_ReturnType ret = E_NOT_OK;
//    if(0 == ADC_Req){
//        ret = ADC_Get_Conversion_Result(&adc_1, &adc_res_1);
//        ADC_Req = 1;
//    }
//    else if(1 == ADC_Req){
//        ret = ADC_Get_Conversion_Result(&adc_1, &adc_res_2);
//        ADC_Req = 2;
//    }
//    else if(2 == ADC_Req){
//        ret = ADC_Get_Conversion_Result(&adc_1, &adc_res_3);
//        ADC_Req = 3;
//    }
//    else if(3 == ADC_Req){
//        ret = ADC_Get_Conversion_Result(&adc_1, &adc_res_4);
//        ADC_Req = 0;
//    }
//    else { /* Nothing */ }
//}
//
