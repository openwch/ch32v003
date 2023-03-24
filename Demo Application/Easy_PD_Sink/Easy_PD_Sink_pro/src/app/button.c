#include "button.h"
#include "multi_button.h"
#include "control.h"
#include "led.h"
#include "adc_monitor_voltage.h"

Button buttonLeft;
Button buttonRight;
Button buttonDown;


/**
 * record button down long press time,
 * exit switch voltage mode if there is no action within 3s 
 */
static volatile uint16_t buttonDownLongPressTimes = 0;

/**
 * @brief init button gpio
 * 
 */
void my_button_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(BUTTON_LEFT_CLK|BUTTON_RIGHT_CLK|BUTTON_DOWN_CLK, ENABLE );
    GPIO_InitStructure.GPIO_Pin = BUTTON_LEFT_PIN;   /* button left */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(BUTTON_LEFT_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = BUTTON_RIGHT_PIN;  /* button right */
    GPIO_Init(BUTTON_RIGHT_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = BUTTON_DOWN_PIN;   /* button down */
    GPIO_Init(BUTTON_DOWN_PORT, &GPIO_InitStructure);
}

uint8_t read_button_pin(uint8_t button_id)
{
    switch(button_id)
    {
        case BUTTON_LEFT_ID:
            return GPIO_ReadInputDataBit(BUTTON_LEFT_PORT,BUTTON_LEFT_PIN);
            break;

        case BUTTON_RIGHT_ID:
            return GPIO_ReadInputDataBit(BUTTON_RIGHT_PORT,BUTTON_RIGHT_PIN);
            break;

        case BUTTON_DOWN_ID:
            return GPIO_ReadInputDataBit(BUTTON_DOWN_PORT,BUTTON_DOWN_PIN);
            break;

        default:
            return 0;
            break;
    }
}


void timer2_init(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitStructure.TIM_Period = arr;
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_ARRPreloadConfig(TIM2, ENABLE);

    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

    NVIC_SetPriority(TIM2_IRQn, 0x80);
    NVIC_EnableIRQ(TIM2_IRQn);

    TIM_Cmd(TIM2,ENABLE);
}

void left_button_single_click_handler(void* btn)
{
    if(easyPDControl.downBottonMode)
    {
        led_turn_off_all();
        easyPDControl.setVoltage--;
        if(easyPDControl.setVoltage < PD_VOLTAGE_5V)
        {
            easyPDControl.setVoltage = PD_VOLTAGE_20V;
        }
        configure_voltage(easyPDControl.setVoltage);

        buttonDownLongPressTimes = 0;
    }  
}

void right_button_single_click_handler(void* btn)
{
    if(easyPDControl.downBottonMode)
    {
        led_turn_off_all();
        easyPDControl.setVoltage++;
        if(easyPDControl.setVoltage > PD_VOLTAGE_20V)
        {
            easyPDControl.setVoltage = PD_VOLTAGE_5V;
        }
        configure_voltage(easyPDControl.setVoltage);

        buttonDownLongPressTimes = 0;
    }
}

void down_button_single_click_handler(void* btn)
{
    led_turn_off_all();
    if(easyPDControl.downBottonMode)
    {
        
        easyPDControl.downBottonMode = 0;
        easyPDControl.VBUS_OUT_ONorOFF = 1;
        // led_turn_on(easyPDControl.setVoltage);

        /*enable adc dma, measure output voltage*/
        DMA1_Channel1->CFGR &= (uint16_t)(~DMA_CFGR1_EN);
        DMA1_Channel1->CNTR = ADC_DATA_LENGTH; 
        DMA1_Channel1->CFGR |= DMA_CFGR1_EN;   
        
        VBUS_OUT_ENABLE();
    }
    else
    {
        if(easyPDControl.VBUS_OUT_ONorOFF)
        {
            VBUS_OUT_DISABLE(); 
            easyPDControl.VBUS_OUT_ONorOFF = 0;
        }
        else
        {
            VBUS_OUT_ENABLE();
            // led_turn_on(easyPDControl.setVoltage);  

            /*enable adc dma, measure output voltage*/   
            DMA1_Channel1->CFGR &= (uint16_t)(~DMA_CFGR1_EN);
            DMA1_Channel1->CNTR = ADC_DATA_LENGTH; 
            DMA1_Channel1->CFGR |= DMA_CFGR1_EN;   

            easyPDControl.VBUS_OUT_ONorOFF = 1;
        }
    }
}


void down_button_long_press_handler(void* btn)
{

    if(easyPDControl.downBottonMode == 0)
    {
        // easyPDControl.setVoltage = easyPDControl.currentVoltage;  /*  */
        
        easyPDControl.downBottonMode = 1;
        VBUS_OUT_DISABLE(); 
        easyPDControl.VBUS_OUT_ONorOFF = 0;
    }

}

void my_button_init(void)
{
    my_button_gpio_init();

    /* init button object*/
    button_init(&buttonLeft,read_button_pin,0,BUTTON_LEFT_ID);
    button_attach(&buttonLeft,SINGLE_CLICK,left_button_single_click_handler);
    button_start(&buttonLeft);

    button_init(&buttonRight,read_button_pin,0,BUTTON_RIGHT_ID);
    button_attach(&buttonRight,SINGLE_CLICK,right_button_single_click_handler);
    button_start(&buttonRight);


    button_init(&buttonDown,read_button_pin,0,BUTTON_DOWN_ID);
    button_attach(&buttonDown,SINGLE_CLICK,down_button_single_click_handler);
    button_attach(&buttonDown,LONG_PRESS_START,down_button_long_press_handler);
    button_start(&buttonDown);


    timer2_init(5000,48-1); // 5ms, 200Hz
}


/**
 * @brief timer2 interrupt service routine
 * 
 */
__attribute__((interrupt("WCH-Interrupt-fast")))
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
    {
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
        
        /* check button action */
        button_ticks();

        /* exit switch voltage mode if there is no action within 3s  */
        if(easyPDControl.downBottonMode)
        {
            buttonDownLongPressTimes++; 
        }
        else
        {
            buttonDownLongPressTimes = 0;
        }

        if(buttonDownLongPressTimes>600) // 3s 
        {
            easyPDControl.downBottonMode = 0;
            led_turn_off_all();
            easyPDControl.downBottonMode = 0;
            easyPDControl.VBUS_OUT_ONorOFF = 1;
            // led_turn_on(easyPDControl.setVoltage);

            /*enable adc dma, measure output voltage*/
            DMA1_Channel1->CFGR &= (uint16_t)(~DMA_CFGR1_EN);
            DMA1_Channel1->CNTR = ADC_DATA_LENGTH; 
            DMA1_Channel1->CFGR |= DMA_CFGR1_EN;   

            VBUS_OUT_ENABLE();
        }   
    }
}

