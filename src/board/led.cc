#include "led.h"

void LedInit() {
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  // Configure the GPIO connected to the LED as push-pull output mode
  GPIO_InitTypeDef gpio;
  gpio.GPIO_Pin = GPIO_Pin_8;
  gpio.GPIO_Speed = GPIO_Speed_50MHz;
  gpio.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &gpio);
}

void LedSet(bool en) {
  if (en) {
    GPIO_SetBits(GPIOA, GPIO_Pin_8);
  } else {
    GPIO_ResetBits(GPIOA, GPIO_Pin_8);
  }
}

void LedSwitch() {
  static bool state_{false};
  LedSet(state_);
  state_ = !state_;
}
