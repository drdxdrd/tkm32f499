#include "hal/hal_gpio.h"
#include "hal/hal_rcc.h"

int main(void) {
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  // Configure the GPIO connected to the LED as push-pull output mode
  GPIO_InitTypeDef gpio;
  gpio.GPIO_Pin = GPIO_Pin_8;
  gpio.GPIO_Speed = GPIO_Speed_50MHz;
  gpio.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &gpio);

  while (true) {
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
    for (uint32_t i = 0; i < 2000000; i++) {
      ;
    }

    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
    for (uint32_t i = 0; i < 2000000; i++) {
      ;
    }
  }
}
