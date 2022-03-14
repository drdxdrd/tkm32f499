#pragma once

#include "hal/hal_rcc.h"
#include "system/cmsis/core_cm4.h"
#include "timer/soft_timer.h"

namespace timer {

class Timer : public SoftTimer {
 public:
  Timer(const ValueType &timeout = 0);

 private:
  static bool is_init_;
  static const uint32_t prescaller_{1000};
  virtual ValueType GetSystemClock() const override;
  void Init();
};

}  // namespace timer
