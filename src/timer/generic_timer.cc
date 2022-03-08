#include "generic_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

void DefaultHandler(void) {}
static IrqHandler Timer3Handler = &DefaultHandler;

void TIM3_IRQHandler(void) {
  // Check whether the TIM3 update interrupt occurs or not
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
    Timer3Handler();
  }
  // Clear the TIMx update interrupt flag
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

#ifdef __cplusplus
} /*extern "C"*/
#endif

void GenericTimerInit(uint32_t arr, IrqHandler handler, uint16_t psc) {
  TIM_TimeBaseInitTypeDef timer;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  timer.TIM_Period = arr;
  timer.TIM_Prescaler = psc;
  timer.TIM_ClockDivision = TIM_CKD_DIV1;
  timer.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &timer);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

  NVIC_InitTypeDef nvic;
  nvic.NVIC_IRQChannel = TIM3_IRQn;
  nvic.NVIC_IRQChannelPreemptionPriority = 1;
  nvic.NVIC_IRQChannelSubPriority = 2;
  nvic.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvic);

  Timer3Handler = handler;
  TIM_Cmd(TIM3, ENABLE);
}
