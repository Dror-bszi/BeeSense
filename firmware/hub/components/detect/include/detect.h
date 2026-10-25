// The two rules the project is graded on -- PDR 3.1.
//
// PRE-SWARM ALERT (the predictive one, precision/recall > 90% target):
//   energy in the swarming band (~100-300 Hz) AND in-hive temperature both
//   stay above the hive's own baseline, |z| > 3. "Stay" matters: a single
//   report over threshold is noise, and the false-alarm budget is <= 1 per
//   hive-week, so the rule needs persistence across consecutive reports.
//
// DEPARTURE CONFIRMED (the retrospective one, ground truth for lead time):
//   weight drops > 0.8 kg within 30 min together with a vibration burst.
//   A prime swarm weighs 1-2 kg, so this is unambiguous -- its job is to
//   timestamp the departure so lead time = t_departure - t_first_alert.
//
// Lead time is the research question, not a target: how early the pre-swarm
// alert can fire is what the field trial is meant to find out.
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"
#include "baseline.h"
#include "beesense_proto.h"
#include "hub_config.h"

typedef enum {
    DETECT_NONE = 0,
    DETECT_PRE_SWARM,        // swarm preparation suspected -- visit this hive
    DETECT_DEPARTURE,        // a swarm just left
    DETECT_COLONY_ANOMALY,   // not swarm-shaped, but off its own baseline
    DETECT_NODE_FAULT,       // sensor fault / low battery / node gone silent
} detect_event_t;

typedef struct {
    detect_event_t event;
    uint8_t  hive_id;
    uint32_t epoch_s;
    float    score;             // the z that triggered it
    uint32_t lead_time_s;       // on DEPARTURE: time since the first pre-swarm alert
    char     reason[96];        // human-readable, goes straight into the alert
} detect_result_t;

esp_err_t detect_init(void);

esp_err_t detect_evaluate(const hub_config_t *cfg, const bs_report_t *report,
                          const baseline_z_t *z, detect_result_t *out);

// A node that has missed several reporting slots is itself an alert: the
// 30-days-unattended target means nobody is at the apiary to notice.
esp_err_t detect_check_silent_nodes(const hub_config_t *cfg, detect_result_t *out);
