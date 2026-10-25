# BeeSense

**IoT-Based Acoustic and Weight Monitoring of Honeybee Colonies**
בי-סנס – ניטור אקוסטי ומשקלי של מושבות דבורי דבש מבוסס האינטרנט של הדברים

Fourth-year engineering project · Ben-Gurion University of the Negev
School of Electrical and Computer Engineering · Project `p-2027-165`

| | |
|---|---|
| **Students** | Dror Ben-Simon (`drorbe@post.bgu.ac.il`), Dan Cooper (`dancoop@post.bgu.ac.il`) |
| **Supervisor** | Dr. Dan Vilenchik |
| **Field site** | Zippory Honey apiary, 3–5 instrumented hives + one empty control platform |
| **Documentation** | [BeeSense-Docs](https://github.com/Dror-bszi/BeeSense-Docs) (PDR, hardware catalog, references) |

---

## What this is

Beekeepers inspect colonies by opening the hive every few weeks to months, and are blind in
between. Swarm preparation takes one to two weeks, but the swarm itself — a large part of the
colony — leaves within minutes and is often discovered only at the next visit.

BeeSense monitors hives at an apiary with **no mains power and no network**. Each hive gets a
battery-powered node built around a piezoelectric vibration sensor that listens at nanoampere
current and wakes the node when vibration rises above the background. The node computes vibration
and sound spectra and fuses them with hive weight and temperature. A solar-powered hub at the
apiary collects the data, detects signs of an approaching swarm and other colony anomalies, and
sends alerts over the cellular network.

The novelty is an autonomous, low-cost system that takes a laboratory-proven capability into the
field at roughly **three orders of magnitude less power** than comparable research systems
(~1 mW average per hive versus ~4 W for a mains-powered Raspberry Pi node).

## Architecture

```
  ┌──────────────── hive node (×N)  ESP32-C3, 1× 18650 ────────────────┐
  │  piezo disc 27 mm (in comb) ──┬── ADC  ─── fixed-point FFT          │
  │                               └── nanopower wake circuit (~2 µA)    │
  │  MEMS microphone (in hive)  ───── I²S                               │
  │  4× 50 kg load cells        ───── HX711 (2-wire)                    │
  │  DS18B20 in / out           ───── 1-Wire                            │
  └──────────────────────────────┬──────────────────────────────────────┘
                                 │  ESP-NOW, 2.4 GHz (no router)
                    ┌────────────┴────────────┐
                    │  hub  ESP32-S3          │  18 V / 15 W solar panel
                    │  baseline + detection   │  + charger + Li-ion pack
                    └────────────┬────────────┘
                                 │  LTE (cellular modem + SIM)
                         cloud ──┴── web dashboard + phone alerts
```

The hives stand close together, so **one hub and one SIM serve a group of hives**.

### Acquisition and detection

A node wakes every **15 minutes**, and additionally whenever the wake circuit detects a vibration
burst (with a 20-minute hold-off after an event). On each wake it records 10 s from the piezo and
the microphone and reads weight and temperature.

Per channel, the power spectral density `S(f)` is summed into `K = 24` bands `Bk` spanning
**50–800 Hz** — the range of the documented pre-swarm changes and of queen piping. Each band is
normalized by the total, which cancels differences in how well the sensor couples to the comb:

```
ek = Σ(f∈Bk) S(f) / Σ(f∈B) S(f),        k = 1 … K
z  = (x − μh) / σh                       ΔW(t) = W(t) − W(t − τ)
```

The hub compares each value against the same hive's mean `μh` and spread `σh` for the same hour of
day over the past week, since colony activity follows a daily cycle. A **pre-swarm alert** is
raised when energy in the swarming band (~100–300 Hz) and the in-hive temperature both stay above
baseline (`|z| > 3`). A **departure** is confirmed when weight drops more than 0.8 kg within
30 minutes together with a vibration burst — a prime swarm weighs about 1–2 kg. The interval
between first alert and departure is the reported *lead time*.

## Targets

| Area | Specification | Target |
|---|---|---|
| Function | Alerts on approaching swarms before departure; colony anomaly alerts | Precision / recall > 90%; lead time reported |
| Inputs | Vibration and sound (50–800 Hz), weight (0–200 kg), temperature in/out | Weight error ≤ ±20 g |
| Outputs | Alerts to the beekeeper's phone; per-hive dashboard | False alarms ≤ 1 per hive-week |
| Rates | Report every 15 min and on vibration events (20-min hold-off) | No missed departure; ≤ 20 false wakes per hive-day |
| Interfaces | Node→hub ESP-NOW (2.4 GHz); hub→cloud LTE; web browser | Packet delivery ≥ 95% |
| Power | Node: one 18650 cell; hub: 18 V / 15 W solar + Li-ion pack | Node ≥ 6 months; ≥ 30 days unattended |
| Environment | Outdoor apiary, no mains power or Wi-Fi; sealed enclosures | Operation through the spring season |

Battery life follows `Ī = I0 + A·Q / 86400` and `Tlife = η·C / Ī`, with `Q ≈ 0.3 A·s` per
acquisition and `I0 ≈ 22 µA` sleep plus wake-circuit current. At 96 periodic acquisitions per day
this gives `Ī ≈ 0.35 mA` and about 285 days on one 3000 mAh cell (`η = 0.8`); the six-month target
holds up to roughly 60 event wakes per day.

## Hardware

| Role | Part |
|---|---|
| Node MCU | ESP32-C3 SuperMini (RISC-V, **no FPU** → fixed-point FFT) |
| Hub MCU | ESP32-S3 (FPU + vector instructions, dual core) |
| Vibration | Piezo ceramic disc 27 mm, pressed into the comb |
| Acoustic | MS3625 MEMS microphone (I²S, INMP441-compatible), behind a mesh |
| Weight | 4× 50 kg half-bridge load cells → HX711 24-bit ADC |
| Temperature | DS18B20 (in-hive and ambient) |
| Node power | 1× 18650 Li-ion cell |
| Hub power | 18 V / 15 W solar panel + charger + Li-ion pack |
| Uplink | Cellular modem + SIM |

Full catalog with order references, prices and per-component notes lives in
[BeeSense-Docs/hardware](https://github.com/Dror-bszi/BeeSense-Docs/tree/main/hardware).

## Repository layout

Firmware and tooling, planned as the stages below are built:

```
firmware/node/      ESP32-C3 hive node: sampling, fixed-point FFT, ESP-NOW, deep sleep
firmware/hub/       ESP32-S3 hub: ESP-NOW receive, baseline, detection, LTE uplink
hardware/           wake-circuit and node schematics, board files
tools/              bench-test scripts, log parsing, signal analysis
dashboard/          per-hive web dashboard
```

## Status

The PDR was submitted on 25 October 2026. No node is built yet — feasibility rests on published
measurements and datasheet values, to be confirmed on the bench. Work stages:

1. **Requirements and bench tests** — each sensor chain and the node's energy use
   (HX711 with reference masses, piezo and wake circuit against known tones, FFT runtime on the
   C3, real sleep current, ESP-NOW range on site, a night-time in-hive recording against the
   circuit noise floor).
2. **Node** — build and test the link at the apiary.
3. **Hub, detection and dashboard** — install on hives before spring.
4. **Field evaluation** — through the spring swarming season, with weekly inspections as ground
   truth.

Open tasks are tracked in
[BeeSense-Docs/TODO.md](https://github.com/Dror-bszi/BeeSense-Docs/blob/main/TODO.md).

## Prior work

| Source | Relevant capability | What BeeSense must show |
|---|---|---|
| Ferrari et al. 2008 [2], Ramsey et al. 2020 [3] | Sound/vibration features precede swarming (>90% accuracy, up to 30 days ahead) | The same features on a battery node, in the field |
| Cecchi et al. 2020 [5] | Microphones, weight to ±10 g, CO₂ — mains powered, ~4 W per hive | Battery operation at ~1 mW average power |
| BeeHero [6] | In-hive acoustics and climate, BLE + cellular gateway (closed system) | Open, measured pipeline; weight + vibration fusion |

[1] Zacepins et al., *Challenges in the development of precision beekeeping*, Biosyst. Eng., 2015 ·
[2] Ferrari et al., *Monitoring of swarming sounds in bee hives*, Comput. Electron. Agric., 2008 ·
[3] Ramsey et al., *The prediction of swarming in honeybee colonies using vibrational spectra*, Sci. Rep., 2020 ·
[4] Meikle & Holst, *Application of continuous monitoring of honeybee colonies*, Apidologie, 2015 ·
[5] Cecchi et al., *A smart sensor-based measurement system for advanced bee hive monitoring*, Sensors, 2020 ·
[6] BeeHero, *Technology*

The full reference list with DOIs and PDFs is in
[BeeSense-Docs/refrence/papers](https://github.com/Dror-bszi/BeeSense-Docs/tree/main/refrence/papers).
