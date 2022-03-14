#pragma once

#include <cstdint>

namespace timer {

class SoftTimer {
 public:
  using ValueType = uint32_t;
  explicit SoftTimer(const ValueType &timeout = 0);
  ///
  /// \brief SetTimeout function set timer timeout if it is not activated
  /// \param timeout
  /// \returns true if timeout is set otherwise returns false
  ///
  bool SetTimeout(const ValueType &timeout);
  void Start(const ValueType &timeout);
  void Start();
  ///
  /// \brief Stop function stop and reset timer
  ///
  void Stop();
  bool IsTimeout();
  ///
  /// \brief IsActivate
  /// \return true if timer is activate. Timer is activate all time from call
  /// Start to call IsTimeout or Stop.
  ///
  bool IsActivate() const;
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
