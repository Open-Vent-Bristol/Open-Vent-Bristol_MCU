#include "drivers/scheduler.hpp"
#include "drivers/system.hpp"
#include "board/board.h"

void Scheduler::initialize()
{
  SCHEDULER_START();
}

// SCHEDULER_ISR() {
//   System::getScheduler().triggerCallbacks();
// }