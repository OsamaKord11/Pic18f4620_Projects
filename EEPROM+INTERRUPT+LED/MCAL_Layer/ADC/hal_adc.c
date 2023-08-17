/* 
 * File:   hal_adc_c
 * Author: OSAMA KORD
 *
 * Created on August 12, 2023, 5:02 PM
 */
#include "hal_adc.h"
#if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
   static void (*ADC_interrupt_handler)(void) = NULL ;  
#endif
static  void adc_input_channel_port_configure(adc_channel_select_t channel);
static  void select_result_format(const adc_conf_t * _adc);
static  void configure_voltage_reference(const adc_conf_t * _adc);

/**
 * @brief
 * @param _adc
 * @return 
 */
Std_ReturnType ADC_Initialize(const adc_conf_t * _adc){
    Std_ReturnType ret = E_NOT_OK ; 
    if (NULL == _adc){
        ret = E_NOT_OK ; 
    }
    else{
        ADC_DISABLE();
        /*configure the acquisition time */
        ADCON2bits.ACQT = _adc->acquisition_time ;
        /*configure the conversion clock*/
        ADCON2bits.ADCS = _adc->conversion_clock ; 
        /*configure the default channel*/
        ADCON0bits.CHS = _adc->channel ;      // After that, the pin where the channel is ,must be configured as input  
        adc_input_channel_port_configure(_adc->channel); // that is the default channel
        /*configure the interrupt*/
#if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        INTERRUPT_GLOBALINTERRUPTENABLE();
        INTERRUPT_PERIPHERALINTERRUPTENABLE();
        ADC_INTERRUPTENABLE();
        ADC_INTERRUPTFLAGCLEAR();
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
        if (_adc->priority == INTERRUPT_HIGH_PRIORITY){
            ADC_HIGHPRIORITYSET();
        }else if (_adc->priority == INTERRUPT_LOW_PRIORITY){
            ADC_LOWPRIORITYSET();
        }
        else{
            /*No thing*/
        }
#endif
      ADC_interrupt_handler = _adc->ADC_Interrupt_handler ; 
#endif
        /*configure the result format */
        select_result_format(_adc);
        /*configure the voltage reference*/
        configure_voltage_reference(_adc);
        /*Enable the ADC*/
        ADC_ENABLE();

        ret = E_OK ;
    }
    return ret ;
}
/**
 * @brief
 * @param _adc
 * @return 
 */
Std_ReturnType ADC_Deinitialize(const adc_conf_t * _adc){
    Std_ReturnType ret = E_NOT_OK ; 
    if (NULL == _adc){
        ret = E_NOT_OK ; 
    }
    else{
       /*Disable The ADC*/
        ADC_DISABLE();
        /*configure the interrupt*/
#if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
      ADC_INTERRUPTDISABLE();
#endif       
        ret = E_OK ;
    }
    return ret ;
}
/**
 * @brief
 * @param _adc
 * @param channel
 * @return 
 */
Std_ReturnType ADC_Select_Channel(const adc_conf_t * _adc ,adc_channel_select_t channel){
    Std_ReturnType ret = E_NOT_OK ; 
    if (NULL == _adc){
        ret = E_NOT_OK ; 
    }
    else{
       /*configure the default channel*/
        ADCON0bits.CHS = channel ;     // After that, the pin where the channel is ,must be configured as input  
        adc_input_channel_port_configure(channel);  // that is the selected channel which you want to change during run time 
        ret = E_OK ;
    }
    return ret ;
}
/**
 * @brief
 * @param _adc
 * @return 
 */
Std_ReturnType ADC_Start_Conversion(const adc_conf_t * _adc){
    Std_ReturnType ret = E_NOT_OK ; 
    if (NULL == _adc){
        ret = E_NOT_OK ; 
    }
    else{
        ADC_START_CONVERSION();
        ret = E_OK ;
    }
    return ret ;
}
/**
 * @brief
 * @param _adc
 * @param Conversion_Status true(1):if conversion is completed
 *                          false(0):if conversion is not completed
 * @return 
 */
Std_ReturnType ADC_Is_Conversion_Done(const adc_conf_t * _adc , uint8 *Conversion_Status){
    Std_ReturnType ret = E_NOT_OK ; 
    if ((NULL == _adc) || (NULL == Conversion_Status)){
        ret = E_NOT_OK ; 
    }
    else{
    /* if it is in progress that means(ADCON0bits.GO_nDONE = 1), then invert it to zero and write it in Conversion_Status 
     as indication that conversion is not completed yet and it is in progress , else if it is idle
     (ADCON0bits.GO_nDONE = 0) invert 0 --> 1 and write the 1 in Conversion_Status as indication that conversion is completed */  
    *Conversion_Status = (uint8)(!(ADCON0bits.GO_nDONE));
        ret = E_OK ;
    }
    return ret ;
}
/**
 * @brief
 * @param _adc
 * @param Conversion_Result
 * @return 
 */
Std_ReturnType ADC_Get_Conversion_Result(const adc_conf_t * _adc , uint16 *Conversion_Result){
    Std_ReturnType ret = E_NOT_OK ; 
    if ((NULL == _adc)||(NULL == Conversion_Result)){
        ret = E_NOT_OK ; 
    }
    else{
        if (_adc->result_format ==ADC_RESULT_RIGHT){
        *Conversion_Result =(uint16)((ADRESH << 8) + ADRESL);
        }
        else if (_adc->result_format ==ADC_RESULT_LEFT){
            *Conversion_Result =(uint16)(((ADRESH << 8) + ADRESL) >> 6);
        }
        else{
           *Conversion_Result =(uint16)((ADRESH << 8) + ADRESL);
        }
            ret = E_OK ;
        }
    return ret ;
}
/**
 * @brief
 * @param _adc
 * @param channel
 * @param conversion_status
 * @return 
 */
Std_ReturnType ADC_Get_Conversion_Blocking(const adc_conf_t * _adc ,adc_channel_select_t channel, uint16 *conversion_result){
    Std_ReturnType ret = E_NOT_OK ; 
    uint8 l_conversion_status = 0 ;
    if ((NULL == _adc)||(NULL == conversion_result)){
         ret = E_NOT_OK ; 
    }
    else{
       ret = ADC_Select_Channel(_adc ,channel);
       ret = ADC_Start_Conversion(_adc);
       ret = ADC_Is_Conversion_Done(_adc, &l_conversion_status);
       while(ADCON0bits.GO_nDONE);   //it will block the code until the GO_nDONE = 0 which means that condition is false which means that conversion terminated 
       ret = ADC_Get_Conversion_Result(_adc,conversion_result); // as soon as the conversion terminated,get the result 
    }  
    return ret ;
}
/**
 * 
 * @param _adc
 * @param channel
 * @return 
 */
Std_ReturnType ADC_Start_Conversion_Interrupt(const adc_conf_t * _adc ,adc_channel_select_t channel){
    Std_ReturnType ret = E_NOT_OK ; 
    if (NULL == _adc){
         ret = E_NOT_OK ; 
    }
    else{
       ret = ADC_Select_Channel(_adc ,channel);
       ret = ADC_Start_Conversion(_adc);
    }  
    return ret ;
}

static  void adc_input_channel_port_configure(adc_channel_select_t channel){
    switch(channel){
        case ADC_CHANNEL0_AN0 : SET_BIT(TRISA, _TRISA_RA0_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL1_AN1 : SET_BIT(TRISA, _TRISA_RA1_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL2_AN2 : SET_BIT(TRISA, _TRISA_RA2_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL3_AN3 : SET_BIT(TRISA, _TRISA_RA3_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL4_AN4 : SET_BIT(TRISA, _TRISA_RA5_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL5_AN5 : SET_BIT(TRISE, _TRISE_RE0_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL6_AN6 : SET_BIT(TRISE, _TRISE_RE1_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL7_AN7 : SET_BIT(TRISE, _TRISE_RE2_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL8_AN8 : SET_BIT(TRISB, _TRISB_RB2_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL9_AN9 : SET_BIT(TRISB, _TRISB_RB3_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL10_AN10 : SET_BIT(TRISB, _TRISB_RB1_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL11_AN11 : SET_BIT(TRISB, _TRISB_RB4_POSN); break; /* Disable the digital output driver */
        case ADC_CHANNEL12_AN12 : SET_BIT(TRISB, _TRISB_RB0_POSN); break; /* Disable the digital output driver */
        default : /* Nothing */;
    }
}


static  void select_result_format(const adc_conf_t * _adc){
    if (_adc->result_format ==ADC_RESULT_RIGHT){
        ADC_RESULT_FORMAT_RIGHT();
    }
    else if (_adc->result_format ==ADC_RESULT_LEFT){
        ADC_RESULT_FORMAT_LEFT();
    }
    else{
       ADC_RESULT_FORMAT_RIGHT();
    }
}

static  void configure_voltage_reference(const adc_conf_t * _adc){
    if (_adc->voltage_reference ==ADC_VOLTAGEREFERENCEENABLE){
        ADC_VOLTAGE_REFERENCE_ENABLE();
    }
    else if (_adc->voltage_reference ==ADC_VOLTAGEREFERENCEDISABLE){
        ADC_VOLTAGE_REFERENCE_DISABLE();
    }
    else{
       ADC_VOLTAGE_REFERENCE_DISABLE();
    }
}

void ADC_ISR (void){
    ADC_INTERRUPTFLAGCLEAR();
    if(ADC_interrupt_handler){
        ADC_interrupt_handler();
    }
}