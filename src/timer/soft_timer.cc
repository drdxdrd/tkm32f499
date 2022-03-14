#include "soft_timer.h"

namespace timer {

SoftTimer::SoftTimer(const ValueType &timeout) : val_timeout_(timeout) {}

bool SoftTimer::SetTimeout(const ValueType &timeout) {
  const bool is_not_act = !IsActivate();
  if (is_not_act) {
    val_timeout_ = timeout;
  }
  return is_not_act;
}

void SoftTimer::Start(const ValueType &timeout) {
  val_timeout_ = timeout;
  Start();
}

void SoftTimer::Start() {
  flag_start_ = true;
  flag_stop_ = false;
  val_start_ = GetSystemClock();
}

void SoftTimer::Reset() {
  flag_start_ = false;
  flag_stop_ = false;
}

SoftTimer::ValueType SoftTimer::GetCount() const {
  const ValueType val_cur = GetSystemClock();
  // mask not need because timer tick up to max type value
  return (val_cur - val_start_);
}

bool SoftTimer::UpdateFlags() {
  const ValueType val_cur = GetCount();
  if (val_timeout_ > val_cur) {
    return false;
  }
  flag_start_ = false;
  flag_stop_ = true;
  return true;
}

bool SoftTimer::IsTimeout() {
  if (flag_stop_) {
    return true;
  }
  if (!flag_start_) {
    return false;
  }
  return UpdateFlags();
}

bool SoftTimer::IsActivate() const { return flag_start_; }

SoftTimer::ValueType SoftTimer::GetTimeoutValue() const { return val_timeout_; }

}  // namespace timer
