# Apiary hub firmware (ESP32-S3)

One hub per group of adjacent hives. Always on, solar powered, and the only
thing at the apiary with an internet connection.

## Build

```bash
. $IDF_PATH/export.sh
idf.py set-target esp32s3
idf.py build flash monitor
```

A scaffold, as with the node: the tasks start, log the pipeline they implement,
and idle.

## Structure

| Component | Role |
|---|---|
| `main/main.c` | task setup; the rx → baseline → detect → spool → alerts pipeline |
| `hub_config` | hive roster (MAC → id) and the detection thresholds (NVS) |
| `espnow_rx` | receive reports, ack with hub time, measure delivery ratio |
| `baseline` | per-hive, per-hour-of-day mean and σ over 7 days (eq. 2) |
| `detect` | pre-swarm rule, departure rule, anomalies, node faults |
| `spool` | SD/flash backlog — raw reports and events survive an uplink outage |
| `uplink_lte` | duty-cycled modem, drains the spool, pushes urgent alerts |
| `alerts` | dedupe, rate-limit, rank hives by urgency |

## Why the thinking happens here, not on the nodes

The S3 has an FPU and vector instructions, a week of history for every hive, and
a solar panel instead of a power budget. The C3 nodes have none of those. So the
nodes only ever compute and ship features, and every comparison against a
baseline happens on the hub.

## Open items

- **Modem is a SIMCom A7608E-H** (HW-16), LTE Cat-4 on a BK-A7608E-H breakout
  with LTE and GNSS antennas. Two design consequences: transmit peaks run to
  amps, so the hub board needs bulk capacitance at the module; and its GNSS
  makes the hub the system's time source, so no node needs an RTC.
- **Board model and PSRAM are unconfirmed** (HW-15). `sdkconfig.defaults`
  enables `CONFIG_SPIRAM`; verify the board actually has it.
- **Thresholds get frozen.** `hub_config` holds them because the PDR tunes them
  during the first weeks of the trial and then freezes them before the
  evaluation window. Record when that freeze happens.
- **Channel coupling.** ESP-NOW and station-mode Wi-Fi share one radio and must
  share a channel. Using the cellular modem for the uplink avoids the problem;
  if you ever switch the uplink to Wi-Fi, pin the channel.
