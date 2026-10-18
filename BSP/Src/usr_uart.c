#include "usr_uart.h"

/* USER CODE BEGIN PV */
#define RXBUFFERSIZE 256

uint8_t dataRcvd;

extern UART_HandleTypeDef huart1;

HAL_StatusTypeDef UART_SenddBuf(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeou)
{
  return (HAL_UART_Transmit(huart, pData, Size, Size));
}

int fputc(int ch, FILE *f)
{
  UART_SenddBuf(&huart1, (uint8_t *)&ch, 1, 5);
}
