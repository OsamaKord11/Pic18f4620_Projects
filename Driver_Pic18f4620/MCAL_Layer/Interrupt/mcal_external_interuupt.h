/* 
 * File:   mcal_external_interuupt.h
 * Author: intel
 *
 * Created on July 31, 2023, 10:14 PM
 */

#ifndef MCAL_EXTERNAL_INTERUUPT_H
#define	MCAL_EXTERNAL_INTERUUPT_H

/*Section  : Includes */
#include "mcal_interuupt_config.h"
/*Section  : Macro Declarations */


/*Section  : Macro Functions Declarations */

#if EXTERNAL_INTERRUPT_INTx_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE 

#define EXT_INT0_INTERRUPTENABLE()         (INTCONbits.INT0IE = 1)
#define EXT_INT0_INTERRUPTDISABLE()        (INTCONbits.INT0IE = 0)

#define EXT_INT0_INTERRUPTFLAGCLEAR()      (INTCONbits.INT0IF = 0)

#define EXT_INT0_RISINGEDGESET()           (INTCON2bits.INTEDG0 = 1)
#define EXT_INT0_FALLINGEDGESET()          (INTCON2bits.INTEDG0 = 0)

#define EXT_INT1_INTERRUPTENABLE()         (INTCON3bits.INT1E = 1)
#define EXT_INT1_INTERRUPTDISABLE()        (INTCON3bits.INT1E = 0)

#define EXT_INT1_INTERRUPTFLAGCLEAR()      (INTCON3bits.INT1F = 0)

#define EXT_INT1_RISINGEDGESET()           (INTCON2bits.INTEDG1 = 1)
#define EXT_INT1_FALLINGEDGESET()          (INTCON2bits.INTEDG1 = 0)

#define EXT_INT2_INTERRUPTENABLE()         (INTCON3bits.INT2E = 1)
#define EXT_INT2_INTERRUPTDISABLE()        (INTCON3bits.INT2E = 0)

#define EXT_INT2_INTERRUPTFLAGCLEAR()      (INTCON3bits.INT2IF = 0)

#define EXT_INT2_RISINGEDGESET()           (INTCON2bits.INTEDG2 = 1)
#define EXT_INT2_FALLINGEDGESET()          (INTCON2bits.INTEDG2 = 0)

/* nested #if*/
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE

#define EXT_INT1_HIGHPRIORITYSET() (INTCON3bits.INT1IP = 1)
#define EXT_INT1_LOWPRIORITYSET()  (INTCON3bits.INT1IP = 0)

#define EXT_INT2_HIGHPRIORITYSET() (INTCON3bits.INT2IP = 1)
#define EXT_INT2_LOWPRIORITYSET()  (INTCON3bits.INT2IP = 0)


#endif

#endif

#if EXTERNAL_INTERRUPT_OnChange_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE 

#define EXT_RBx_INTERRUPTENABLE()         (INTCONbits.RBIE = 1)
#define EXT_RBx_INTERRUPTDISABLE()        (INTCONbits.RBIE = 0)

#define EXT_RBx_INTERRUPTFLAGCLEAR()      (INTCONbits.RBIF = 0)

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE

#define EXT_RBx_HIGHPRIORITY()           (INTCON2bits.RBIP = 1)
#define EXT_RBx_LOWPRIORITY()            (INTCON2bits.RBIP = 0)

#endif
#endif
/*Section  : Data Type Declarations */

typedef enum {
    INTERRUPT_FALLING_EDGE = 0 ,
    INTERRUPT_RISING_EDGE        
}interrupt_INTx_edge;

typedef enum {
    INTERRUPT_EXTERNAL_INT0 = 0 ,
    INTERRUPT_EXTERNAL_INT1 ,
    INTERRUPT_EXTERNAL_INT2        
}interrupt_INTx_src;

typedef struct{
    /*to make the above layer know that there is an interrupt coming from the below
    layer we use "call back" function(void and void) that called the indirect call*/
    void(*EXT_Interrupt_Handler)(void);
    pin_config_t pin ;  
    interrupt_INTx_edge edge ; 
    interrupt_INTx_src source ; 
    interrupt_priority_cfg priority ;
}interrupt_INTx_t;
   
typedef struct{
    void(*EXT_Interrupt_Handler_HIGH)(void);
    void(*EXT_Interrupt_Handler_LOW)(void);
    pin_config_t pin ;
    interrupt_priority_cfg priority ;
}interrupt_RBx_t;

/*Section  : Function Declarations */
Std_ReturnType Interrupt_INTx_Init(const interrupt_INTx_t* int_obj);
Std_ReturnType Interrupt_INTx_DeInit(const interrupt_INTx_t* int_obj);

Std_ReturnType Interrupt_RBx_Init(const interrupt_RBx_t* int_obj);
Std_ReturnType Interrupt_RBx_DeInit(const interrupt_RBx_t* int_obj);

#endif	/* MCAL_EXTERNAL_INTERUUPT_H */

