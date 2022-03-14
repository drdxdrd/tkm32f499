#include "timer.h"

namespace timer {

static Timer::ValueType system_timer_cnt{0};

#ifdef __cplusplus
extern "C" {
#endif

void SysTick_Handler(void) { system_timer_cnt++; }

#ifdef __cplusplus
} /*extern "C"*/
#endif

bool Timer::is_init_{false};

Timer::Timer(const ValueType &timeout) {
  // call once
  Init();
}

SoftTimer::ValueType Timer::GetSystemClock() const { return system_timer_cnt; }

void Timer::Init() {
  if (!is_init_) {
    is_init_ = true;
    system_timer_cnt = 0;
    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency / prescaller_);
  }
}

}  // namespace timer
