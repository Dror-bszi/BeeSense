// Why did this node boot, and should it actually acquire?
//
// The node has two wake paths (PDR 1.3, Table I):
//   - the 15 min periodic timer, and
//   - the ~2 uA analog wake circuit on the piezo, which fires when vibration
//     jumps above its usual level.
// After an event wake the node ignores new triggers for 20 min, which is what
// keeps the <= 20 false wakes per hive-day budget (and the 6-month battery
// target) from being blown by a noisy afternoon.
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "beesense_proto.h"

bs_wake_reason_t wake_source_get(void);

// True when an event wake arrives inside the hold-off window and should be
// dropped without acquiring. Hold-off state lives in RTC memory, which
// survives deep sleep.
bool wake_source_in_holdoff(uint16_t holdoff_s);

void wake_source_mark_event(void);

// Event wakes per day, kept in RTC memory -- this is the number the battery
// model in tools/battery_life.py is sensitive to, so the node reports it.
uint16_t wake_source_event_count_today(void);
