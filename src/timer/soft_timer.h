#pragma once

#include <cstdint>

namespace timer {

class SoftTimer {
 public:
  using ValueType = uint32_t;
  explicit SoftTimer(const ValueType &timeout = 0);
  void SetTimeout(const ValueType &timeout);
  void Start(const ValueType &timeout);
  void Start();
  void Reset();
  bool IsTimeout();
  bool IsActivate();
  ValueType GetTimeoutValue() const;

 private:
  ValueType GetCount() const;
  bool UpdateFlags();
  bool flag_start_{false};
  bool flag_stop_{false};
  // bool is_run_{false};
  ValueType val_start_{0};
  ValueType val_timeout_{0};
  virtual ValueType GetSystemClock() const = 0;
};

}  // namespace timer
