/* 
 * File:   hal_Timer0.h
 * Author: intel
 *
 * Created on August 19, 2023, 9:22 PM
 */

#ifndef HAL_TIMER0_H
#define	HAL_TIMER0_H

/*Section  : Includes */
#include "../GPIO/hal_gpio.h"
#include"../Interrupt/mcal_internal_interuupt.h"
/*Section  : Macro Declarations */
#define TIMER0_PRESCALAR_ENABLE_CFG  1
#define TIMER0_PRESCALAR_DISABLE_CFG 0
#define TIMER0_RISING_EDGE_CFG  1
#define TIMER0_FALLING_EDGE_CFG 0
#define TIMER0_TIMER_MODE_CFG   1
#define TIMER0_COUNTER_MODE_CFG 0 
#define TIMER0_8bit_REGISTER_CFG  1
#define TIMER0_16bit_REGISTER_CFG 0
/*Section  : Macro Functions Declarations */

/*= Timer0 prescaler is assigned. Timer0 clock input comes from prescaler output.*/
#define TIMER0_PRESCALAR_ENABLE()           (T0CONbits.PSA = 0)
/*TImer0 prescaler is not assigned. Timer0 clock input bypasses prescaler*/
#define TIMER0_PRESCALAR_DISABLE()           (T0CONbits.PSA = 1)  

/*= Increment on low-to-high transition on T0CKI pin*/
#define TIMER0_RISING_EDGE()      (T0CONbits.T0SE = 0 )
/* Increment on high-to-low transition on T0CKI pin*/
#define TIMER0_FALLING_EDGE()     (T0CONbits.T0SE = 1 )

/*Internal instruction cycle clock*/
#define TIMER0_TIMER_MODE_ENABLE()     (T0CONbits.T0CS = 0 )
/*Transition on T0CKI pin*/
#define TIMER0_COUNTER_MODE_ENABLE()   (T0CONbits.T0CS = 1 )

/*Timer0 is configured as an 8-bit timer/counter*/
#define TIMER0_8BIT_REG_MODE_ENABLE()     (T0CONbits.T08BIT = 1 )
/*Timer0 is configured as a 16-bit timer/counter */
#define TIMER0_16BIT_REG_MODE_ENABLE()    (T0CONbits.T08BIT = 0 )

/* Enables Timer0*/
#define TIMER0_MODULE_ENABLE()   (T0CONbits.TMR0ON = 1)
/*Stops Timer0*/
#define TIMER0_MODULE_DISABLE()   (T0CONbits.TMR0ON = 0)
/*Section  : Data Type Declarations */
typedef enum{
    TIMER0_PRESCALAR_VALUE_DIV_2 = 0 ,
    TIMER0_PRESCALAR_VALUE_DIV_4,        
    TIMER0_PRESCALAR_VALUE_DIV_8 ,       
    TIMER0_PRESCALAR_VALUE_DIV_16 ,
    TIMER0_PRESCALAR_VALUE_DIV_32 ,
    TIMER0_PRESCALAR_VALUE_DIV_64 ,
    TIMER0_PRESCALAR_VALUE_DIV_128 ,
    TIMER0_PRESCALAR_VALUE_DIV_256         
}Timer0_Prescaler_Select_t;

typedef struct {
#if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
       void (*Timer0_InterruptHandler)(void); 
       interrupt_priority_cfg priority;
#endif
    Timer0_Prescaler_Select_t prescalar_value ;
    uint16 timer0_preload_value ; 
    uint8 prescalar_enable : 1 ; 
    uint8 timer0_counter_edge : 1 ; 
    uint8 timer0_mode : 1 ;
    uint8 rgister_size : 1 ; 
    uint8 reserved : 4 ;
}timer0_t;
/*Section  : Function Declarations */
Std_ReturnType Timer0_Init(const timer0_t * _timer0);
Std_ReturnType Timer0_DeInit(const timer0_t * _timer0);
Std_ReturnType Timer0_Write_Value(const timer0_t * _timer0 , uint16 _value);
Std_ReturnType Timer0_Read_Value(const timer0_t * _timer0 , uint16 * _value);
#endif	/* HAL_TIMER0_H */

