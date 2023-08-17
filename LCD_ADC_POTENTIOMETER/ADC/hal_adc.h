/* 
 * File:   hal_adc.h
 * Author: OSAMA KORD
 *
 * Created on August 12, 2023, 5:01 PM
 */

#ifndef HAL_ADC_H
#define	HAL_ADC_H


/*Section  : Includes */

#include "hal_adc_cfg.h"
#include "../GPIO/hal_gpio.h"
#include "../Interrupt/mcal_internal_interuupt.h"

/*Section  : Macro Declarations */

/**
* @brief A/D Port Configuration Control bits
*
* @note if ADC_AN5_ANALOG_FUNCTIONALITY is selected it means
* that AN5 , AN4 , AN3 , AN2 , AN1 , AN0 are all working as
* analog input channels for ADC  
*/
#define ADC_AN0_ANALOG_FUNCTIONALITY  0x0E
#define ADC_AN1_ANALOG_FUNCTIONALITY  0x0D
#define ADC_AN2_ANALOG_FUNCTIONALITY  0x0C
#define ADC_AN3_ANALOG_FUNCTIONALITY  0x0B
#define ADC_AN4_ANALOG_FUNCTIONALITY  0x0A
#define ADC_AN5_ANALOG_FUNCTIONALITY  0x09
#define ADC_AN6_ANALOG_FUNCTIONALITY  0x08
#define ADC_AN7_ANALOG_FUNCTIONALITY  0x07
#define ADC_AN8_ANALOG_FUNCTIONALITY  0x06
#define ADC_AN9_ANALOG_FUNCTIONALITY  0x05
#define ADC_AN10_ANALOG_FUNCTIONALITY 0x04
#define ADC_AN11_ANALOG_FUNCTIONALITY 0x03
#define ADC_AN12_ANALOG_FUNCTIONALITY 0x02
#define ADC_ALL_ANALOG_FUNCTIONALITY  0x00
#define ADC_ALL_DIGITAL_FUNCTIONALITY 0x0F


#define ADC_RESULT_RIGHT  0x01U
#define ADC_RESULT_LEFT   0x00U
#define ADC_VOLTAGEREFERENCEENABLE    0x01U
#define ADC_VOLTAGEREFERENCEDISABLE   0x00U
#define ADC_CONVERSION_COMPLETED    1
#define ADC_CONVERSION_IN_PROGRESS  0
/*Section  : Macro Functions Declarations */

/*
 * ADC conversion Status:
 * 1 = A/D conversion in progress 
 * 0 = A/D Idle 
 */
#define ADC_CONVERSION_STATUS() (ADCON0bits.GO_nDONE)

/*Before starting conversion ,ADON must be 1 */
#define ADC_START_CONVERSION()  (ADCON0bits.GODONE = 1)


/*
 * ADON: A/D On bit 
 *1 = A/D Converter module is enabled
 *0 = A/D Converter module is disabled
 */
#define ADC_ENABLE()  (ADCON0bits.ADON = 1)
#define ADC_DISABLE() (ADCON0bits.ADON = 0)

/*
 * VCFG1: Voltage Reference Configuration bit (VREF- source)
 *1 = VREF-   (AN2)
 *0 = VSS
 *VCFG0: Voltage Reference Configuration bit (VREF+ source)
 *1 = VREF+    (AN3)
 *0 = VDD
 */
#define ADC_VOLTAGE_REFERENCE_ENABLE()   do{ ADCON1bits.VCFG1 = 1 ;\
                                             ADCON1bits.VCFG0 = 1 ;\
                                                }while(0);                                                         
#define ADC_VOLTAGE_REFERENCE_DISABLE()  do{ ADCON1bits.VCFG1 = 0 ;\
                                             ADCON1bits.VCFG0 = 0 ;\
                                                }while(0);

/*ex: ADC_ANALOG_DIGITAL_PORT_CONFIG(ADC_AN4_ANALOG_FUNCTIONALITY);
 *when ADC_AN4_ANALOG_FUNCTIONALITY is configured ,this means that AN4 ,AN3 ,
 * AN2 ,AN1 , AN0 are analog functionality
 */
#define ADC_ANALOG_DIGITAL_PORT_CONFIG(_CONFIG)      (ADCON1bits.PCFG = _CONFIG)


#define ADC_RESULT_FORMAT_RIGHT() (ADCON2bits.ADFM = 1)
#define ADC_RESULT_FORMAT_LEFT()  (ADCON2bits.ADFM = 0)




/*Section  : Data Type Declarations */
typedef enum{
    ADC_CHANNEL0_AN0 = 0,
    ADC_CHANNEL1_AN1,
    ADC_CHANNEL2_AN2,
    ADC_CHANNEL3_AN3,
    ADC_CHANNEL4_AN4,
    ADC_CHANNEL5_AN5,
    ADC_CHANNEL6_AN6,
    ADC_CHANNEL7_AN7,
    ADC_CHANNEL8_AN8,
    ADC_CHANNEL9_AN9,
    ADC_CHANNEL10_AN10,
    ADC_CHANNEL11_AN11,
    ADC_CHANNEL12_AN12               
}adc_channel_select_t;

typedef enum{
    ADC_0_TAD = 0 ,
    ADC_2_TAD,
    ADC_4_TAD,
    ADC_6_TAD,
    ADC_8_TAD,        
    ADC_12_TAD,        
    ADC_16_TAD,
    ADC_20_TAD
}adc_acquisition_time_t;

typedef enum{
    ADC_CONVERSION_CLOCK_FOSC_DIV_2 = 0 ,
    ADC_CONVERSION_CLOCK_FOSC_DIV_8,
    ADC_CONVERSION_CLOCK_FOSC_DIV_32,
    ADC_CONVERSION_CLOCK_FOSC_DIV_FRC,
    ADC_CONVERSION_CLOCK_FOSC_DIV_4,
    ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    ADC_CONVERSION_CLOCK_FOSC_DIV_64
}adc_conversion_clock_t;

typedef struct{
#if ADC_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
    void(*ADC_Interrupt_handler)(void);
    interrupt_priority_cfg priority ;
#endif
    adc_acquisition_time_t acquisition_time ;
    adc_conversion_clock_t conversion_clock;
    adc_channel_select_t channel;
    uint8 voltage_reference : 1 ; 
    uint8 result_format : 1 ;  
    uint8 reserved : 6 ;  
}adc_conf_t;
/*Section  : Functions Declarations */

Std_ReturnType ADC_Initialize(const adc_conf_t * _adc);
Std_ReturnType ADC_Deinitialize(const adc_conf_t * _adc);
Std_ReturnType ADC_Select_Channel(const adc_conf_t * _adc ,adc_channel_select_t channel);
Std_ReturnType ADC_Start_Conversion(const adc_conf_t * _adc);
Std_ReturnType ADC_Is_Conversion_Done(const adc_conf_t * _adc , uint8 *Conversion_Status);
Std_ReturnType ADC_Get_Conversion_Result(const adc_conf_t * _adc , uint16 *Conversion_Result);
Std_ReturnType ADC_Get_Conversion_Blocking(const adc_conf_t * _adc ,adc_channel_select_t channel, uint16 *conversion_result);
Std_ReturnType ADC_Start_Conversion_Interrupt(const adc_conf_t * _adc ,adc_channel_select_t channel);

#endif	/* HAL_ADC_H */

