

#include <gtest/gtest.h>

#include "timer/soft_timer.h"

class Timer : public timer::SoftTimer {
 public:
  Timer(const ValueType &timeout = 0) : SoftTimer(timeout){};
  ValueType clk_{0};

 private:
  virtual ValueType GetSystemClock() const override { return clk_; }
};

TEST(SoftTimer, Init) {
  Timer timer1;
  EXPECT_EQ(static_cast<Timer::ValueType>(0), timer1.GetTimeoutValue());
  const Timer::ValueType t = 42;
  Timer timer2(t);
  EXPECT_EQ(t, timer2.GetTimeoutValue());
}

TEST(SoftTimer, Reset) {
  const Timer::ValueType t = 42;
  Timer timer(t);
  EXPECT_EQ(t, timer.GetTimeoutValue());
  EXPECT_FALSE(timer.IsActivate());
  EXPECT_FALSE(timer.IsTimeout());
  timer.Reset();
  EXPECT_EQ(t, timer.GetTimeoutValue());
  EXPECT_FALSE(timer.IsActivate());
  EXPECT_FALSE(timer.IsTimeout());
  timer.Reset();
  EXPECT_EQ(t, timer.GetTimeoutValue());
  EXPECT_FALSE(timer.IsActivate());
  EXPECT_FALSE(timer.IsTimeout());
  timer.Reset();
  EXPECT_EQ(t, timer.GetTimeoutValue());
  EXPECT_FALSE(timer.IsActivate());
  EXPECT_FALSE(timer.IsTimeout());
}

TEST(SoftTimer, SetTimeout) {
  const Timer::ValueType t = 42;
  Timer timer(t);
  EXPECT_EQ(t, timer.GetTimeoutValue());
  timer.SetTimeout(55);
  EXPECT_EQ(static_cast<Timer::ValueType>(55), timer.GetTimeoutValue());
  timer.Reset();
  EXPECT_EQ(static_cast<Timer::ValueType>(55), timer.GetTimeoutValue());
}

TEST(SoftTimer, StartReset) {
  const Timer::ValueType t = 42;
  Timer timer(t);
  timer.clk_ = 10;
  timer.Start();
  EXPECT_TRUE(timer.IsActivate());
  EXPECT_FALSE(timer.IsTimeout());
  timer.clk_++;
  EXPECT_TRUE(timer.IsActivate());
  EXPECT_FALSE(timer.IsTimeout());
  timer.Reset();
  EXPECT_EQ(t, timer.GetTimeoutValue());
  EXPECT_FALSE(timer.IsActivate());
  EXPECT_FALSE(timer.IsTimeout());
}

TEST(SoftTimer, Timeout) {
  const Timer::ValueType t = 42;
  Timer timer(t);
  timer.clk_ = 10;
  timer.Start();
  EXPECT_FALSE(timer.IsTimeout());
  timer.clk_ += (t - 1);
  EXPECT_FALSE(timer.IsTimeout());

  // Timeout
  timer.clk_++;
  EXPECT_TRUE(timer.IsTimeout());

  timer.clk_++;
  EXPECT_TRUE(timer.IsTimeout());

  timer.clk_ += std::numeric_limits<Timer::ValueType>::max();
  EXPECT_TRUE(timer.IsTimeout());

  timer.clk_++;
  EXPECT_TRUE(timer.IsTimeout());
  EXPECT_FALSE(timer.IsActivate());

  timer.Reset();
  EXPECT_EQ(t, timer.GetTimeoutValue());
  EXPECT_FALSE(timer.IsActivate());
  EXPECT_FALSE(timer.IsTimeout());
}

TEST(SoftTimer, Activate) {
  const Timer::ValueType t = 42;
  Timer timer(t);
  timer.clk_ = 10;
  timer.Start();
  EXPECT_TRUE(timer.IsActivate());

  timer.clk_ += (t - 1);
  EXPECT_TRUE(timer.IsActivate());

  // Timeout
  timer.clk_++;
  EXPECT_FALSE(timer.IsActivate());

  timer.clk_++;
  EXPECT_FALSE(timer.IsActivate());

  timer.clk_ += std::numeric_limits<Timer::ValueType>::max();
  EXPECT_FALSE(timer.IsActivate());

  timer.clk_++;
  EXPECT_FALSE(timer.IsActivate());
  EXPECT_TRUE(timer.IsTimeout());

  timer.Reset();
  EXPECT_EQ(t, timer.GetTimeoutValue());
  EXPECT_FALSE(timer.IsActivate());
  EXPECT_FALSE(timer.IsTimeout());
}

TEST(SoftTimer, PreOverflow) {
  const Timer::ValueType t = 42;
  Timer timer(t);
  timer.clk_ = 10;
  timer.Start();
  EXPECT_TRUE(timer.IsActivate());
  EXPECT_FALSE(timer.IsTimeout());

  // max timer timeout
  timer.clk_ += std::numeric_limits<Timer::ValueType>::max();

  EXPECT_FALSE(timer.IsActivate());
  EXPECT_TRUE(timer.IsTimeout());

  timer.Reset();
  EXPECT_EQ(t, timer.GetTimeoutValue());
  EXPECT_FALSE(timer.IsActivate());
  EXPECT_FALSE(timer.IsTimeout());
}

TEST(SoftTimer, Overflow) {
  const Timer::ValueType t = 42;
  Timer timer(t);
  timer.clk_ = 10;
  timer.Start();
  EXPECT_TRUE(timer.IsActivate());
  EXPECT_FALSE(timer.IsTimeout());

  // max timer timeout + 1
  timer.clk_++;
  timer.clk_ += std::numeric_limits<Timer::ValueType>::max();
  EXPECT_TRUE(timer.IsActivate());
  EXPECT_FALSE(timer.IsTimeout());

  timer.clk_ += t;
  EXPECT_FALSE(timer.IsActivate());
  EXPECT_TRUE(timer.IsTimeout());

  timer.Reset();
  EXPECT_EQ(t, timer.GetTimeoutValue());
  EXPECT_FALSE(timer.IsActivate());
  EXPECT_FALSE(timer.IsTimeout());
}
