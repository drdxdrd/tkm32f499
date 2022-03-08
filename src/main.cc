
#include "board/led.h"
#include "hal/hal_gpio.h"
#include "hal/hal_rcc.h"
#include "system/sys.h"
#include "timer/generic_timer.h"

static void SysTimerHandler() { LedSwitch(); }

int main(void) {
  RemapVtorTable();
  SystemClk_HSEInit(RCC_PLLMul_20);  // Start PLL clock, 12MHz*20=240MHz
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  LedInit();
  // clock(240MHz) / default kGenericTimerPrescalerMs(24000) = 10KHz
  const uint32_t tm_cnt = 5000;
  GenericTimerInit(tm_cnt, SysTimerHandler);

  while (true) {
    ;
  }
}
