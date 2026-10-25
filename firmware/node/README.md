# Hive node firmware (ESP32-C3)

One node per hive, on one 18650 cell, targeting six months per charge.

## Build

```bash
. $IDF_PATH/export.sh
idf.py set-target esp32c3
idf.py build flash monitor
```

Currently a scaffold: every module logs what it will do and returns success, so
`monitor` prints the whole intended duty cycle without any hardware attached.
That output is the spec — if the flow in the log is wrong, fix it here before
writing drivers.

## Structure

| Component | Role |
|---|---|
| `main/main.c` | the duty cycle: wake → acquire → dsp → send → sleep |
| `node_config` | hive id, pin map, tare/calibration, thresholds (NVS) |
| `wake_source` | timer vs piezo-event wake, and the 20 min hold-off |
| `piezo_adc` | piezo record; also gates the amp's power |
| `mic_i2s` | MEMS microphone record, decimated to the piezo's rate |
| `dsp_fft` | Q15 FFT → PSD → 24 bands over 50–800 Hz → `e_k` (eq. 1) |
| `scale_hx711` | 4 load cells → HX711, tare and calibration |
| `temp_probe` | DS18B20 in-hive + ambient |
| `espnow_link` | report to hub, ack, RTC-memory backlog |
| `power_mgr` | battery read, arms both wake paths, deep sleep |

The packet format is in `firmware/shared/beesense_proto`, compiled by both
projects so node and hub cannot drift apart.

## Constraints worth remembering

- **No FPU.** The C3 emulates float in software. Keep the signal path fixed
  point; `dsp_fft` is Q15 on purpose.
- **Deep sleep is the normal state.** Anything left enabled before
  `power_mgr_sleep()` is charged against `I0` for 15 minutes. The radio, the
  piezo amp, the HX711 and I²S all have explicit teardown calls for this reason.
- **The pin map is TBD.** Every `NODE_PIN_*` in `node_config.h` is a placeholder;
  the piezo wake pin must be RTC/deep-sleep-wake capable.
- **Detection does not live here.** The node ships features; the hub decides.
