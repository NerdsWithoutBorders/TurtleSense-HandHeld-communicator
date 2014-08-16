/*
 *  This file is automatically generated and does not require a license
 *
 *  ==== WARNING: CHANGES TO THIS GENERATED FILE WILL BE OVERWRITTEN ====
 *
 *  To make changes to the generated code, use the space between existing
 *      "USER CODE START (section: <name>)"
 *  and
 *      "USER CODE END (section: <name>)"
 *  comments, where <name> is a single word identifying the section.
 *  Only these sections will be preserved.
 *
 *  Do not move these sections within this file or change the START and
 *  END comments in any way.
 *  ==== ALL OTHER CHANGES WILL BE OVERWRITTEN WHEN IT IS REGENERATED ====
 *
 *  This file was generated from
 *      C:/ti/grace_2_20_02_32/packages/ti/mcu/msp430/csl2/communication/EUSCI_init.xdt
 */
#include <stdint.h>
#include <driverlib/MSP430FR57xx/inc/hw_memmap.h>
#include <driverlib/MSP430FR57xx/eusci_a_uart.h>

/* USER CODE START (section: EUSCI_A0_init_c_prologue) */
/* User defined includes, defines, global variables and functions */
/* USER CODE END (section: EUSCI_A0_init_c_prologue) */

/*
 *  ======== EUSCI_A0_graceInit ========
 *  Initialize Config for the MSP430 eUSCI_A0
 */
void EUSCI_A0_graceInit(void)
{

    /* USER CODE START (section: EUSCI_A0_graceInit_prologue) */
    /* User initialization code */
    /* USER CODE END (section: EUSCI_A0_graceInit_prologue) */

    /* initialize UART for 115200 baud (based on a 8000000 Hz clock) */
    if (STATUS_FAIL == EUSCI_A_UART_initAdvance(EUSCI_A0_BASE, EUSCI_A_UART_CLOCKSOURCE_SMCLK, 4, 5, 85,
                        EUSCI_A_UART_NO_PARITY, EUSCI_A_UART_LSB_FIRST, EUSCI_A_UART_ONE_STOP_BIT, EUSCI_A_UART_MODE, 1)) {
        return;
    }

    /* enable eUSCI UART */
    EUSCI_A_UART_enable(EUSCI_A0_BASE);

    /* set deglitch time */
    EUSCI_A_UART_selectDeglitchTime(EUSCI_A0_BASE, EUSCI_A_UART_DEGLITCH_TIME_200ns);

    /* disable eUSCI UART transmit interrupt */
    EUSCI_A_UART_disableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT);

    /* disable eUSCI UART receive interrupt */
    EUSCI_A_UART_disableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);

    /* USER CODE START (section: EUSCI_A0_graceInit_epilogue) */
    /* User code */
    /* USER CODE END (section: EUSCI_A0_graceInit_epilogue) */

}
