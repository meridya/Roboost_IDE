/**************************************************************************//**
 * @file     pdma.c
 * @version  V1.00
 * $Revision: 2 $
 * $Date: 14/01/28 10:04a $
 * @brief    Nano100 series PDMA driver source file
 *
 * @note
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include "Nano100Series.h"



/** @addtogroup NANO100_Device_Driver NANO100 Device Driver
  @{
*/

/** @addtogroup NANO100_PDMA_Driver PDMA Driver
  @{
*/


/** @addtogroup NANO100_PDMA_EXPORTED_FUNCTIONS PDMA Exported Functions
  @{
*/

/**
 * @brief       PDMA Open
 *
 * @param[in]   u32Mask     Channel enable bits.
 *
 * @return      None
 *
 * @details     This function enable the PDMA channels.
 */
void PDMA_Open(uint32_t u32Mask)
{
    PDMAGCR->CSR |= (u32Mask << 8);
}

/**
 * @brief       PDMA Close
 *
 * @param[in]   None
 *
 * @return      None
 *
 * @details     This function disable all PDMA channels.
 */
void PDMA_Close(void)
{
    PDMAGCR->CSR = 0;
}

/**
 * @brief       Set PDMA Transfer Count
 *
 * @param[in]   u32Ch           The selected channel
 * @param[in]   u32Width        Data width. PDMA_WIDTH_8, PDMA_WIDTH_16, or PDMA_WIDTH_32
 * @param[in]   u32TransCount   Transfer count
 *
 * @return      None
 *
 * @details     This function set the selected channel data width and transfer count.
 */
void PDMA_SetTransferCnt(uint32_t u32Ch, uint32_t u32Width, uint32_t u32TransCount)
{
    PDMA_T *pdma;
    pdma = (PDMA_T *)((uint32_t) PDMA1_BASE + (0x100 * (u32Ch-1)));
    pdma->CSR = (pdma->CSR & ~PDMA_CSR_APB_TWS_Msk) | u32Width;
    switch (u32Width) {
    case PDMA_WIDTH_32:
        pdma->BCR = (u32TransCount << 2);
        break;

    case PDMA_WIDTH_8:
        pdma->BCR = u32TransCount;
        break;

    case PDMA_WIDTH_16:
        pdma->BCR = (u32TransCount << 1);
        break;

    default:
        ;
    }
}

/**
 * @brief       Set PDMA Transfer Address
 *
 * @param[in]   u32Ch           The selected channel
 * @param[in]   u32SrcAddr      Source address
 * @param[in]   u32SrcCtrl      Source control attribute. PDMA_SAR_INC or PDMA_SAR_FIX or PDMA_SAR_WRA
 * @param[in]   u32DstAddr      destination address
 * @param[in]   u32DstCtrl      destination control attribute. PDMA_DAR_INC or PDMA_DAR_FIX or PDMA_DAR_WRA
 *
 * @return      None
 *
 * @details     This function set the selected channel source/destination address and attribute.
 */
void PDMA_SetTransferAddr(uint32_t u32Ch, uint32_t u32SrcAddr, uint32_t u32SrcCtrl, uint32_t u32DstAddr, uint32_t u32DstCtrl)
{
    PDMA_T *pdma;
    pdma = (PDMA_T *)((uint32_t) PDMA1_BASE + (0x100 * (u32Ch-1)));

    pdma->SAR = u32SrcAddr;
    pdma->DAR = u32DstAddr;
    pdma->CSR = (pdma->CSR & ~(PDMA_CSR_SAD_SEL_Msk|PDMA_CSR_DAD_SEL_Msk)) | (u32SrcCtrl | u32DstCtrl);
}

/**
 * @brief       Set PDMA Transfer Mode
 *
 * @param[in]   u32Ch           The selected channel
 * @param[in]   u32Peripheral   The selected peripheral. PDMA_SPI0_TX, PDMA_UART0_TX, PDMA_I2S_TX,...PDMA_MEM
 * @param[in]   u32ScatterEn    Scatter-gather mode enable
 * @param[in]   u32DescAddr     Scatter-gather descriptor address
 *
 * @return      None
 *
 * @details     This function set the selected channel transfer mode. Include peripheral setting.
 */
void PDMA_SetTransferMode(uint32_t u32Ch, uint32_t u32Peripheral, uint32_t u32ScatterEn, uint32_t u32DescAddr)
{
    switch (u32Ch) {
    case 1:
        PDMAGCR->PDSSR0 = (PDMAGCR->PDSSR0 & ~PDMA_PDSSR0_CH1_SEL_Msk) | (u32Peripheral << PDMA_PDSSR0_CH1_SEL_Pos);
        break;
    case 2:
        PDMAGCR->PDSSR0 = (PDMAGCR->PDSSR0 & ~PDMA_PDSSR0_CH2_SEL_Msk) | (u32Peripheral << PDMA_PDSSR0_CH2_SEL_Pos);
        break;
    case 3:
        PDMAGCR->PDSSR0 = (PDMAGCR->PDSSR0 & ~PDMA_PDSSR0_CH3_SEL_Msk) | (u32Peripheral << PDMA_PDSSR0_CH3_SEL_Pos);
        break;
    case 4:
        PDMAGCR->PDSSR1 = (PDMAGCR->PDSSR1 & ~PDMA_PDSSR1_CH4_SEL_Msk) | u32Peripheral;
        break;
    default:
        ;
    }
}

/**
 * @brief       Set PDMA Timeout
 *
 * @param[in]   u32Ch           The selected channel
 * @param[in]   u32OnOff        Enable/disable time out function
 * @param[in]   u32TimeOutCnt   Timeout count
 *
 * @return      None
 *
 * @details     This function set the timeout count.
 */
void PDMA_SetTimeOut(uint32_t u32Ch, uint32_t u32OnOff, uint32_t u32TimeOutCnt)
{
    PDMA_T *pdma;
    pdma = (PDMA_T *)((uint32_t) PDMA1_BASE + (0x100 * (u32Ch-1)));

    pdma->TCR = (pdma->TCR & ~PDMA_TCR_TCR_Msk) | u32TimeOutCnt;
    pdma->CSR = (pdma->CSR & ~PDMA_CSR_TO_EN_Msk) | (u32OnOff << PDMA_CSR_TO_EN_Pos);

}

/**
 * @brief       Trigger PDMA
 *
 * @param[in]   u32Ch           The selected channel
 *
 * @return      None
 *
 * @details     This function trigger the selected channel.
 */
void PDMA_Trigger(uint32_t u32Ch)
{
    PDMA_T *pdma;
    pdma = (PDMA_T *)((uint32_t) PDMA1_BASE + (0x100 * (u32Ch-1)));

    pdma->CSR |= (PDMA_CSR_TRIG_EN_Msk | PDMA_CSR_PDMACEN_Msk);
}

/**
 * @brief       Enable Interrupt
 *
 * @param[in]   u32Ch           The selected channel
 * @param[in]   u32Mask         The Interrupt Type
 *
 * @return      None
 *
 * @details     This function enable the selected channel interrupt.
 */
void PDMA_EnableInt(uint32_t u32Ch, uint32_t u32Mask)
{
    PDMA_T *pdma;
    pdma = (PDMA_T *)((uint32_t) PDMA1_BASE + (0x100 * (u32Ch-1)));

    pdma->IER |= u32Mask;
}

/**
 * @brief       Disable Interrupt
 *
 * @param[in]   u32Ch           The selected channel
 * @param[in]   u32Mask         The Interrupt Type
 *
 * @return      None
 *
 * @details     This function disable the selected channel interrupt.
 */
void PDMA_DisableInt(uint32_t u32Ch, uint32_t u32Mask)
{
    PDMA_T *pdma;
    pdma = (PDMA_T *)((uint32_t) PDMA1_BASE + (0x100 * (u32Ch-1)));

    pdma->IER &= ~u32Mask;
}


/*@}*/ /* end of group NANO100_PDMA_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group NANO100_PDMA_Driver */

/*@}*/ /* end of group NANO100_Device_Driver */

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/
