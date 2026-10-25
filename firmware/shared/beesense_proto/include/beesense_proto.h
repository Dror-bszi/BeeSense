// BeeSense node -> hub report packet. Shared by both firmware projects so the
// wire format has exactly one definition.
//
// Sizing: one report must fit in a single ESP-NOW payload (250 B max), because
// the PDR budgets one short message per report for the >= 95% delivery target.

#pragma once

#include <stdint.h>

#define BS_PROTO_VERSION 1

// Spectral bands: K = 24 bands spanning 50-800 Hz (PDR 3.1). Covers the
// documented pre-swarm changes (~100-300 Hz) and queen piping.
#define BS_FFT_BANDS     24
#define BS_BAND_F_MIN_HZ 50
#define BS_BAND_F_MAX_HZ 800

// Acquisition shape (PDR Table I / 3.1).
#define BS_RECORD_SECONDS      10
#define BS_PERIOD_SECONDS      (15 * 60)
#define BS_EVENT_HOLDOFF_SEC   (20 * 60)

typedef enum {
    BS_WAKE_BOOT = 0,      // cold boot / reset
    BS_WAKE_TIMER,         // periodic 15 min cycle
    BS_WAKE_PIEZO_EVENT,   // nanopower wake circuit fired on a vibration burst
} bs_wake_reason_t;

typedef enum {
    BS_FLAG_SCALE_FAULT  = 1 << 0,
    BS_FLAG_MIC_FAULT    = 1 << 1,
    BS_FLAG_TEMP_FAULT   = 1 << 2,
    BS_FLAG_LOW_BATTERY  = 1 << 3,
    BS_FLAG_SPOOL_REPLAY = 1 << 4,  // report resent from the node's backlog
} bs_flags_t;

// Band energies are the normalized e_k of PDR eq. (1):
//   e_k = sum(f in B_k) S(f) / sum(f in B) S(f)
// so they sum to 1.0 and the sensor's coupling to the comb cancels out.
// Stored as unsigned Q0.16 fixed point (65535 == 1.0) -- the C3 has no FPU.
typedef struct __attribute__((packed)) {
    uint8_t  version;                    // BS_PROTO_VERSION
    uint8_t  hive_id;                    // which hive this node sits on
    uint8_t  wake_reason;                // bs_wake_reason_t
    uint8_t  seq;                        // wraps; lets the hub count loss
    uint32_t epoch_s;                    // node clock, resynced from hub acks
    uint16_t piezo_bands[BS_FFT_BANDS];  // Q0.16, in-comb vibration channel
    uint16_t mic_bands[BS_FFT_BANDS];    // Q0.16, in-hive acoustic channel
    int32_t  weight_g;                   // 4x 50 kg cells via HX711, 0-200 kg
    int16_t  temp_in_c100;               // in-hive DS18B20, 0.01 C
    int16_t  temp_out_c100;              // ambient DS18B20, 0.01 C
    uint16_t vbat_mv;                    // 18650 terminal voltage
    uint16_t flags;                      // bs_flags_t bitmask
} bs_report_t;

// Hub -> node acknowledgement. Carries the clock so nodes stay aligned without
// an RTC, and can retune a node in the field without reflashing it.
typedef struct __attribute__((packed)) {
    uint8_t  version;
    uint8_t  ack_seq;             // the seq being acknowledged
    uint16_t next_period_s;       // 0 = keep current period
    uint32_t epoch_s;             // hub time, for node clock correction
} bs_ack_t;

const char *bs_wake_reason_str(bs_wake_reason_t reason);

// Sanity-check a received packet (version, hive_id range, bands sum to ~1.0).
// Returns 0 when the report is usable.
int bs_report_validate(const bs_report_t *report);
