#pragma once

#include <cstdint>

#include "board/board.h"
#include "hal/hal_misc.h"
#include "hal/hal_rcc.h"
#include "hal/hal_tim.h"

static const uint16_t kGenericTimerPrescalerMs = 24000;

///
/// \brief GenericTimerInit
/// \param arr - auto-load increment counter.
/// \param handler
/// \param psc - (prescaler) frequency division factor of the counter clock
///
void GenericTimerInit(uint32_t arr, IrqHandler handler,
                      uint16_t psc = kGenericTimerPrescalerMs);
