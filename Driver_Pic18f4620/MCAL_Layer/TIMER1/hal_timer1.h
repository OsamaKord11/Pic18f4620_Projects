/* 
 * File:   hal_timer1.h
 * Author: intel
 *
 * Created on August 21, 2023, 8:55 AM
 */

#ifndef HAL_TIMER1_H
#define	HAL_TIMER1_H

/*Section  : Includes */
#include "../GPIO/hal_gpio.h"
#include "../Interrupt/mcal_internal_interuupt.h"
/*Section  : Macro Declarations */

#define TIMER1_COUNTER_MODE_CFG     1
#define TIMER1_TIMER_MODE_CFG       0
#define TIMER1_ASYNC_COUNTER_CFG    1
#define TIMER1_SYNC_COUNTER_CFG     0
/* 16-Bit Read/Write Mode Enable bit, often in timer1 the 16bit are used*/
#define TIMER1_8bit_RW_REGISTER_CFG   0
#define TIMER1_16bit_RW_REGISTER_CFG  1

#define TIMER1_OSC_ENABLE_CFG       1
#define TIMER1_OSC_DISABLE_CFG      0


/* Timer1 Input Clock Prescale Select bits*/
#define  TIMER1_PRESCALAR_VALUE_DIV_1   0
#define  TIMER1_PRESCALAR_VALUE_DIV_2   1
#define  TIMER1_PRESCALAR_VALUE_DIV_4   2
#define  TIMER1_PRESCALAR_VALUE_DIV_8   3



/*Section  : Macro Functions Declarations */

/*1 = Enables Timer1 */
#define TIMER1_MODULE_ENABLE()                  (T1CONbits.TMR1ON = 1)
/*0 = Stops Timer1 */
#define TIMER1_MODULE_DISABLE()                 (T1CONbits.TMR1ON = 0)

/*0 = Internal clock (FOSC/4) --> timer*/
#define TIMER1_TIMER_MODE_ENABLE()               (T1CONbits.TMR1CS = 0)
/*1 = External clock from pin RC0/T1OSO/T13CKI (on the rising edge) --> counter*/
#define TIMER1_COUNTER_MODE_ENABLE()              (T1CONbits.TMR1CS = 1)

/*When TMR1CS = 1:
if T1SYNC = 1 = Do not synchronize external clock input */
#define TIMER1_ASYNC_COUNTER_ENABLE()           (T1CONbits.T1SYNC = 1)
/*When TMR1CS = 1:
if T1SYNC = 0 = Synchronize external clock input*/
#define TIMER1_SYNC_COUNTER_ENABLE()            (T1CONbits.T1SYNC = 0)
/*1 = Timer1 oscillator is enabled */
#define TIMER1_OSCILLATOR_ENABLE()              (T1CONbits.T1OSCEN = 1)
/*0 = Timer1 oscillator is shut off*/
#define TIMER1_OSCILLATOR_DISABLE()             (T1CONbits.T1OSCEN = 0)

/* Timer1 Input Clock Prescale Select bits*/
#define TIMER1_PRESCALAR_SELECT(_PRESCALAR_VAL) (T1CONbits.T1CKPS = _PRESCALAR_VAL)

/*Timer1 System Clock Status bit , it is raed only ,you can not write 0 nor 1 into it
 if T1RUN =  1 = Device clock is derived from Timer1 oscillator
 else T1RUN = 0 = Device clock is derived from another source*/
#define TIMER1_SYS_CLK_STATUS()                 (T1CONbits.T1RUN)
 
/*0 = Enables register read/write of Timer1 in two 8-bit operations*/
#define TIMER1_8BIT_RW_REG_MODE_ENABLE()     (T1CONbits.RD16 = 0 )
/*1 = Enables register read/write of TImer1 in one 16-bit operation*/
#define TIMER1_16BIT_RW_REG_MODE_ENABLE()    (T1CONbits.RD16 = 1 )


/*Section  : Data Type Declarations */
typedef struct{
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void(*TMR1_InterruptHandler_Callback)(void);
    interrupt_priority_cfg priority; 
#endif
uint8  timer1_mode : 1 ;
uint8  timer1_counter_mode : 1 ;
uint8  timer1_rw_register_size : 1 ;
uint8  timer1_oscillator_cfg : 1 ;
uint8  timer1_prescalar_value : 2 ;
uint16 timer1_preload_value ; 
uint8  reserved : 2 ; 
}timer1_t; 
/*Section  : Function Declarations */

Std_ReturnType Timer1_Init(const timer1_t * _timer1);
Std_ReturnType Timer1_DeInit(const timer1_t * _timer1);
Std_ReturnType Timer1_Write_Value(const timer1_t * _timer1 , uint16 _value);
Std_ReturnType Timer1_Read_Value(const timer1_t * _timer1 , uint16 * _value);

#endif	/* HAL_TIMER1_H */

