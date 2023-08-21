/* 
 * File:   hal_Timer0.h
 * Author: intel
 *
 * Created on August 19, 2023, 9:22 PM
 */
#include"hal_Timer0.h"

static uint16 preload_value = 0 ; // to fill the register with preloaded value again ,after overflow occured 

static inline void Timer0_prescaler_config(const timer0_t * _timer0);
static inline void Timer0_select_mode_config(const timer0_t * _timer0);
static inline void Timer0_select_register_size(const timer0_t * _timer0);


#if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
   static void (*TMR0_Interrupt_Handler)(void) = NULL;  
#endif

Std_ReturnType Timer0_Init(const timer0_t * _timer0){
    Std_ReturnType ret = E_NOT_OK ; 
    if(_timer0 == NULL){
        ret = E_NOT_OK ; 
    }
    else{
        TIMER0_MODULE_DISABLE();
        Timer0_prescaler_config(_timer0);
        Timer0_select_mode_config(_timer0);
        Timer0_select_register_size(_timer0);
        //Must Preloaded(write) first , based on data sheet
        TMR0H = ((_timer0->timer0_preload_value) >> 8);
        // it preloaded (written) after TMR0H , but is read before it
        TMR0L = (uint8)(_timer0->timer0_preload_value); 
        /* to fill the register with preloaded value again ,after has overflow occured 
        we save a copy of preloaded value ,once overflow has occured, fill the register again  */
        preload_value = _timer0->timer0_preload_value;
#if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER0_INTERRUPTENABLE();
        TIMER0_INTERRUPTFLAGCLEAR();
        TMR0_Interrupt_Handler = _timer0->Timer0_InterruptHandler ;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PRIORITYLEVELSENABLE();
        if(INTERRUPT_HIGH_PRIORITY == _timer0->priority){
            INTERRUPT_GLOBALINTERRUPTHIGHENABLE();
            TIMER0_HIGHPRIORITYSET();
        } 
        else if(INTERRUPT_LOW_PRIORITY == _timer0->priority){
            INTERRUPT_GLOBALINTERRUPTLOWENABLE();
            TIMER0_LOWPRIORITYSET();
        }
        else{
            //No thing else
        }
#else 
        INTERRUPT_GLOBALINTERRUPTENABLE();
        INTERRUPT_PERIPHERALINTERRUPTENABLE();
#endif
#endif
        TIMER0_MODULE_ENABLE();
    }
    return ret ; 
}
Std_ReturnType Timer0_DeInit(const timer0_t * _timer0){
    Std_ReturnType ret = E_NOT_OK ; 
    if(_timer0 == NULL){
        ret = E_NOT_OK ; 
    }
    else{
     TIMER0_MODULE_DISABLE() ;
     
#if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    TIMER0_INTERRUPTDISABLE();
    
#endif 
       
    }
    return ret ; 
}
Std_ReturnType Timer0_Write_Value(const timer0_t * _timer0 , uint16 _value){
    Std_ReturnType ret = E_NOT_OK ; 
    if(_timer0 == NULL){
        ret = E_NOT_OK ; 
    }
    else{
        TMR0H = ((_value) >> 8);   //Must Written first , based on data sheet
        TMR0L = (uint8)(_value);
    }
    return ret ; 
}
Std_ReturnType Timer0_Read_Value(const timer0_t * _timer0 , uint16 * _value){
    Std_ReturnType ret = E_NOT_OK ; 
   uint8 l_tmr0l = 0 , l_tmr0h = 0 ;
    if((_timer0 == NULL)||(_value == NULL)){
        ret = E_NOT_OK ; 
    }
    else{
     l_tmr0l = TMR0L ;
     l_tmr0h = TMR0H ;  
      * _value =(uint16)((l_tmr0h << 8) + l_tmr0l);
    }
    return ret ; 
}


static inline void Timer0_prescaler_config(const timer0_t * _timer0) {
    if(TIMER0_PRESCALAR_ENABLE_CFG == _timer0->prescalar_enable){
        TIMER0_PRESCALAR_ENABLE();
        T0CONbits.T0PS = _timer0->prescalar_value ; 
    }
    else if(TIMER0_PRESCALAR_DISABLE_CFG == _timer0->prescalar_enable){
        TIMER0_PRESCALAR_DISABLE();
    }
    else{
        //NO thing else
    } 
}

static inline void Timer0_select_mode_config(const timer0_t * _timer0){
    if (TIMER0_TIMER_MODE_CFG == _timer0->timer0_mode){
        TIMER0_TIMER_MODE_ENABLE();
    }
    else if(TIMER0_COUNTER_MODE_CFG == _timer0->timer0_mode){
        TIMER0_COUNTER_MODE_ENABLE();
            if(TIMER0_RISING_EDGE_CFG == _timer0->timer0_counter_edge){
                TIMER0_RISING_EDGE();
            }
            else if(TIMER0_FALLING_EDGE_CFG == _timer0->timer0_counter_edge){
                TIMER0_FALLING_EDGE();
            }
            else{
                //NO thing else
            }
    }
    else{
        //NO thing else
    }
}

static inline void Timer0_select_register_size(const timer0_t * _timer0){
    if(TIMER0_8bit_REGISTER_CFG == _timer0 ->rgister_size){
        TIMER0_8BIT_REG_MODE_ENABLE();
    }
    else if(TIMER0_16bit_REGISTER_CFG == _timer0 ->rgister_size){
        TIMER0_16BIT_REG_MODE_ENABLE();
    }
    else{
        //NO thing else
    } 
}

void TMR0_ISR(void){
    TIMER0_INTERRUPTFLAGCLEAR();
    /*to fill the register with preloaded value again ,after overflow has occured.
     note : overflow happens when the register is maximum filled , and also with 
     each overflow there is an interrupt is fired*/ 
    TMR0H = preload_value >> 8;
    TMR0L = (uint8)(preload_value);
    if(TMR0_Interrupt_Handler){
        TMR0_Interrupt_Handler();
    }
}