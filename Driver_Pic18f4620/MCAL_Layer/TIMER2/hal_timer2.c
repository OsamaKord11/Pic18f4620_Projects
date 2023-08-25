/* 
 * File:   hal_timer2.c
 * Author: Osama Kord
 *
 * Created on August 22, 2023, 11:44 AM
 */
#include "hal_timer2.h"

#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
   static void (*TMR2_Interrupt_Handler)(void) = NULL;  
#endif
   static volatile uint32 preload_val = 0 ; 
Std_ReturnType Timer2_Init(const timer2_t * _timer){
    Std_ReturnType ret = E_NOT_OK ;
    if(NULL == _timer){
        ret = E_NOT_OK ;
    }
    else{
       TIMER2_MODULE_DISABLE();
       TIMER2_PRESCALAR_VAL_SELECT(_timer->timer2_prescalar_value);
       TIMER2_POSTCALAR_VAL_SELECT(_timer->timer2_postscalar_value);
       TMR2 = _timer ->timer2_preload_value ; 
       preload_val = _timer ->timer2_preload_value ; 
       
#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER2_INTERRUPTENABLE();
        TIMER2_INTERRUPTFLAGCLEAR();
        TMR2_Interrupt_Handler = _timer->TMR2_InterruptHandler_Callback;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PRIORITYLEVELSENABLE();
        if(INTERRUPT_HIGH_PRIORITY == _timer->priority){
            INTERRUPT_GLOBALINTERRUPTHIGHENABLE();
            TIMER2_HIGHPRIORITYSET();
        } 
        else if(INTERRUPT_LOW_PRIORITY == _timer->priority){
            INTERRUPT_GLOBALINTERRUPTLOWENABLE();
            TIMER2_LOWPRIORITYSET();
        }
        else{
            //No thing else
        }
#else 
        INTERRUPT_GLOBALINTERRUPTENABLE();
        INTERRUPT_PERIPHERALINTERRUPTENABLE();
#endif
#endif 
       TIMER2_MODULE_ENABLE();
    }
    return ret ; 
}
Std_ReturnType Timer2_DeInit(const timer2_t * _timer){
    Std_ReturnType ret = E_NOT_OK ;
    if(NULL == _timer){
        ret = E_NOT_OK ;
    }
    else{
       TIMER2_MODULE_DISABLE();
#if TIMER2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER2_INTERRUPTDISABLE();
#endif         
    }
    return ret ; 
}
Std_ReturnType Timer2_Write_Value(const timer2_t * _timer , uint8 _value){
    Std_ReturnType ret = E_NOT_OK ;
    if(NULL == _timer){
        ret = E_NOT_OK ;
    }
    else{
       TMR2 = _value; 
    }
    return ret ; 
}
Std_ReturnType Timer2_Read_Value(const timer2_t * _timer , uint8 * _value){
    Std_ReturnType ret = E_NOT_OK ;
    uint8 l_tmr2 = 0;
    if(NULL == _timer){
        ret = E_NOT_OK ;
    }
    else{
        //* _value = TMR2; or
       l_tmr2 = TMR2;
       * _value = l_tmr2;
    }
    return ret ; 
}

 void TMR2_ISR(void){
    TIMER2_INTERRUPTFLAGCLEAR();
    if(TMR2_Interrupt_Handler){
        TMR2_Interrupt_Handler();
    }
 }