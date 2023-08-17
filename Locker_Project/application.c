/* 
 * File:   application.c
 * Author: Osama Kord
 * www.linkedin.com/in/osama-kord-2080a7271
 * Created on Aug 11, 2023, 8:43 PM
 */
#include "application.h"

chr_lcd_4bit_t lcd1 = {
    .lcd_rs.port = PORTE_INDEX,
    .lcd_rs.pin = GPIO_PIN0,
    .lcd_rs.direction = GPIO_DIRECTION_OUTPUT,
    .lcd_rs.logic = GPIO_LOW,

    .lcd_en.port = PORTE_INDEX,
    .lcd_en.pin = GPIO_PIN1,
    .lcd_en.direction = GPIO_DIRECTION_OUTPUT,
    .lcd_en.logic = GPIO_LOW,

    .lcd_data[0].port = PORTC_INDEX,
    .lcd_data[0].pin = GPIO_PIN0,
    .lcd_data[0].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[0].logic = GPIO_LOW,

    .lcd_data[1].port = PORTC_INDEX,
    .lcd_data[1].pin = GPIO_PIN1,
    .lcd_data[1].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[1].logic = GPIO_LOW,

    .lcd_data[2].port = PORTC_INDEX,
    .lcd_data[2].pin = GPIO_PIN2,
    .lcd_data[2].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[2].logic = GPIO_LOW,

    .lcd_data[3].port = PORTC_INDEX,
    .lcd_data[3].pin = GPIO_PIN3,
    .lcd_data[3].direction = GPIO_DIRECTION_OUTPUT,
    .lcd_data[3].logic = GPIO_LOW
};

keypad_t keypad1 = {
    .keypad_row_pins[0].port = PORTD_INDEX,
    .keypad_row_pins[0].pin = GPIO_PIN0,
    .keypad_row_pins[0].direction = GPIO_DIRECTION_OUTPUT,
    .keypad_row_pins[0].logic = GPIO_LOW,

    .keypad_row_pins[1].port = PORTD_INDEX,
    .keypad_row_pins[1].pin = GPIO_PIN1,
    .keypad_row_pins[1].direction = GPIO_DIRECTION_OUTPUT,
    .keypad_row_pins[1].logic = GPIO_LOW,

    .keypad_row_pins[2].port = PORTD_INDEX,
    .keypad_row_pins[2].pin = GPIO_PIN2,
    .keypad_row_pins[2].direction = GPIO_DIRECTION_OUTPUT,
    .keypad_row_pins[2].logic = GPIO_LOW,

    .keypad_row_pins[3].port = PORTD_INDEX,
    .keypad_row_pins[3].pin = GPIO_PIN3,
    .keypad_row_pins[3].direction = GPIO_DIRECTION_OUTPUT,
    .keypad_row_pins[3].logic = GPIO_LOW,

    .keypad_columns_pins[0].port = PORTD_INDEX,
    .keypad_columns_pins[0].pin = GPIO_PIN4,
    .keypad_columns_pins[0].direction = GPIO_DIRECTION_INPUT,
    .keypad_columns_pins[0].logic = GPIO_LOW,

    .keypad_columns_pins[1].port = PORTD_INDEX,
    .keypad_columns_pins[1].pin = GPIO_PIN5,
    .keypad_columns_pins[1].direction = GPIO_DIRECTION_INPUT,
    .keypad_columns_pins[1].logic = GPIO_LOW,

    .keypad_columns_pins[2].port = PORTD_INDEX,
    .keypad_columns_pins[2].pin = GPIO_PIN6,
    .keypad_columns_pins[2].direction = GPIO_DIRECTION_INPUT,
    .keypad_columns_pins[2].logic = GPIO_LOW,

    .keypad_columns_pins[3].port = PORTD_INDEX,
    .keypad_columns_pins[3].pin = GPIO_PIN7,
    .keypad_columns_pins[3].direction = GPIO_DIRECTION_INPUT,
    .keypad_columns_pins[3].logic = GPIO_LOW

};


Std_ReturnType ret = E_NOT_OK;
uint8 keypad_value = '0';
uint8 tries = MAX_TRIES;
uint8 flag = 0;
uint8 Pass[4];
uint8 Pin[4];
uint8 check_Pass[4];
uint8 counter = 0;

int main() {

    ret = lcd_4bit_initialize(&lcd1);
    ret = keypad_initialize(&keypad1);

    ret = lcd_4bit_send_string_pos(&lcd1, 2, 2, "Welcome to ");
    ret = lcd_4bit_send_string_pos(&lcd1, 2, 14, "Locker");
    __delay_ms(2000);
    ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
    __delay_ms(50);

    while (1) {
        for (counter = 0; counter < PASS_LENGTH; counter++) {
            ret = lcd_4bit_send_string_pos(&lcd1, 1, 1, "Pass:");
            do {
                ret = keypad_get_value(&keypad1, &keypad_value);
            } while (keypad_value == '0');
            Pass[counter] = keypad_value;
            ret = lcd_4bit_send_char_data_pos(&lcd1, 1, 6 + counter, keypad_value);
            __delay_ms(250);
            ret = lcd_4bit_send_char_data_pos(&lcd1, 1, 6 + counter, '*');
            __delay_ms(10);
            keypad_value = '0';
            flag = 0;
        }
        while (flag == 0) {
            __delay_ms(250);
            ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
            __delay_ms(50);
            for (counter = 0; counter < PASS_LENGTH; counter++) {
                ret = lcd_4bit_send_string_pos(&lcd1, 1, 1, "Confirm Pass:");
                do {
                    ret = keypad_get_value(&keypad1, &keypad_value);
                } while (keypad_value == '0');
                check_Pass[counter] = keypad_value;
                ret = lcd_4bit_send_char_data_pos(&lcd1, 1, 14 + counter, keypad_value);
                __delay_ms(250);
                ret = lcd_4bit_send_char_data_pos(&lcd1, 1, 14 + counter, '*');
                keypad_value = '0';
            }
            __delay_ms(500);
            ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
            __delay_ms(50);

            if ((Pass[0] == check_Pass[0]) && (Pass[1] == check_Pass[1])
                    &&(Pass[2] == check_Pass[2]) && (Pass[3] == check_Pass[3])) {
                ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
                __delay_ms(50);
                ret = lcd_4bit_send_string_pos(&lcd1, 1, 1, "Correct Password");
                __delay_ms(2000);
                ret = lcd_4bit_send_string_pos(&lcd1, 2, 1, "Door is Opened");
                __delay_ms(2000);
                ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
                ret = lcd_4bit_send_string_pos(&lcd1, 3, 1, "System Closed");
                __delay_ms(2000);
                ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
                __delay_ms(25000);
                flag = 1;   // Function done successfully , go out if 

            }

            else {
                tries--;  
                ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
                __delay_ms(50);
                ret = lcd_4bit_send_string_pos(&lcd1, 1, 1, "Wrong Password");
                __delay_ms(2000);
                ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
                __delay_ms(50);
                ret = lcd_4bit_send_string_pos(&lcd1, 1, 1, "Your tries decreased");
                ret = lcd_4bit_send_string_pos(&lcd1, 2, 1, "to:");
                ret = lcd_4bit_send_char_data_pos(&lcd1, 2, 4, tries + 48);
                __delay_ms(2000);
                ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
                __delay_ms(5);

                if (tries == 0) {
               
                    ret = lcd_4bit_send_string_pos(&lcd1, 1, 1, "No available tries");
                    __delay_ms(2000);
                    ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
                    __delay_ms(50);
                    ret = lcd_4bit_send_string_pos(&lcd1, 1, 1, "Reset Password");
                    __delay_ms(2000);
                    ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
                    __delay_ms(50);
                    ret = lcd_4bit_send_string_pos(&lcd1, 1, 1, "To Reset Press1");
                     keypad_value = '0';
                    do {
                        ret = keypad_get_value(&keypad1, &keypad_value);
                    } while (keypad_value == '0');

                    if ('1' == keypad_value) {
                        ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
                        __delay_ms(100);
                        ret = lcd_4bit_send_string_pos(&lcd1, 1, 1, "Enter PIN num:");
                        __delay_ms(100);
                        keypad_value = '0';
                        for (counter = 0; counter < PASS_LENGTH; counter++) {
                            do {
                                ret = keypad_get_value(&keypad1, &keypad_value);
                            } while (keypad_value == '0');
                            Pin[counter] = keypad_value;
                            ret = lcd_4bit_send_char_data_pos(&lcd1, 1, 15 + counter, keypad_value);
                            __delay_ms(250);
                            ret = lcd_4bit_send_char_data_pos(&lcd1, 1, 15 + counter, '*');
                            __delay_ms(100);
                            keypad_value = '0';
                        }
                        __delay_ms(1000);
                        ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
                        __delay_ms(100);
                        keypad_value == '0';
                        if (Pin[0] == '5' && Pin[1] == '5' && Pin[2] == '5' && Pin[3] == '5') {
                            tries = 3;
                            for (counter = 0; counter < PASS_LENGTH; counter++) {
                                ret = lcd_4bit_send_string_pos(&lcd1, 1, 1, "Pass:");
                                do {
                                    ret = keypad_get_value(&keypad1, &keypad_value);
                                } while (keypad_value == '0');
                                Pass[counter] = keypad_value;
                                ret = lcd_4bit_send_char_data_pos(&lcd1, 1, 6 + counter, keypad_value);
                                __delay_ms(250);
                                ret = lcd_4bit_send_char_data_pos(&lcd1, 1, 6 + counter, '*');
                                __delay_ms(10);
                                keypad_value = '0';
                            }
                        } else {
                            ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
                            __delay_ms(50);
                            ret = lcd_4bit_send_string_pos(&lcd1, 1, 1, "PIN number is wrong");
                            __delay_ms(2000);
                            ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
                            __delay_ms(50);
                            ret = lcd_4bit_send_string_pos(&lcd1, 1, 1, "Try again after");
                            ret = lcd_4bit_send_string_pos(&lcd1, 2, 1, "2 hours");
                            __delay_ms(2000);
                            ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
                            __delay_ms(50);
                            ret = lcd_4bit_send_string_pos(&lcd1, 1, 1, "System Closed");
                            __delay_ms(2000);
                            ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
                            __delay_ms(25000);
                            flag = 1;
                        }
                    }
                    else{
                        ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
                         __delay_ms(50);
                        ret = lcd_4bit_send_string_pos(&lcd1, 1, 1, "Wrong Pressed Number");
                            __delay_ms(2000);
                        ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
                            __delay_ms(50);
                        ret = lcd_4bit_send_string_pos(&lcd1, 1, 1, "System Closed");
                            __delay_ms(2000);
                        ret = lcd_4bit_send_command(&lcd1, _LCD_CLEAR);
                            __delay_ms(25000);
                            flag = 1;
                    }
                }
            }
        }
    }
    return (EXIT_SUCCESS);
}
