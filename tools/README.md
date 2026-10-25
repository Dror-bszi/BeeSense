# Bench and analysis tools

Host-side scripts, one per Stage 1 test. Everything here except
`battery_life.py` is a placeholder that prints its procedure — the procedure is
the useful part until there is hardware to point it at.

```bash
python3 -m venv .venv && . .venv/bin/activate
pip install -r requirements.txt
```

| Script | Test | Why it matters |
|---|---|---|
| `battery_life.py` | eq. (3) battery model — **implemented** | says whether a measured `I0` still clears six months |
| `bench_sleep_current.py` | real node sleep current | `I0 ≈ 22 µA` is the project's load-bearing assumption |
| `bench_piezo.py` | piezo chain + wake circuit | can the chain see 1–5 µm/s² at all? |
| `bench_hx711.py` | weighing chain | ±20 g is mechanical and thermal, not ADC-limited |
| `bench_fft_timing.py` | Q15 FFT runtime on the C3 | no FPU; also yields `Q` for the battery model |
| `bench_espnow_range.py` | link range and delivery | ≥ 95% delivery, or fall back to LoRa |
| `analyze_night_recording.py` | hive signal vs noise floor | the decisive measurement — everything downstream assumes it passes |

`battery_life.py` runs now and reproduces the PDR figures:

```
$ python3 tools/battery_life.py
  0 event wakes/day  ->  0.355 mA  ->  281 days
  Six months holds up to ~55 event wakes/day.
```

Re-run it with measured values as soon as `bench_sleep_current.py` produces
them — if `I0` comes back at 200 µA instead of 22 µA, the six-month target is
gone and the node board needs rework, which is much cheaper to learn now.
