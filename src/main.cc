
#include "board/led.h"
#include "hal/hal_gpio.h"
#include "hal/hal_rcc.h"
#include "hal/hal_uart.h"
#include "system/sys.h"
#include "timer/generic_timer.h"

static uint8_t buf = 0;

static int cnt = 0;
static void SysTimerHandler() {
  LedSwitch();
  cnt++;
}

void UartInit(UART_TypeDef *UARTx, int BaudRate);
void UartSend(uint8_t data);
uint8_t UartReceive(void);

int main(void) {
  RemapVtorTable();
  SystemClk_HSEInit(RCC_PLLMul_20);  // Start PLL clock, 12MHz*20=240MHz
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  LedInit();
  // clock(240MHz) / default kGenericTimerPrescalerMs(24000) = 10KHz
  const uint32_t tm_cnt = 5000;
  GenericTimerInit(tm_cnt, SysTimerHandler);

  UartInit(UART1, 9600);
  int old_cnt = 0;
  while (true) {
    if (old_cnt != cnt) {
      old_cnt = cnt;
      // const uint8_t c = UartReceive();
      // if (c >= 0x30 && c <= 0x39) {
      UartSend(buf);

    }  // else {
       // UartSend(0x2B);
    //}
  }
}

//////////////////////////////////////

// struct __FILE {
//  int handle;
//};
// FILE __stdout;
// int _sys_exit(int x) { x = x; }
// int fputc(int ch, FILE *f) {
//  while ((UART1->CSR & 0x1) == 0) {
//  }
//  UART1->TDR = (u8)ch;
//  return ch;
//}

void UartInit(UART_TypeDef *UARTx, int BaudRate) {
  UART_InitTypeDef UART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;  // uart1_tx  pa9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // Multiplex-free output
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  // uart1_rx  pa10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // pull-up input

  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, GPIO_Pin_9 | GPIO_Pin_10,
                   GPIO_AF_UART_1);  // PA9、PA10复用为串口1

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);
  UART_InitStructure.UART_BaudRate = BaudRate;
  UART_InitStructure.UART_WordLength = UART_WordLength_8b;
  UART_InitStructure.UART_StopBits = UART_StopBits_1;
  UART_InitStructure.UART_Parity = UART_Parity_No;
  UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
  UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
  UART_Init(UARTx, &UART_InitStructure);
  UART_Cmd(UARTx, ENABLE);
  UART_ITConfig(UARTx, UART_IT_RXIEN, ENABLE);
  UART_ClearITPendingBit(UARTx, 0xff);

  NVIC_SetPriority(UART1_IRQn, 3);
  NVIC_EnableIRQ(UART1_IRQn);
}

uint8_t UartReceive(void) {
  while ((UART1->CSR & UART_CSR_RXAVL) == 0) {
    ;
  }
  return UART1->RDR;
}

void UartSend(uint8_t data) {
  while (0 == (UART1->CSR & UART_CSR_TXC)) {
    ;
  }
  UART1->TDR = data;
}

// void send_group(u8 *data, u16 len) {
//  while (len--) send_data(*data++);
//}
// void send_str(char *p) {
//  while (*p != '\0') {
//    send_data(*p);
//    p++;
//  }
//}

#ifdef __cplusplus
extern "C" {
#endif

void UART1_IRQHandler(void) {
  if (UART1->ISR & UART_ISR_RX_INTF) {
    buf = UART1->RDR;  // = UartReceive();
    UART1->ICR |= UART_ICR_RXICLR;
    // printf("UART1_RDR = %d\n", UART1->RDR);
  }
}

#ifdef __cplusplus
} /*extern "C"*/
#endif
