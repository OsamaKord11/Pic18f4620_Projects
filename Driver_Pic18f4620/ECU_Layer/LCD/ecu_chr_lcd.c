/* 
 * File:   ecu_chr_lcd.c
 * Author: intel
 *
 * Created on July 24, 2023, 10:49 AM
 */
#include "ecu_chr_lcd.h"


static Std_ReturnType lcd_send_4bits(const chr_lcd_4bit_t *lcd  , uint8 _command_data) ;
static Std_ReturnType  lcd_4bit_send_enable_signal(const chr_lcd_4bit_t *lcd );
static Std_ReturnType lcd_4bit_set_cursor(const chr_lcd_4bit_t *lcd , uint8 row , uint8 column);

static Std_ReturnType  lcd_8bit_send_enable_signal(const chr_lcd_8bit_t *lcd );
static Std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bit_t *lcd , uint8 row , uint8 column);

/**
 * @brief
 * @param lcd
 * @return 
 */
Std_ReturnType lcd_4bit_initialize(const chr_lcd_4bit_t *lcd){
    Std_ReturnType ret = E_NOT_OK;
    uint8 counter = 0 ;
    if (NULL == lcd){
        ret = E_NOT_OK;
    }
    else {
        ret = gpio_pin_initialize(&(lcd->lcd_rs));
        ret = gpio_pin_initialize(&(lcd->lcd_en));
        for (counter = 0 ; counter < 4 ; counter++){
            ret = gpio_pin_initialize(&(lcd->lcd_data[counter]));
        }
        __delay_ms(20);
        ret = lcd_4bit_send_command(lcd , _LCD_8BIT_MODE_2_LINE);
        __delay_ms(5);
        ret = lcd_4bit_send_command(lcd ,_LCD_8BIT_MODE_2_LINE);
        __delay_us(150);
        ret = lcd_4bit_send_command(lcd ,_LCD_8BIT_MODE_2_LINE);
        ret = lcd_4bit_send_command(lcd ,_LCD_CLEAR);
        ret = lcd_4bit_send_command(lcd ,_LCD_RETURN_HOME);
        ret = lcd_4bit_send_command(lcd ,_LCD_ENTRY_MODE_INC_SHIFT_OFF);
        ret = lcd_4bit_send_command(lcd ,_LCD_CURSOR_OFF_DISPLAY_ON);
        ret = lcd_4bit_send_command(lcd ,_LCD_4BIT_MODE_2_LINE);
        ret = lcd_4bit_send_command(lcd ,0x80);
    }
    return ret  ;
}
/**
 * @brief
 * @param lcd
 * @param command
 * @return 
 */
Std_ReturnType lcd_4bit_send_command(const chr_lcd_4bit_t *lcd , uint8 command){
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == lcd){
        ret = E_NOT_OK;
    }
    else {
        ret = gpio_pin_write_logic(&(lcd->lcd_rs) , GPIO_LOW);
        ret = lcd_send_4bits (lcd , (command >> 4));
        ret = lcd_4bit_send_enable_signal(lcd);
        ret = lcd_send_4bits (lcd ,command);
        ret = lcd_4bit_send_enable_signal(lcd); 
    }
    return ret  ;
}
/**
 * @brief
 * @param lcd
 * @param data
 * @return 
 */
Std_ReturnType lcd_4bit_send_char_data(const chr_lcd_4bit_t *lcd , uint8 data){
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == lcd){
        ret = E_NOT_OK;
    }
    else {
        
        ret = gpio_pin_write_logic(&(lcd->lcd_rs) , GPIO_HIGH);
        ret = lcd_send_4bits (lcd ,( data >> 4 ));
        ret = lcd_4bit_send_enable_signal(lcd);
        ret = lcd_send_4bits (lcd ,data);
        ret = lcd_4bit_send_enable_signal(lcd);  // After every send operation , enable operation should be
    }
    return ret  ;
}
/**
 * @brief
 * @param lcd
 * @param row
 * @param column
 * @param data
 * @return 
 */
Std_ReturnType lcd_4bit_send_char_data_pos(const chr_lcd_4bit_t *lcd ,uint8 row, 
                                           uint8 column, uint8 data){
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == lcd){
        ret = E_NOT_OK;
    }
    else {
        ret = lcd_4bit_set_cursor(lcd , row , column);
        ret = lcd_4bit_send_char_data(lcd , data);
    }
    return ret  ;
}
/**
 * @brief
 * @param lcd
 * @param str
 * @return 
 */
Std_ReturnType lcd_4bit_send_string(const chr_lcd_4bit_t *lcd , uint8 * str){
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == lcd){
        ret = E_NOT_OK;
    }
    else {
        while(*str){
            ret = lcd_4bit_send_char_data(lcd ,*str++);
        } 
    }
    return ret  ;
}
/**
 * @brief
 * @param lcd
 * @param row
 * @param column
 * @param str
 * @return 
 */
Std_ReturnType lcd_4bit_send_string_pos(const chr_lcd_4bit_t *lcd ,
                                        uint8 row, uint8 column, uint8 * str){
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == lcd){
        ret = E_NOT_OK;
    }
    else {
        ret = lcd_4bit_set_cursor(lcd , row , column);
         while(*str){
            ret = lcd_4bit_send_char_data(lcd ,*str++);
         }        
    }
    return ret  ;
}
/**
 * @brief
 * @param lcd
 * @param row
 * @param column
 * @param _chr
 * @param mem_pos
 * @return 
 */
Std_ReturnType lcd_4bit_send_custom_char(const chr_lcd_4bit_t *lcd , uint8 row, 
                                        uint8 column, const uint8 _chr[] , uint8 mem_pos){
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == lcd){
        ret = E_NOT_OK;
    }
    else {
        ret = lcd_4bit_send_command(lcd , (_LCD_CGRAM_START + (8 * mem_pos)));
        for (uint8 counter = 0 ; counter <=7 ; counter++){
            ret = lcd_4bit_send_char_data(lcd , _chr[counter]);
        }
        ret = lcd_4bit_send_char_data_pos(lcd , row , column , mem_pos);
    }
    return ret  ;
}
/**
 * @brief
 * @param lcd
 * @return 
 */
Std_ReturnType lcd_8bit_initialize(const chr_lcd_8bit_t *lcd){
    Std_ReturnType ret = E_NOT_OK;
    uint8 counter = 0 ;
    if (NULL == lcd){
        ret = E_NOT_OK;
    }
    else {
         ret = gpio_pin_initialize(&(lcd->lcd_rs));
        ret = gpio_pin_initialize(&(lcd->lcd_en));
        for (counter = 0 ; counter < 8 ; counter++){
            ret = gpio_pin_initialize(&(lcd->lcd_data[counter]));
    }   
        __delay_ms(20);
        ret = lcd_8bit_send_command(lcd , _LCD_8BIT_MODE_2_LINE);
        __delay_ms(5);
        ret = lcd_8bit_send_command(lcd ,_LCD_8BIT_MODE_2_LINE);
        __delay_us(150);
        ret = lcd_8bit_send_command(lcd ,_LCD_8BIT_MODE_2_LINE);
        ret = lcd_8bit_send_command(lcd ,_LCD_CLEAR);
        ret = lcd_8bit_send_command(lcd ,_LCD_RETURN_HOME);
        ret = lcd_8bit_send_command(lcd ,_LCD_ENTRY_MODE_INC_SHIFT_OFF);
        ret = lcd_8bit_send_command(lcd ,_LCD_CURSOR_OFF_DISPLAY_ON);
        ret = lcd_8bit_send_command(lcd ,_LCD_8BIT_MODE_2_LINE);
        ret = lcd_8bit_send_command(lcd ,0x80);
    return ret  ;
    }
}
/**
 * @brief
 * @param lcd
 * @param command
 * @return 
 */
Std_ReturnType lcd_8bit_send_command(const chr_lcd_8bit_t *lcd , uint8 command){
    Std_ReturnType ret = E_NOT_OK;
    uint8 counter = 0 ; 
    if (NULL == lcd){
        ret = E_NOT_OK;
    }
    else {
       ret = gpio_pin_write_logic(&(lcd->lcd_rs) , GPIO_LOW); 
       for (uint8 counter = 0 ; counter < 8 ; counter++){
           ret = gpio_pin_write_logic (&(lcd->lcd_data[counter]),(command >> counter) & (uint8)0x01);
       }
       ret = lcd_8bit_send_enable_signal (lcd);
    }
    return ret  ; 
}
/**
 * @brief
 * @param lcd
 * @param data
 * @return 
 */
Std_ReturnType lcd_8bit_send_char_data(const chr_lcd_8bit_t *lcd , uint8 data){
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == lcd){
        ret = E_NOT_OK;
    }
    else {
        ret = gpio_pin_write_logic(&(lcd->lcd_rs) , GPIO_HIGH);
       for (uint8 counter = 0 ; counter < 8 ; counter++){
           ret = gpio_pin_write_logic (&(lcd->lcd_data[counter]),(data >> counter) & (uint8)0x01);
       }
       ret = lcd_8bit_send_enable_signal(lcd); 
    } 
    return ret  ; 
}
/**
 * @brief
 * @param lcd
 * @param row
 * @param column
 * @param data
 * @return 
 */
Std_ReturnType lcd_8bit_send_char_data_pos(const chr_lcd_8bit_t *lcd ,uint8 row,
                                           uint8 column, uint8 data){
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == lcd){
        ret = E_NOT_OK;
    }
    else {
        ret = lcd_8bit_set_cursor(lcd ,row , column);
        ret = lcd_8bit_send_char_data(lcd , data);
    }
    return ret  ;
}
/**
 * @brief
 * @param lcd
 * @param str
 * @return 
 */
Std_ReturnType lcd_8bit_send_string(const chr_lcd_8bit_t *lcd , uint8 * str){
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == lcd){
        ret = E_NOT_OK;
    }
    else {
        while(*str){
            ret = lcd_8bit_send_char_data(lcd , *str++); // OR(....,*str); and then in a new line increment it --> str++; 
        }
    }
    return ret  ;
}
/**
 * @brief
 * @param lcd
 * @param row
 * @param column
 * @param str
 * @return 
 */
Std_ReturnType lcd_8bit_send_string_pos(const chr_lcd_8bit_t *lcd , uint8 row, 
                                        uint8 column, uint8* str){
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == lcd){
        ret = E_NOT_OK;
    }
    else {
        ret = lcd_8bit_set_cursor(lcd , row , column);
        while(*str){
            ret = lcd_8bit_send_char_data(lcd ,*str++); //OR(....,*str); and then in a new line increment it --> str++; 
        }
    }
    return ret  ;  
}
/**
 * @brief
 * @param lcd
 * @param row
 * @param column
 * @param _chr
 * @param mem_pos
 * @return 
 */
Std_ReturnType lcd_8bit_send_custom_char(const chr_lcd_8bit_t *lcd , uint8 row, 
                                         uint8 column, const uint8 _chr[] , uint8 mem_pos){
    Std_ReturnType ret = E_NOT_OK;
    if (NULL == lcd){
        ret = E_NOT_OK;
    }
    else {
        ret = lcd_8bit_send_command(lcd , (_LCD_CGRAM_START + (8 * mem_pos)));
        for (uint8 counter = 0 ; counter <=7 ; counter++){
            ret = lcd_8bit_send_char_data(lcd , _chr[counter]);
        }
        ret = lcd_8bit_send_char_data_pos(lcd , row , column , mem_pos);
    }
    
    return ret  ;  
}

/**
 * @brief
 * @param value
 * @param str
 */
Std_ReturnType convert_uint8_to_string(uint8 value , uint8 * str){
    Std_ReturnType ret = E_NOT_OK ; 
    if (NULL == str){
        ret = E_NOT_OK ; 
    }
    else{
       memset(str , '\0' , 4) ;
       sprintf(str , "%i" , value);
    }
    return ret ;
}
/**
 * @brief
 * @param value
 * @param str
 */
Std_ReturnType convert_uint16_to_string(uint16 value , uint8 * str){
   Std_ReturnType ret = E_NOT_OK ; 
   uint8 Temp_string[6] = {0};
   uint8 counter = 0 ;
   if(str == NULL){
       ret = E_NOT_OK ; 
   }
   else{
       memset(str , ' ' , 5);
       str[5] = '\0';
       sprintf(Temp_string , "%i" ,value);
       while(Temp_string[counter] != '\0'){
           str[counter]= Temp_string[counter];
           counter++;
       }
   }
   return ret; 
}
/**
 * @brief
 * @param value
 * @param str
 */
Std_ReturnType convert_uint32_to_string(uint32 value , uint8 * str){
    Std_ReturnType ret = E_NOT_OK ; 
   if(str == NULL){
       ret = E_NOT_OK ; 
   }
   else{
       memset(str , '\0' , 11);
       sprintf(str , "%i" ,value);
   }
   return ret; 
}

static Std_ReturnType lcd_send_4bits(const chr_lcd_4bit_t *lcd  , uint8 _command_data){
    Std_ReturnType ret =E_NOT_OK ; 
  
    ret = gpio_pin_write_logic (&(lcd->lcd_data[0]), (_command_data >> 0 ) & (uint8)0x01) ;
    ret = gpio_pin_write_logic (&(lcd->lcd_data[1]), (_command_data >> 1 ) & (uint8)0x01) ;
    ret = gpio_pin_write_logic (&(lcd->lcd_data[2]), (_command_data >> 2 ) & (uint8)0x01) ;
    ret = gpio_pin_write_logic (&(lcd->lcd_data[3]), (_command_data >> 3 ) & (uint8)0x01) ;
    return ret ; 
}

static Std_ReturnType  lcd_4bit_send_enable_signal(const chr_lcd_4bit_t *lcd ){
    Std_ReturnType ret =E_NOT_OK ; 
    ret = gpio_pin_write_logic (&(lcd->lcd_en),GPIO_HIGH);
    __delay_us(5);
    ret = gpio_pin_write_logic (&(lcd->lcd_en),GPIO_LOW);
    return ret ; 
}

static Std_ReturnType  lcd_8bit_send_enable_signal(const chr_lcd_8bit_t *lcd ){
    Std_ReturnType ret =E_NOT_OK ; 
    ret = gpio_pin_write_logic (&(lcd->lcd_en),GPIO_HIGH);
    __delay_us(5);
    ret = gpio_pin_write_logic (&(lcd->lcd_en),GPIO_LOW);
    return ret ; 
}

static Std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bit_t *lcd , uint8 row , uint8 column){
    Std_ReturnType ret =E_NOT_OK ;
    column-- ;
    switch(row){
        case  ROW1 : ret =  lcd_8bit_send_command(lcd , (0x80 + column));  break ; 
        case  ROW2 : ret =  lcd_8bit_send_command(lcd ,  (0xC0 + column));  break ;  
        case  ROW3 : ret =  lcd_8bit_send_command(lcd , (0x94 + column));  break ; 
        case  ROW4 : ret =  lcd_8bit_send_command(lcd ,  (0xD4 + column));  break ;   
    }
    return ret ; 
}

static Std_ReturnType lcd_4bit_set_cursor(const chr_lcd_4bit_t *lcd , uint8 row , uint8 column){
    Std_ReturnType ret =E_NOT_OK ;
    column-- ;
    switch(row){
        case  ROW1 : ret =  lcd_4bit_send_command(lcd , (0x80 + column));  break ; 
        case  ROW2 : ret =  lcd_4bit_send_command(lcd , (0xC0 + column));  break ;  
        case  ROW3 : ret =  lcd_4bit_send_command(lcd , (0x94 + column));  break ; 
        case  ROW4 : ret =  lcd_4bit_send_command(lcd , (0xD4 + column));  break ; 
    } 
    return ret ; 
}