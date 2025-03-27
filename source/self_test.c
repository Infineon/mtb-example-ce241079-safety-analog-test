/******************************************************************************
* File Name:   self_test.c
*
* Description:This file provides the helper functions for executing self tests
*             for  PSOC Control C3 MCU.
*
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

#include "cy_pdl.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "self_test.h"


/*******************************************************************************
* Global Variables
*******************************************************************************/
static cy_stc_lpcomp_context_t context;

/*******************************************************************************
* Function Name: adc_test_2vdd_3
********************************************************************************
* Summary:
* This function performs self test on the ADC block by  measuring the voltage on
* a specific channel and comparing it against the expected result within a defined
* accuracy range, this test validates the accuracy and functionality of the ADC in
* converting analog signals to digital values. The function expects VDD/3 as the
* input voltage.
*
* Parameters:
*  none
*
* Return :
*  void
*
*******************************************************************************/
void adc_test_2vdd_3(void)
{
    /* ADC Testing after connecting 2*VDDA/3 voltage to ADC channel 4. */
    if (OK_STATUS != SelfTests_ADC(0, 4, ANALOG_ADC_SAR_RESULT2, ANALOG_ADC_ACURACCY, 0, 0))
    {
        /* Process error */
        printf("Error: ADC SelfTest failed for 2VDD/3 signal.\r\n");
    }
    else
    {
        printf("SUCCESS: ADC SelfTest passed for 2VDD/3 signal.\r\n");
    }
}

/*******************************************************************************
* Function Name: adc_test_vdd_3
********************************************************************************
* Summary:
* This function performs self test on the ADC block by  measuring the voltage on
* a specific channel and comparing it against the expected result within a defined
* accuracy range, this test validates the accuracy and functionality of the ADC in
* converting analog signals to digital values. The function expects 2*VDD/3 as the
* input voltage.
*
* Parameters:
*  none
*
* Return :
*  void
*
*******************************************************************************/
void adc_test_vdd_3(void)
{
    /* ADC Testing after connecting VDDA/3 voltage to ADC channel 4. */
    if (OK_STATUS != SelfTests_ADC( 0, 4, ANALOG_ADC_SAR_RESULT1, ANALOG_ADC_ACURACCY, 0, 0))
    {
        /* Process error */
        printf("Error: ADC SelfTest failed for VDD/3 signal.\r\n");
    }
    else
    {
        printf("SUCCESS: ADC SelfTest passed for VDD/3 signal.\r\n");
    }
}

/*******************************************************************************
* Function Name: comparator_test_output_high
********************************************************************************
* Summary:
* This function is to performs the test on the comparator block by verifying if
* the comparator output aligns with the expected result when a higher voltage
* is provided to the positive terminal of the comparator when compared to the
* negative terminal.
*
* Parameters:
*  none
*
* Return :
*  void
*
*******************************************************************************/
void comparator_test_output_high(void)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    /*Initialize the LPCOMP with device configurator generated structure*/
    result = Cy_LPComp_Init_Ext(CYBSP_LPCOMP_HW, CYBSP_LPCOMP_CHANNEL , &CYBSP_LPCOMP_config, &context);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
    Cy_LPComp_Enable_Ext(CYBSP_LPCOMP_HW, CYBSP_LPCOMP_CHANNEL, &context);

    /* Comparator Testing after applying higher voltage to positive input */
    if(OK_STATUS != SelfTests_Comparator(CYBSP_LPCOMP_HW, CYBSP_LPCOMP_CHANNEL, ANALOG_COMP_RESULT1))
    {
        /* Process error */
        printf("Error: LPCOMP higher voltage test fail\r\n");
    }
    else
    {
        printf("SUCCESS: LPCOMP higher voltage test\r\n");
    }
}

/*******************************************************************************
* Function Name: comparator_test_output_low
********************************************************************************
* Summary:
* This function is to performs the test on the comparator block by verifying if
* the comparator output aligns with the expected result when a lower voltage
* is provided to the positive terminal of the comparator when compared to the
* negative terminal.
*
* Parameters:
*  none
*
* Return :
*  void
*
*******************************************************************************/
void comparator_test_output_low(void)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    /*Initialize the LPCOMP with device configurator generated structure*/
    result = Cy_LPComp_Init_Ext(CYBSP_LPCOMP_HW, CYBSP_LPCOMP_CHANNEL , &CYBSP_LPCOMP_config, &context);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
    Cy_LPComp_Enable_Ext(CYBSP_LPCOMP_HW, CYBSP_LPCOMP_CHANNEL, &context);

    /* Comparator Testing after applying lower voltage to positive input */
    if(OK_STATUS != SelfTests_Comparator(CYBSP_LPCOMP_HW, CYBSP_LPCOMP_CHANNEL, ANALOG_COMP_RESULT2))
    {
        /* Process error */
        printf("Error: LPCOMP lower voltage test fail\r\n");
    }
    else
    {
        printf("SUCCESS: LPCOMP lower voltage test\r\n");
    }
}

/*******************************************************************************
* Function Name: dac_test
********************************************************************************
* Summary:
* This function is to performs the test on the DAC connected to the CSG by
* reading the DAC output with an ADC and comparing it with the expected result.
* The test will be run 3 times with 3 different inputs and verified.
*
* Parameters:
*  none
*
* Return :
*  void
*
*******************************************************************************/
void dac_test(void)
{
    /* DAC testing by setting lowest value as input. */
    if (OK_STATUS != SelfTests_DAC(CSG_VDAC_OUT_CHAN_IDX, CYBSP_DAC_SLICE_IDX, DAC_TEST1_DAC_IN, DAC_TEST1_ADC_VAL, DAC_TEST_ADC_ACCURACY))
    {
        /* Process error */
        printf("Error: DAC Test with input value 0.\r\n");
    }
    else
    {
        printf("SUCCESS: DAC Test with input value 0.\r\n");
    }

    /* DAC testing by setting middle value as input. */
    if (OK_STATUS != SelfTests_DAC(CSG_VDAC_OUT_CHAN_IDX, CYBSP_DAC_SLICE_IDX, DAC_TEST2_DAC_IN, DAC_TEST2_ADC_VAL, DAC_TEST_ADC_ACCURACY))
    {
        /* Process error */
        printf("Error: DAC Test with input value 511.\r\n");
    }
    else
    {
        printf("SUCCESS: DAC Test with input value 511.\r\n");
    }

    /* DAC testing by setting middle value as input. */
    if (OK_STATUS != SelfTests_DAC(CSG_VDAC_OUT_CHAN_IDX, CYBSP_DAC_SLICE_IDX, DAC_TEST3_DAC_IN, DAC_TEST3_ADC_VAL, DAC_TEST_ADC_ACCURACY))
    {
        /* Process error */
        printf("Error: DAC Test with input value 1023.\r\n");
    }
    else
    {
        printf("SUCCESS: DAC Test with input value 1023.\r\n");
    }
}

/* [] END OF FILE */

