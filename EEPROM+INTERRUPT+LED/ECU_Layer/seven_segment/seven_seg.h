/* 
 * File:   seven_seg.h
 * Author: intel
 *
 * Created on July 19, 2023, 1:37 PM
 */

#ifndef SEVEN_SEG_H
#define	SEVEN_SEG_H


/*Section  : Includes */
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include"seven_Seg_cfg.h"


/*Section  : Macro Declarations */

#define SEVEN_SEGMENT_PIN0   0
#define SEVEN_SEGMENT_PIN1   1
#define SEVEN_SEGMENT_PIN2   2
#define SEVEN_SEGMENT_PIN3   3


/*Section  : Macro Functions Declarations */

/*Section  : Data Type Declarations */

typedef enum {
    COMMON_ANODE = 0 ,
    COMMON_CATHODE      
}seven_segment_type_t;

typedef struct {
    pin_config_t segment_pins[4];
    seven_segment_type_t segment_type;
}segment_t;
/*Section  : Function Declarations */

Std_ReturnType seven_segment_initialize(const segment_t* seg);
Std_ReturnType seven_segment_write_number(const segment_t* seg , uint8 number);

#endif SEVEN_SEG_H