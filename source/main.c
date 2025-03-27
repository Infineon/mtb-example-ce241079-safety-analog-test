/*******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for Analog peripheral Class-B safety Test
*              code example for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2024, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/


/*******************************************************************************
* Header Files
*******************************************************************************/
#include "cy_pdl.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "mtb_hal.h"
#include "self_test.h"
/*******************************************************************************
* Macros
*******************************************************************************/

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* For the Retarget -IO (Debug UART) usage */
static cy_stc_scb_uart_context_t    CYBSP_UART_context;           /** UART context */
static mtb_hal_uart_t               CYBSP_UART_hal_obj;           /** Debug UART HAL object  */

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/*******************************************************************************
* Function Definitions
*******************************************************************************/

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function. It performs Class-B safety test for Analog block.
* SelfTest is performed for ADC, and Comparator based on the user command.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;
    uint8_t command;

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /* Debug UART init */
    result = (cy_rslt_t)Cy_SCB_UART_Init(CYBSP_UART_HW, &CYBSP_UART_config, &CYBSP_UART_context);

    /* UART init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    Cy_SCB_UART_Enable(CYBSP_UART_HW);

    /* Setup the HAL UART */
    result = mtb_hal_uart_setup(&CYBSP_UART_hal_obj, &CYBSP_UART_hal_config, &CYBSP_UART_context, NULL);

    /* HAL UART init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* HAL retarget_io init */
    result = cy_retarget_io_init(&CYBSP_UART_hal_obj);

    /* HAL retarget_io init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");
    printf("****************** "
           "Class-B: Analog IP SAFETY TEST "
           "****************** \r\n\n");
    /* Display available commands */
    printf("Available commands \r\n");
    printf("1 : Run ADC test with 2*VDD/3 as input \r\n");
    printf("2 : Run ADC test with VDD/3 as input \r\n");
    printf("3 : Run Comparator test with +ve input pin voltage higher than -ve input pin \r\n");
    printf("4 : Run Comparator test with +ve input pin voltage lower than -ve input pin \r\n");
    printf("5 : DAC DAC test with 0, 511 and 1023 as inputs\r\n");

    for (;;)
    {
        /* Check for commands entered */
        result = mtb_hal_uart_get(&CYBSP_UART_hal_obj, &command, UART_TIMEOUT_MS);

        if (result != MTB_HAL_UART_RSLT_ERR_CSP_UART_GETC_TIMEOUT)
        {
            /* Safety test execution */
            if (SELFTEST_CMD_ADC_2VDD_3 == command)
            {
                printf("\r\nADC test with 2*VDD/3 as input\r\n");
                adc_test_2vdd_3();
            }
            else if (SELFTEST_CMD_ADC_1VDD_3 == command)
            {
                printf("\r\nADC test with VDD/3 as input\r\n");
                adc_test_vdd_3();
            }
            else if (SELFTEST_COMP_OUT_HIGH == command)
            {
                printf("\r\nComparator test with +ve input pin voltage higher than -ve input pin \r\n");
                comparator_test_output_high();
            }
            else if (SELFTEST_COMP_OUT_LOW == command)
            {
                printf("\r\nComparator test with +ve input pin voltage lower than -ve input pin \r\n");
                comparator_test_output_low();
            }
            else if(SELFTEST_CSG_DAC == command)
            {
                printf("\r\nDAC test with 0, 511 and 1023 as inputs \r\n");
                dac_test();
            }

            printf("\r\n\r\n");
        }
    }
}




/* [] END OF FILE */
