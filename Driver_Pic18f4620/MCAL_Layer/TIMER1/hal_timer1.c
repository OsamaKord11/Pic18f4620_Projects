/* 
 * File:   hal_timer1.h
 * Author: intel
 *
 * Created on August 21, 2023, 8:55 AM
 */

#include "hal_timer1.h"
static volatile uint16 preload_value = 0 ;

static inline void Timer1_select_mode_config(const timer1_t * _timer1);
static inline void Timer1_select_rw_reg_size(const timer1_t * _timer1); // mostly 16 but i configured it
static inline void Timer1_select_oscillator(const timer1_t * _timer1); // the default is disabled

#if TIMER0_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
   static void (*TMR1_Interrupt_Handler)(void) = NULL;  
#endif


Std_ReturnType Timer1_Init(const timer1_t * _timer1){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL==_timer1){
        ret = E_NOT_OK;
    }else{
        TIMER1_MODULE_DISABLE();
        TIMER1_PRESCALAR_SELECT(_timer1->timer1_prescalar_value);
        Timer1_select_mode_config(_timer1);     
        Timer1_select_rw_reg_size(_timer1);
        Timer1_select_oscillator(_timer1);
        TMR1H = ((_timer1->timer1_preload_value) >> 8);
        TMR1L = (uint8)(_timer1->timer1_preload_value);
        preload_value = _timer1->timer1_preload_value;
  
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER1_INTERRUPTENABLE();
        TIMER1_INTERRUPTFLAGCLEAR();
        TMR1_Interrupt_Handler = _timer1->TMR1_InterruptHandler_Callback;
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PRIORITYLEVELSENABLE();
        if(INTERRUPT_HIGH_PRIORITY == _timer1->priority){
            INTERRUPT_GLOBALINTERRUPTHIGHENABLE();
            TIMER1_HIGHPRIORITYSET();
        } 
        else if(INTERRUPT_LOW_PRIORITY == _timer1->priority){
            INTERRUPT_GLOBALINTERRUPTLOWENABLE();
            TIMER1_LOWPRIORITYSET();
        }
        else{
            //No thing else
        }
#else 
        INTERRUPT_GLOBALINTERRUPTENABLE();
        INTERRUPT_PERIPHERALINTERRUPTENABLE();
#endif
#endif
        TIMER1_MODULE_ENABLE();
    }
    return ret ; 
}
Std_ReturnType Timer1_DeInit(const timer1_t * _timer1){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL==_timer1){
        ret = E_NOT_OK;
    }else{
        TIMER1_MODULE_DISABLE();
#if TIMER1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        TIMER1_INTERRUPTDISABLE();
#endif 
    }
    return ret ; 
}
Std_ReturnType Timer1_Write_Value(const timer1_t * _timer1 , uint16 _value){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL==_timer1){
        ret = E_NOT_OK;
    }else{
        TMR1H = (_value) >> 8 ;
        TMR1L = (uint8)_value;
    }
    return ret ; 
}
Std_ReturnType Timer1_Read_Value(const timer1_t * _timer1 , uint16 * _value){
    Std_ReturnType ret = E_NOT_OK;
    uint8 l_tmr1l = 0, l_tmr1h = 0 ;
    if((NULL==_timer1)||(NULL==_value)){
        ret = E_NOT_OK;
    }else{
       l_tmr1l = TMR1L;
       l_tmr1h = TMR1H;
       * _value =(uint16)(l_tmr1l + (l_tmr1h << 8));
    }
    return ret ; 
}

static inline void Timer1_select_mode_config(const timer1_t * _timer1){
    if(_timer1->timer1_mode == TIMER1_TIMER_MODE_CFG){
        TIMER1_TIMER_MODE_ENABLE();
    }else if(_timer1->timer1_mode == TIMER1_COUNTER_MODE_CFG){
      TIMER1_COUNTER_MODE_ENABLE();
      if(_timer1->timer1_counter_mode ==TIMER1_ASYNC_COUNTER_CFG){
        TIMER1_ASYNC_COUNTER_ENABLE();
      }
      else if(_timer1->timer1_counter_mode ==TIMER1_SYNC_COUNTER_CFG){
        TIMER1_SYNC_COUNTER_ENABLE();
      }
      else{
        //NO thing else 
      }
    }
    else{
        //NO thing else
    }
}
  void TMR1_ISR(void){
      TIMER1_INTERRUPTFLAGCLEAR();
      TMR1H = (preload_value >> 8);
      TMR1L = (uint8)(preload_value);
      if(TMR1_Interrupt_Handler){
          TMR1_Interrupt_Handler();
      }
  }
static inline void Timer1_select_rw_reg_size(const timer1_t * _timer1){
    if(_timer1->timer1_rw_register_size == TIMER1_8bit_RW_REGISTER_CFG){
        TIMER1_8BIT_RW_REG_MODE_ENABLE();
    }
    else if(_timer1->timer1_rw_register_size == TIMER1_16bit_RW_REGISTER_CFG){
        TIMER1_16BIT_RW_REG_MODE_ENABLE();
    }
    else{
       //NO thing else 
    }
}

static inline void Timer1_select_oscillator(const timer1_t * _timer1){
    if(_timer1->timer1_oscillator_cfg == TIMER1_OSC_ENABLE_CFG){
        TIMER1_OSCILLATOR_ENABLE() ;
    }
    else if(_timer1->timer1_oscillator_cfg == TIMER1_OSC_DISABLE_CFG){
        TIMER1_OSCILLATOR_DISABLE(); 
    }
    else{
       //NO thing else 
    }
}

