
#include "board/led.h"
#include "hal/hal_gpio.h"
#include "hal/hal_rcc.h"
#include "system/sys.h"
#include "timer/system_timer.h"

static uint16_t cnt = 0;
static void SysTimerHandler() {
  if (250 == cnt) {
    LedSwitch();
    cnt = 0;
  } else {
    cnt++;
  }
}

int main(void) {
  RemapVtorTable();
  SystemClk_HSEInit(RCC_PLLMul_20);  // Start PLL clock, 12MHz*20=240MHz
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  LedInit();
  SystemTimerInit(SysTimerHandler);
  while (true) {
    ;
  }
}
