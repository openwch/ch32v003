#ifndef _CONFIGURE_H_
#define _CONFIGURE_H_

#include "debug.h"


/* configure_1  ->  PC2 */ 
#define  CONFIGURE_1_CLK         RCC_APB2Periph_GPIOC
#define  CONFIGURE_1_PORT        GPIOC
#define  CONFIGURE_1_PIN         GPIO_Pin_2

/* configure_2  ->  PC1 */ 
#define  CONFIGURE_2_CLK         RCC_APB2Periph_GPIOC
#define  CONFIGURE_2_PORT        GPIOC
#define  CONFIGURE_2_PIN         GPIO_Pin_1

/* configure_3  ->  PC0 */ 
#define  CONFIGURE_3_CLK         RCC_APB2Periph_GPIOC
#define  CONFIGURE_3_PORT        GPIOC
#define  CONFIGURE_3_PIN         GPIO_Pin_0

/* vbus_out_switch -> PC4 */
#define  VBUS_OUT_SWITCH_CLK     RCC_APB2Periph_GPIOC
#define  VBUS_OUT_SWITCH_PORT    GPIOC
#define  VBUS_OUT_SWITCH_PIN     GPIO_Pin_4


#define VBUS_OUT_DISABLE()    (VBUS_OUT_SWITCH_PORT->BCR = VBUS_OUT_SWITCH_PIN)
#define VBUS_OUT_ENABLE()     (VBUS_OUT_SWITCH_PORT->BSHR = VBUS_OUT_SWITCH_PIN)

typedef enum
{
    PD_VOLTAGE_5V = 1,
    PD_VOLTAGE_9V,
    PD_VOLTAGE_12V,
    PD_VOLTAGE_15V,
    PD_VOLTAGE_20V  
}PD_voltage_t;



typedef struct 
{
    volatile uint8_t setVoltage;
    volatile uint8_t downBottonMode;
    volatile uint8_t VBUS_OUT_ONorOFF;
}easy_PD_control_t;


extern easy_PD_control_t easyPDControl;

extern void configure_gpio_init(void);
extern void configure_voltage(PD_voltage_t configureVoltage);
extern void vbus_out_switch_gpio_init(void);

#endif /* end of configure.h */
