/* 
 * File:   mcal_interuupt_manager.c
 * Author: intel
 *
 * Created on July 31, 2023, 10:15 PM
 */

#include "mcal_interuupt_manager.h"

static volatile uint8 RB4_Flag = 1, RB5_Flag = 1, RB6_Flag = 1, RB7_Flag = 1;

#if INTERRUPT_PRIORITY_LEVELS_ENABLE==INTERRUPT_FEATURE_ENABLE

void __interrupt() InterruptManagerHigh(void){
    if(INTCONbits.INT0IE == 1 && INTCONbits.INT0IF == 1){
        INT0_ISR();
    }else {
    }
    if(INTCON3bits.INT1IE == 1 && INTCON3bits.INT1IF == 1){
        INT1_ISR();
    }else {
    }
    if(INTCON3bits.INT2IE == 1 && INTCON3bits.INT2IF  == 1){
        INT2_ISR();
    }else{
    }
    
}
void __interrupt(low_priority) InterruptManagerLow(void){  
    if(INTCON3bits.INT1IE == 1 && INTCON3bits.INT1IF == 1){
        INT1_ISR();
    }else {
    }
    if(INTCON3bits.INT2IE == 1 && INTCON3bits.INT2IF  == 1){
        INT2_ISR();
    }else{
    }
}

#else 
void __interrupt() InterruptManager(void){
   if(INTCONbits.INT0IE == 1 && INTCONbits.INT0IF == 1){
        INT0_ISR();
    }else {
    }
    if(INTCON3bits.INT1IE == 1 && INTCON3bits.INT1IF == 1){
        INT1_ISR();
    }else {
    }
    if(INTCON3bits.INT2IE == 1 && INTCON3bits.INT2IF  == 1){
        INT2_ISR();
    }else{
    }
   
   if((INTCONbits.RBIE == 1 )&& (INTCONbits.RBIF == 1) && (PORTBbits.RB4 = 1)&&(RB4_Flag == 1)){
        RB4_Flag = 0 ;
        RB4_ISR(0);
    }else{
    }
   if((INTCONbits.RBIE == 1 )&& (INTCONbits.RBIF == 1) && (PORTBbits.RB4 = 0)&&(RB4_Flag == 0)){
        RB4_Flag = 1 ;
        RB4_ISR(1);
    }else{
    }
     if((INTCONbits.RBIE == 1 )&& (INTCONbits.RBIF == 1) && (PORTBbits.RB5 = 1)&&(RB5_Flag == 1)){
          RB5_Flag = 0 ;
        RB5_ISR(0);
    }else{
    }
   if((INTCONbits.RBIE == 1 )&& (INTCONbits.RBIF == 1) && (PORTBbits.RB5 = 0)&&(RB5_Flag == 0)){
        RB5_Flag = 1 ;
        RB5_ISR(1);
    }else{
    }
    if((INTCONbits.RBIE == 1 )&& (INTCONbits.RBIF == 1) && (PORTBbits.RB6 = 1)&&(RB6_Flag == 1)){
         RB6_Flag = 0 ;
        RB6_ISR(0);
        
    }else{
    }
   if((INTCONbits.RBIE == 1 )&& (INTCONbits.RBIF == 1) && (PORTBbits.RB6 = 0)&&(RB6_Flag ==0)){
       RB6_Flag = 1;
        RB6_ISR(1);
        
    }else{
    }
    if((INTCONbits.RBIE == 1 )&& (INTCONbits.RBIF == 1) && (PORTBbits.RB7 = 1)&&(RB7_Flag == 1)){
         RB7_Flag = 0;
        RB7_ISR(0);
    }else{
    }
   if((INTCONbits.RBIE == 1 )&& (INTCONbits.RBIF == 1) && (PORTBbits.RB7 = 0)&&(RB7_Flag == 0)){
        RB7_ISR(1);
         RB7_Flag = 1;
    }else{
    }
   
   if((PIE1bits.ADIE = 1 )&& (PIR1bits.ADIF = 1)){
       ADC_ISR();
    }else{
       
    }
   
   
   
}

#endif