#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "debug.h"

/* left button  -> PC7 */
#define  BUTTON_LEFT_CLK    RCC_APB2Periph_GPIOC
#define  BUTTON_LEFT_PORT   GPIOC
#define  BUTTON_LEFT_PIN    GPIO_Pin_7

/* right button -> PC6 */
#define  BUTTON_RIGHT_CLK   RCC_APB2Periph_GPIOC
#define  BUTTON_RIGHT_PORT  GPIOC
#define  BUTTON_RIGHT_PIN   GPIO_Pin_6

/* down button  -> PC5 */
#define  BUTTON_DOWN_CLK    RCC_APB2Periph_GPIOC
#define  BUTTON_DOWN_PORT   GPIOC
#define  BUTTON_DOWN_PIN    GPIO_Pin_5


/* button id enum */
typedef enum
{
    BUTTON_LEFT_ID = 0,
    BUTTON_RIGHT_ID,
    BUTTON_DOWN_ID
}button_id_t;


extern void my_button_init(void);

#endif /* end of button.h*/
