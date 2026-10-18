#ifndef __USR_UART_H
#define __USR_UART_H

#include <string.h>
#include <stdio.h>
#include "main.h"

extern HAL_StatusTypeDef UART_SenddBuf(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeou);
extern uint8_t dataRcvd;

#endif
