/* 
 * File:   hal_timer2.h
 * Author: intel
 *
 * Created on August 22, 2023, 11:44 AM
 */

#ifndef HAL_TIMER2_H
#define	HAL_TIMER2_H
/*Section  : Includes */

#include "../Interrupt/mcal_internal_interuupt.h"
#include "../GPIO/hal_gpio.h"

/*Section  : Macro Declarations */

#define TIMER2_ENABLE_CFG  1
#define TIMER2_DISABLE_CFG 0

#define TIMER2_PRESCALAR_VALUE_DIV_1   0
#define TIMER2_PRESCALAR_VALUE_DIV_4   1
#define TIMER2_PRESCALAR_VALUE_DIV_16  2

#define TIMER2_POSTSCALAR_VALUE_DIV_1  0
#define TIMER2_POSTSCALAR_VALUE_DIV_2  1
#define TIMER2_POSTSCALAR_VALUE_DIV_3  2
#define TIMER2_POSTSCALAR_VALUE_DIV_4  3
#define TIMER2_POSTSCALAR_VALUE_DIV_5  4
#define TIMER2_POSTSCALAR_VALUE_DIV_6  5
#define TIMER2_POSTSCALAR_VALUE_DIV_7  6
#define TIMER2_POSTSCALAR_VALUE_DIV_8  7
#define TIMER2_POSTSCALAR_VALUE_DIV_9  8
#define TIMER2_POSTSCALAR_VALUE_DIV_10 9
#define TIMER2_POSTSCALAR_VALUE_DIV_11 10
#define TIMER2_POSTSCALAR_VALUE_DIV_12 11
#define TIMER2_POSTSCALAR_VALUE_DIV_13 12
#define TIMER2_POSTSCALAR_VALUE_DIV_14 13
#define TIMER2_POSTSCALAR_VALUE_DIV_15 14
#define TIMER2_POSTSCALAR_VALUE_DIV_16 15


/*Section  : Macro Functions Declarations */
/*1 = Timer2 is on */
#define TIMER2_MODULE_ENABLE()    (T2CONbits.TMR2ON = 1)
/*0 = Timer2 is off */
#define TIMER2_MODULE_DISABLE()    (T2CONbits.TMR2ON = 0)

#define TIMER2_PRESCALAR_VAL_SELECT(PRESCALAR_VAL)     (T2CONbits.T2CKPS = PRESCALAR_VAL)
#define TIMER2_POSTCALAR_VAL_SELECT(POSTCALAR_VAL)     (T2CONbits.TOUTPS = POSTCALAR_VAL)
/*Section  : Data Type Declarations */

typedef struct{
#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void(*TMR2_InterruptHandler_Callback)(void);
    interrupt_priority_cfg priority; 
#endif
    uint8 timer2_preload_value;
    uint8 timer2_prescalar_value : 2 ;
    uint8 timer2_postscalar_value : 4 ;
    uint8 reserved;
}timer2_t;

/*Section  : Function Declarations */
Std_ReturnType Timer2_Init(const timer2_t * _timer);
Std_ReturnType Timer2_DeInit(const timer2_t * _timer);
Std_ReturnType Timer2_Write_Value(const timer2_t * _timer , uint8 _value);
Std_ReturnType Timer2_Read_Value(const timer2_t * _timer , uint8 * _value);



#endif	/* HAL_TIMER2_H */

