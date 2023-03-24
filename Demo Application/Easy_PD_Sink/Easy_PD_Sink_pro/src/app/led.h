#ifndef _LED_H_
#define _LED_H_

#include "debug.h"
#include "control.h"

/* led 5v  ->  PD6 */ 
#define  LED_5v_CLK     RCC_APB2Periph_GPIOD
#define  LED_5v_PORT    GPIOD
#define  LED_5v_PIN     GPIO_Pin_6

/* led 9v ->   PD5 */
#define  LED_9v_CLK     RCC_APB2Periph_GPIOD
#define  LED_9v_PORT    GPIOD
#define  LED_9v_PIN     GPIO_Pin_5

/* led 12v  -> PD4 */
#define  LED_12v_CLK    RCC_APB2Periph_GPIOD
#define  LED_12v_PORT   GPIOD
#define  LED_12v_PIN    GPIO_Pin_4

/* led 15v  -> PD3 */
#define  LED_15v_CLK    RCC_APB2Periph_GPIOD
#define  LED_15v_PORT   GPIOD
#define  LED_15v_PIN    GPIO_Pin_3

/* led 20v  -> PD2 */
#define  LED_20v_CLK    RCC_APB2Periph_GPIOD
#define  LED_20v_PORT   GPIOD
#define  LED_20v_PIN    GPIO_Pin_2



extern void led_gpio_init(void);
extern void led_turn_off_all(void);
extern void led_turn_on(PD_voltage_t ledVlotageNum);
extern void led_blink(PD_voltage_t ledVlotageNum);

#endif /* end of led.h*/
