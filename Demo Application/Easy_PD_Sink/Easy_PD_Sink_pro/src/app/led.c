#include "led.h"

void led_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(LED_5v_CLK|LED_9v_CLK|LED_12v_CLK|LED_15v_CLK|LED_20v_CLK, ENABLE );
    GPIO_InitStructure.GPIO_Pin = LED_5v_PIN;   /* led 5v*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_5v_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED_9v_PIN;   /* led 9v*/
    GPIO_Init(LED_9v_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED_12v_PIN;   /* led 12v*/
    GPIO_Init(LED_12v_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED_15v_PIN;   /* led 15v*/
    GPIO_Init(LED_15v_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED_20v_PIN;   /* led 20v*/
    GPIO_Init(LED_20v_PORT, &GPIO_InitStructure);
    
}

void led_turn_off_all(void)
{
    GPIO_SetBits(LED_5v_PORT,LED_5v_PIN);
    GPIO_SetBits(LED_9v_PORT,LED_9v_PIN);
    GPIO_SetBits(LED_12v_PORT,LED_12v_PIN);
    GPIO_SetBits(LED_15v_PORT,LED_15v_PIN);
    GPIO_SetBits(LED_20v_PORT,LED_20v_PIN);
}

void led_turn_on(PD_voltage_t ledVlotageNum)
{
    led_turn_off_all();
    switch(ledVlotageNum)
    {
        case PD_VOLTAGE_5V:
            GPIO_ResetBits(LED_5v_PORT,LED_5v_PIN);
            break;

        case PD_VOLTAGE_9V:
            GPIO_ResetBits(LED_9v_PORT,LED_9v_PIN);
            break;

        case PD_VOLTAGE_12V:
            GPIO_ResetBits(LED_12v_PORT,LED_12v_PIN);
            break;

        case PD_VOLTAGE_15V:
            GPIO_ResetBits(LED_15v_PORT,LED_15v_PIN);
            break;

        case PD_VOLTAGE_20V:
            GPIO_ResetBits(LED_20v_PORT,LED_20v_PIN);
            break;

        default:
            break;
    }
}

void led_blink(PD_voltage_t ledVlotageNum)
{
    switch(ledVlotageNum)
    {
        case PD_VOLTAGE_5V:
            LED_5v_PORT->OUTDR^=LED_5v_PIN;
            break;

        case PD_VOLTAGE_9V:
            LED_9v_PORT->OUTDR^=LED_9v_PIN;
            break;

        case PD_VOLTAGE_12V:
            LED_12v_PORT->OUTDR^=LED_12v_PIN;
            break;

        case PD_VOLTAGE_15V:
            LED_15v_PORT->OUTDR^=LED_15v_PIN;
            break;

        case PD_VOLTAGE_20V:
            LED_20v_PORT->OUTDR^=LED_20v_PIN;
            break;

        default:
            break;
    }
}