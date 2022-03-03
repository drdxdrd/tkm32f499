#pragma once

#include "hal/hal_rcc.h"
#include "system/cmsis/core_cm4.h"

using IrqHandler = void (*)();

void SystemTimerInit(IrqHandler handler);
