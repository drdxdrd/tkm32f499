#include "system_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

void DefaultSysTimerHandler(void) {}
static IrqHandler SysTimerHandler = &DefaultSysTimerHandler;

void SysTick_Handler(void) { SysTimerHandler(); }

#ifdef __cplusplus
} /*extern "C"*/
#endif

void SystemTimerInit(IrqHandler handler) {
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
  SysTimerHandler = handler;
}
