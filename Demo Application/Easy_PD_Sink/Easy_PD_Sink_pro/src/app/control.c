#include "control.h"


easy_PD_control_t easyPDControl = 
{
    .setVoltage = PD_VOLTAGE_5V,
    .VBUS_OUT_ONorOFF = 0,  /* 1->ON, 0->OFF*/
    .downBottonMode = 0  /* 1->1ong, 0->short */
};

void configure_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(CONFIGURE_1_CLK|CONFIGURE_2_CLK|CONFIGURE_3_CLK, ENABLE );
    GPIO_InitStructure.GPIO_Pin = CONFIGURE_1_PIN;   /* configure 1*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(CONFIGURE_1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = CONFIGURE_2_PIN;  /* configure 2 */
    GPIO_Init(CONFIGURE_2_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = CONFIGURE_3_PIN;  /* configure 3 */
    GPIO_Init(CONFIGURE_3_PORT, &GPIO_InitStructure);
}


void configure_voltage(PD_voltage_t configureVoltage)
{
    switch(configureVoltage)
    {
        case PD_VOLTAGE_5V: //100
            GPIO_SetBits(CONFIGURE_1_PORT,CONFIGURE_1_PIN);
            GPIO_ResetBits(CONFIGURE_2_PORT,CONFIGURE_2_PIN);
            GPIO_ResetBits(CONFIGURE_3_PORT,CONFIGURE_3_PIN);
            break;

        case PD_VOLTAGE_9V: //000
            GPIO_ResetBits(CONFIGURE_1_PORT,CONFIGURE_1_PIN);
            GPIO_ResetBits(CONFIGURE_2_PORT,CONFIGURE_2_PIN);
            GPIO_ResetBits(CONFIGURE_3_PORT,CONFIGURE_3_PIN);
            break;

        case PD_VOLTAGE_12V: //001
            GPIO_ResetBits(CONFIGURE_1_PORT,CONFIGURE_1_PIN);
            GPIO_ResetBits(CONFIGURE_2_PORT,CONFIGURE_2_PIN);
            GPIO_SetBits(CONFIGURE_3_PORT,CONFIGURE_3_PIN);
            break;

        case PD_VOLTAGE_15V: //011
            GPIO_ResetBits(CONFIGURE_1_PORT,CONFIGURE_1_PIN);
            GPIO_SetBits(CONFIGURE_2_PORT,CONFIGURE_2_PIN);
            GPIO_SetBits(CONFIGURE_3_PORT,CONFIGURE_3_PIN);
            break;

        case PD_VOLTAGE_20V: //010
            GPIO_ResetBits(CONFIGURE_1_PORT,CONFIGURE_1_PIN);
            GPIO_SetBits(CONFIGURE_2_PORT,CONFIGURE_2_PIN);
            GPIO_ResetBits(CONFIGURE_3_PORT,CONFIGURE_3_PIN);
            break;
        
        default:
            break;                 
    }
}

/**
 * @brief PMOS+NPN  PC4
 *        High level enable
 * 
 */
void vbus_out_switch_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(VBUS_OUT_SWITCH_CLK, ENABLE );
    GPIO_InitStructure.GPIO_Pin = VBUS_OUT_SWITCH_PIN;   /* configure 1*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(VBUS_OUT_SWITCH_PORT, &GPIO_InitStructure);

    VBUS_OUT_DISABLE(); /* disable vbus out*/
}



