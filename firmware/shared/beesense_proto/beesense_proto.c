#include "beesense_proto.h"

#include <stdio.h>

const char *bs_wake_reason_str(bs_wake_reason_t reason)
{
    switch (reason) {
    case BS_WAKE_BOOT:        return "boot";
    case BS_WAKE_TIMER:       return "timer";
    case BS_WAKE_PIEZO_EVENT: return "piezo-event";
    default:                  return "unknown";
    }
}

int bs_report_validate(const bs_report_t *report)
{
    // TODO: reject on version mismatch, unknown hive_id, and band vectors whose
    // Q0.16 sum strays far from 65535 (a sign of a truncated or corrupt packet).
    printf("[proto] validate: check version/hive_id, and that band energies sum to ~1.0\n");
    (void)report;
    return 0;
}
