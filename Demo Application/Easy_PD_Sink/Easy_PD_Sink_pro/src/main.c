

#include "debug.h"
#include "button.h"
#include "control.h"
#include "led.h"
#include "adc_monitor_voltage.h"

/**
 * @brief 
 * 
 * @return int 
 */
int main(void)
{
    Delay_Init();
    USART_Printf_Init(115200);
    printf("ch32v003 hello world!\r\n");

    vbus_out_switch_gpio_init();

    configure_gpio_init();
    configure_voltage(PD_VOLTAGE_5V); //default 5v

    my_button_init();

    led_gpio_init();
    led_turn_off_all();
    
    adc_init();

    while(1)
    {
        if(easyPDControl.downBottonMode)
        {
            led_blink(easyPDControl.setVoltage);
            Delay_Ms(200);
        }
    }
}

