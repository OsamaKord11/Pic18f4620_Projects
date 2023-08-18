/* 
 * File:   hal_eeprom.c
 * Author: intel
 *
 * Created on August 6, 2023, 4:54 PM
 */
#include "hal_eeprom.h"
Std_ReturnType Data_EEPROM_WriteByte(uint16 bAdd , uint8 bData){
    Std_ReturnType ret = E_NOT_OK; 
    /*Read The Interrupt Status (Enabled Or Disabled)*/
    uint8 Global_Interrupt_Status = INTCONbits.GIE ;
    /*Update The Address Registers*/
    EEADRH =(uint8)((bAdd >> 8) & 0x03); /* ex : badd = 0x038f((00000011 10001111) >> 8 ) --> (00000000 00000011) & 0x03(00000000 00000011) ==> 00000000 00000011 ==> uint8(00000000 00000011) = 000000011 = High byte of address*/
    EEADR  = (uint8)(bAdd & 0xff); /* ex : badd = 0x038f((00000011 10001111) & 0x00000000 11111111 ==>0x00000000 10001111 ==> uint8(00000000 10001111)= 10001111 = Low byte of address */
    /*Update The Data Register*/
    EEDATA = bData ;
    /*Select Access Data EEPROM memory*/
    EECON1bits.EEPGD = ACCESS_EEPROM_PROGRAM_MEMROY ;
    EECON1bits.CFGS = ACCESS_FLASH_EEPROM_MEMORY ;
    /*Allows Write Cycles to Flash Program / Data EEPROM*/
    EECON1bits.WREN = ALLOW_WRITE_CYCLES_FLASH_EEPROM ; 
    /*Disable all interrupts(General interrupt)*/
    INTERRUPT_GLOBALINTERRUPTDISABLE();
    /*Write the required sequence:ox55 then 0xAA */
    EECON2 = 0x55 ;
    EECON2 = 0xAA ; 
    /*Initiate data EEPrOM erase /write cycle */
    EECON1bits.WR = INITIATE_DATA_EEPROM_WRITE_ERASE;
    /*Wait for write to complete*/
    while(EECON1bits.WR);
    /*Inhibits Write Cycles to Flash Program / Data EEPROM*/
     EECON1bits.WREN = INHIBIT_WRITE_CYCLES_FLASH_EEPROM ; 
    /*Restore The Interrupt Status (Enabled Or Disabled)*/
    INTCONbits.GIE = Global_Interrupt_Status ;
    return ret ; 
    
}
Std_ReturnType Data_EEPROM_ReadByte(uint16 bAdd , uint8* bData){
    Std_ReturnType ret = E_NOT_OK; 
    if (bData == NULL){
         ret = E_NOT_OK;
    }
    else{
        /*Update The Address Registers*/
        EEADRH =(uint8)((bAdd >> 8) & 0x03); /* ex : badd = 0x038f((00000011 10001111) >> 8 ) --> (00000000 00000011) & 0x03(00000000 00000011) ==> 00000000 00000011 ==> uint8(00000000 00000011) = 000000011 = High byte of address*/
        EEADR  = (uint8)(bAdd & 0xff); /* ex : badd = 0x038f((00000011 10001111) & 0x00000000 11111111 ==>0x00000000 10001111 ==> uint8(00000000 10001111)= 10001111 = Low byte of address */
        /*Select Access Data EEPROM memory*/
        EECON1bits.EEPGD = ACCESS_EEPROM_PROGRAM_MEMROY ;
        EECON1bits.CFGS = ACCESS_FLASH_EEPROM_MEMORY ;
        /*Initiate data EEPrOM read cycle */
        EECON1bits.RD = INITIATE_DATA_EEPROM_READ;
        NOP();                  /*No Operation ==> preferred in high freq. (Crystal 40GHz)*/ 
        NOP();
        /*return data by Pointer indirectly*/
        *bData = EEDATA ;          /* notice in read and write both we use  EEDATA*/ 
        
        ret = E_OK;
    }   
    return ret ;
    
}