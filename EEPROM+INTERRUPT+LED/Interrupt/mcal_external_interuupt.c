/* 
 * File:   mcal_external_interuupt.c
 * Author: intel
 *
 * Created on July 31, 2023, 10:14 PM
 */

#include "mcal_external_interuupt.h"


/*these helper functions are implemented to reduce the change that you may  
  make in the code if you decided to use another microcontroller , to make 
  the change in priority helper function or edge helper function ...etc not to change the initialization function*/
static Std_ReturnType interrupt_INTx_Enable(const interrupt_INTx_t* int_obj);
static Std_ReturnType interrupt_INTx_Disable(const interrupt_INTx_t* int_obj);
static Std_ReturnType interrupt_INTx_Priority_Init(const interrupt_INTx_t* int_obj);
static Std_ReturnType interrupt_INTx_Edge_Init(const interrupt_INTx_t* int_obj);
static Std_ReturnType interrupt_INTx_Pin_Init(const interrupt_INTx_t* int_obj);
static Std_ReturnType interrupt_INTx_Clear_Flag(const interrupt_INTx_t* int_obj);

static Std_ReturnType interrupt_RBx_Enable(const interrupt_RBx_t* int_obj);
static Std_ReturnType interrupt_RBx_Disable(const interrupt_RBx_t* int_obj);
static Std_ReturnType interrupt_RBx_Priority_Init(const interrupt_RBx_t* int_obj);
static Std_ReturnType interrupt_RBx_Pin_Init(const interrupt_RBx_t* int_obj);

static void(* INT0_Interrupt_Handler)(void) = NULL ; 
static void(* INT1_Interrupt_Handler)(void) = NULL ; 
static void(* INT2_Interrupt_Handler)(void) = NULL ; 

static void (*RB4_Interrupt_Handler_HIGH)(void) = NULL;
static void (*RB4_Interrupt_Handler_LOW)(void)  = NULL;
static void (*RB5_Interrupt_Handler_HIGH)(void) = NULL;
static void (*RB5_Interrupt_Handler_LOW)(void)  = NULL;
static void (*RB6_Interrupt_Handler_HIGH)(void) = NULL;
static void (*RB6_Interrupt_Handler_LOW)(void)  = NULL;
static void (*RB7_Interrupt_Handler_HIGH)(void) = NULL;
static void (*RB7_Interrupt_Handler_LOW)(void)  = NULL;

static Std_ReturnType INT0_SetInterruptHandler(void(*Interrupt_Handler)(void));
static Std_ReturnType INT1_SetInterruptHandler(void(*Interrupt_Handler)(void));
static Std_ReturnType INT2_SetInterruptHandler(void(*Interrupt_Handler)(void));
static Std_ReturnType interrupt_INTx_SetInterruptHandler(const interrupt_INTx_t* int_obj);
/**
 * @brief
 * @param int_obj
 * @return 
 */
Std_ReturnType Interrupt_INTx_Init(const interrupt_INTx_t* int_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (int_obj == NULL) {
            ret = E_NOT_OK;
    } else {
        /*disable the external interrupt*/
         ret = interrupt_INTx_Disable(int_obj);
        /*clear the external interrupt flag : did not occur*/
         ret = interrupt_INTx_Clear_Flag(int_obj);
        /*configure the external interrupt edge*/
         ret = interrupt_INTx_Edge_Init(int_obj);
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE 
        /*configure the external interrupt Priority*/
         ret = interrupt_INTx_Priority_Init(int_obj);
#endif
        /*configure the external interrupt I/O pin*/
         ret = interrupt_INTx_Pin_Init(int_obj);
        /*configure the default interrupt call back*/
         ret = interrupt_INTx_SetInterruptHandler(int_obj);
        /*enable the external interrupt*/
         ret = interrupt_INTx_Enable(int_obj);
    }
    return ret;
}

void INT0_ISR(void){
    /*The INT0 external interrupt happened(must be cleared in software)*/
    EXT_INT0_INTERRUPTFLAGCLEAR();
    /*code*/
    /*call back function gets called every time this ISR executes*/
    if(INT0_Interrupt_Handler)
    {
        INT0_Interrupt_Handler();
    }
}
void INT1_ISR(void){
    /*The INT1 external interrupt happened(must be cleared in software)*/
    EXT_INT1_INTERRUPTFLAGCLEAR();
    /*code*/
    /*call back function gets called every time this ISR executes*/
    if(INT1_Interrupt_Handler)
    {
        INT1_Interrupt_Handler();
    }
}
void INT2_ISR(void){
    /*The INT2 external interrupt happened(must be cleared in software)*/
    EXT_INT2_INTERRUPTFLAGCLEAR();
    /*code*/
    /*call back function gets called every time this ISR executes*/
    if(INT2_Interrupt_Handler)
    {
        INT2_Interrupt_Handler();
    }
}
void RB4_ISR(uint8 RB4_Source){
    EXT_RBx_INTERRUPTFLAGCLEAR();
    if (0 == RB4_Source){
        if (RB4_Interrupt_Handler_HIGH){
            RB4_Interrupt_Handler_HIGH();
    }
    }
    else if (1 == RB4_Source){
        if (RB4_Interrupt_Handler_LOW){
            RB4_Interrupt_Handler_LOW();
}
}
}
void RB5_ISR(uint8 RB5_Source){
    EXT_RBx_INTERRUPTFLAGCLEAR();
     if (0 == RB5_Source){
        if (RB5_Interrupt_Handler_HIGH){
            RB5_Interrupt_Handler_HIGH();
    }
     }
     else if (1 == RB5_Source){
        if (RB5_Interrupt_Handler_LOW){
            RB5_Interrupt_Handler_LOW();
}
}  
}
void RB6_ISR(uint8 RB6_Source){
    EXT_RBx_INTERRUPTFLAGCLEAR();
    if (0==RB6_Source){
        if (RB6_Interrupt_Handler_HIGH){
            RB6_Interrupt_Handler_HIGH();
    }
    }
    else if (1==RB6_Source){
            if (RB6_Interrupt_Handler_LOW){
                RB6_Interrupt_Handler_LOW();
}
}
}
void RB7_ISR(uint8 RB7_Source){
    EXT_RBx_INTERRUPTFLAGCLEAR();
    if (0==RB7_Source){
        if (RB7_Interrupt_Handler_HIGH){
            RB7_Interrupt_Handler_HIGH();
    }
    }
    else if (1 == RB7_Source){
            if (RB7_Interrupt_Handler_LOW){
                 RB7_Interrupt_Handler_LOW(); 
        }
    }
}

/**
 * @brief
 * @param int_obj
 * @return 
 */
Std_ReturnType Interrupt_INTx_DeInit(const interrupt_INTx_t* int_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (int_obj == NULL) {
              ret = E_NOT_OK;
    } else {
            ret = interrupt_INTx_Disable(int_obj);
    }

    return ret;
}
/**
 * @brief
 * @param int_obj
 * @return 
 */
Std_ReturnType Interrupt_RBx_Init(const interrupt_RBx_t* int_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (int_obj == NULL) {
          ret = E_NOT_OK;
    } else {
          EXT_RBx_INTERRUPTDISABLE();
          EXT_RBx_INTERRUPTFLAGCLEAR();
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
          INTERRUPT_PRIORITYLEVELSENABLE();
          if(int_obj->priority==INTERRUPT_HIGH_PRIORITY){
          INTERRUPT_GLOBALINTERRUPTHIGHENABLE();
           EXT_RBx_HIGHPRIORITY();
          }
          else if (int_obj->priority==INTERRUPT_LOW_PRIORITY){
          INTERRUPT_GLOBALINTERRUPTLOWENABLE();  
           EXT_RBx_LOWPRIORITY();
          }
          else{
          }
#else 
          INTERRUPT_GLOBALINTERRUPTENABLE();
          INTERRUPT_PERIPHERALINTERRUPTENABLE();
                  
#endif
         ret = gpio_pin_direction_initialize(&(int_obj->pin));
          switch(int_obj->pin.pin){
            case GPIO_PIN4 : 
                RB4_Interrupt_Handler_HIGH = int_obj->EXT_Interrupt_Handler_HIGH; 
                RB4_Interrupt_Handler_LOW = int_obj->EXT_Interrupt_Handler_LOW; 
            break;
            case GPIO_PIN5 : 
                RB5_Interrupt_Handler_HIGH = int_obj->EXT_Interrupt_Handler_HIGH;
                RB5_Interrupt_Handler_LOW = int_obj->EXT_Interrupt_Handler_LOW;
            break;
            case GPIO_PIN6 : 
                RB6_Interrupt_Handler_HIGH = int_obj->EXT_Interrupt_Handler_HIGH;
                RB6_Interrupt_Handler_LOW = int_obj->EXT_Interrupt_Handler_LOW;
            break;
            case GPIO_PIN7 : 
                RB7_Interrupt_Handler_HIGH = int_obj->EXT_Interrupt_Handler_HIGH;
                RB7_Interrupt_Handler_LOW = int_obj->EXT_Interrupt_Handler_LOW;
            break;
            default: ret = E_NOT_OK ;
          }  
          
          EXT_RBx_INTERRUPTENABLE();
    }

    return ret;
}
/**
 * @brief
 * @param int_obj
 * @return 
 */
Std_ReturnType Interrupt_RBx_DeInit(const interrupt_RBx_t* int_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (int_obj == NULL) {
              ret = E_NOT_OK;
    } else {
       
    }
    return ret;
}
/**
 * @brief
 * @param int_obj
 * @return 
 */
static Std_ReturnType interrupt_INTx_Enable(const interrupt_INTx_t* int_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (int_obj == NULL) {
               ret = E_NOT_OK;
    } else {
        switch (int_obj->source) {
            case INTERRUPT_EXTERNAL_INT0:
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
                INTERRUPT_GLOBALINTERRUPTHIGHENABLE();
#else
                INTERRUPT_GLOBALINTERRUPTENABLE();
                INTERRUPT_PERIPHERALINTERRUPTENABLE();
#endif 
                EXT_INT0_INTERRUPTENABLE();
                ret = E_OK;
                break;
            case INTERRUPT_EXTERNAL_INT1: 
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
               INTERRUPT_PRIORITYLEVELSENABLE();
               if (int_obj->priority == INTERRUPT_LOW_PRIORITY){
                    INTERRUPT_GLOBALINTERRUPTLOWENABLE();
                }
                else if(int_obj->priority == INTERRUPT_HIGH_PRIORITY){
                    INTERRUPT_GLOBALINTERRUPTHIGHENABLE();
                }
                else {
                }
#else 
                INTERRUPT_GLOBALINTERRUPTENABLE();
                INTERRUPT_PERIPHERALINTERRUPTENABLE();          
#endif
                EXT_INT1_INTERRUPTENABLE();
                ret = E_OK;
                break;
            case INTERRUPT_EXTERNAL_INT2: 
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
                INTERRUPT_PRIORITYLEVELSENABLE();
                if (int_obj->priority == INTERRUPT_LOW_PRIORITY){
                    INTERRUPT_GLOBALINTERRUPTLOWENABLE();
                }
                else if(int_obj->priority == INTERRUPT_HIGH_PRIORITY){
                    INTERRUPT_GLOBALINTERRUPTHIGHENABLE();
                }
                else {
                }           
#else 
                INTERRUPT_GLOBALINTERRUPTENABLE();
                INTERRUPT_PERIPHERALINTERRUPTENABLE();    
#endif
                EXT_INT2_INTERRUPTENABLE();
                ret = E_OK;
                break;
            default: ret = E_NOT_OK;
        }
    }
    return ret;
}

/**
 * @brief
 * @param int_obj
 * @return 
 */
static Std_ReturnType interrupt_INTx_Disable(const interrupt_INTx_t* int_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (int_obj == NULL) {
             ret = E_NOT_OK;
    } else {
        switch (int_obj->source) {
            case INTERRUPT_EXTERNAL_INT0: EXT_INT0_INTERRUPTDISABLE();
                ret = E_OK;
                break;
            case INTERRUPT_EXTERNAL_INT1: EXT_INT1_INTERRUPTDISABLE();
                ret = E_OK;
                break;
            case INTERRUPT_EXTERNAL_INT2: EXT_INT2_INTERRUPTDISABLE();
                ret = E_OK;
                break;
            default: ret = E_NOT_OK;
        }
    }
    return ret;
}

/**
 * @brief
 * @param int_obj
 * @return 
 */
#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE
static Std_ReturnType interrupt_INTx_Priority_Init(const interrupt_INTx_t* int_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (int_obj == NULL) {
                  ret = E_NOT_OK;
    } else {
        switch (int_obj->source) {
            case INTERRUPT_EXTERNAL_INT1:
                if (int_obj->priority == INTERRUPT_HIGH_PRIORITY) {
                    EXT_INT1_HIGHPRIORITYSET();
                } else if (int_obj->priority == INTERRUPT_LOW_PRIORITY) {
                    EXT_INT1_LOWPRIORITYSET();
                } else {
                    /*No Thing*/
                }
                ret = E_OK;
                break;

            case INTERRUPT_EXTERNAL_INT2:
                if (int_obj->priority == INTERRUPT_HIGH_PRIORITY) {
                    EXT_INT2_HIGHPRIORITYSET();
                } else if (int_obj->priority == INTERRUPT_LOW_PRIORITY) {
                    EXT_INT2_LOWPRIORITYSET();
                } else {
                    /*No Thing*/
                }
                ret = E_OK;
                break;
            default: ret = E_NOT_OK;
        }
    }
    return ret;
}
#endif
/**
 * @brief
 * @param int_obj
 * @return 
 */
static Std_ReturnType interrupt_INTx_Edge_Init(const interrupt_INTx_t* int_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (int_obj == NULL) {
     ret = E_NOT_OK;
    } else {
switch (int_obj->source) {
            case INTERRUPT_EXTERNAL_INT0:
                if (int_obj->edge == INTERRUPT_RISING_EDGE) {
                   EXT_INT0_RISINGEDGESET();
                } else if (int_obj->edge == INTERRUPT_FALLING_EDGE) {
                   EXT_INT0_FALLINGEDGESET();
                } else {
                    /*No Thing*/
                }
                ret = E_OK;
                break;
            case INTERRUPT_EXTERNAL_INT1:
                if (int_obj->edge == INTERRUPT_RISING_EDGE) {
                   EXT_INT1_RISINGEDGESET();
                } else if (int_obj->edge == INTERRUPT_FALLING_EDGE) {
                   EXT_INT1_FALLINGEDGESET();
                } else {
                    /*No Thing*/
                }
                ret = E_OK;
                break;

            case INTERRUPT_EXTERNAL_INT2:
                if (int_obj->edge == INTERRUPT_RISING_EDGE) {
                    EXT_INT2_RISINGEDGESET();
                } else if (int_obj->edge == INTERRUPT_FALLING_EDGE) {
                   EXT_INT2_FALLINGEDGESET();
                } else {
                    /*No Thing*/
                }
                ret = E_OK;
                break;
            default: ret = E_NOT_OK;
        }
    }
    return ret;
}

/**
 * @brief
 * @param int_obj
 * @return 
 */
static Std_ReturnType interrupt_INTx_Pin_Init(const interrupt_INTx_t* int_obj) {
    Std_ReturnType ret = E_NOT_OK;
    if (int_obj == NULL) {
            ret = E_NOT_OK;
    } else {
        ret = gpio_pin_direction_initialize(&(int_obj->pin));
    }
    return ret;
}
/**
 * @brief
 * @param int_obj
 * @return 
 */
static Std_ReturnType interrupt_INTx_Clear_Flag(const interrupt_INTx_t* int_obj){
    Std_ReturnType ret = E_NOT_OK ; 
    if (NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        switch (int_obj->source) {
            case INTERRUPT_EXTERNAL_INT0:
                EXT_INT0_INTERRUPTFLAGCLEAR();
                ret = E_OK;
                break;
            case INTERRUPT_EXTERNAL_INT1: 
                EXT_INT1_INTERRUPTFLAGCLEAR();    
                ret = E_OK;
                break;
            case INTERRUPT_EXTERNAL_INT2: 
                EXT_INT2_INTERRUPTFLAGCLEAR();
                ret = E_OK;
                break;
            default: ret = E_NOT_OK;
        }
        
    }
    return ret ;
}
/**
 * @brief
 * @param int_obj
 * @return 
 */
static Std_ReturnType interrupt_RBx_Enable(const interrupt_RBx_t* int_obj){
    Std_ReturnType ret = E_NOT_OK ; 
    if (NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        
    }
    
    return ret ;
}
/**
 * @brief
 * @param int_obj
 * @return 
 */
static Std_ReturnType interrupt_RBx_Disable(const interrupt_RBx_t* int_obj){
    Std_ReturnType ret = E_NOT_OK ; 
    if (NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        
    }
    
    return ret ;
}
/**
 * @brief
 * @param int_obj
 * @return 
 */
static Std_ReturnType interrupt_RBx_Priority_Init(const interrupt_RBx_t* int_obj){
    Std_ReturnType ret = E_NOT_OK ; 
    if (NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        
    }
    
    return ret ;
}
/**
 * @brief
 * @param int_obj
 * @return 
 */
static Std_ReturnType interrupt_RBx_Pin_Init(const interrupt_RBx_t* int_obj){
    Std_ReturnType ret = E_NOT_OK ; 
    if (NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        
    }
    
    return ret ;
}

static Std_ReturnType INT0_SetInterruptHandler(void(*Interrupt_Handler)(void)){
    Std_ReturnType ret = E_NOT_OK ; 
    if (NULL == Interrupt_Handler){
        ret = E_NOT_OK;
    }
    else{
        INT0_Interrupt_Handler= Interrupt_Handler;
         ret = E_OK;
    }
    return ret ;
}
static Std_ReturnType INT1_SetInterruptHandler(void(*Interrupt_Handler)(void)){
    Std_ReturnType ret = E_NOT_OK ; 
    if (NULL == Interrupt_Handler){
        ret = E_NOT_OK;
    }
    else{
        INT1_Interrupt_Handler= Interrupt_Handler;
         ret = E_OK;
    }
    return ret ;
}
static Std_ReturnType INT2_SetInterruptHandler(void(*Interrupt_Handler)(void)){
    Std_ReturnType ret = E_NOT_OK ; 
    if (NULL == Interrupt_Handler){
        ret = E_NOT_OK;
    }
    else{
        INT2_Interrupt_Handler= Interrupt_Handler;
         ret = E_OK;
    }
    return ret ;
}
static Std_ReturnType interrupt_INTx_SetInterruptHandler(const interrupt_INTx_t* int_obj){
     Std_ReturnType ret = E_NOT_OK ; 
    if (NULL == int_obj){
        ret = E_NOT_OK;
    }
    else{
        switch (int_obj->source) {
            case INTERRUPT_EXTERNAL_INT0:
               INT0_SetInterruptHandler(int_obj->EXT_Interrupt_Handler);
                ret = E_OK;
                break;
            case INTERRUPT_EXTERNAL_INT1: 
               INT1_SetInterruptHandler(int_obj->EXT_Interrupt_Handler);
                ret = E_OK;
                break;
            case INTERRUPT_EXTERNAL_INT2: 
               INT2_SetInterruptHandler(int_obj->EXT_Interrupt_Handler);
                ret = E_OK;
                break;
            default: ret = E_NOT_OK;
        }
        
    }
    return ret ;
}