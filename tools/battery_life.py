#!/usr/bin/env python3
"""Node battery-life model -- PDR eq. (3).

    I_avg  = I0 + A * Q / 86400
    T_life = eta * C / I_avg

where Q is the charge of one acquisition (mostly the CPU during the 10 s
record), A the acquisitions per day, and I0 the sleep plus wake-circuit
current.

This one is implemented rather than stubbed: it is pure arithmetic, it
reproduces the figures quoted in the PDR, and it is what says whether a
measured sleep current still clears the six-month target. Every default below
is a datasheet-level estimate and should be replaced with a bench measurement
(see bench_sleep_current.py).
"""

from __future__ import annotations

import argparse

# Datasheet-level defaults -- TO BE MEASURED.
I0_UA_DEFAULT = 22.0      # deep sleep + ~2 uA analog wake circuit
Q_AS_DEFAULT = 0.3        # A*s per acquisition
PERIODIC_PER_DAY = 96     # one every 15 min
CELL_MAH_DEFAULT = 3000.0 # one 18650
ETA_DEFAULT = 0.8         # usable fraction of nameplate capacity

TARGET_DAYS = 182.5       # the six-month acceptance target


def average_current_ua(i0_ua: float, acquisitions_per_day: float, q_as: float) -> float:
    """I_avg in microamps."""
    return i0_ua + (acquisitions_per_day * q_as * 1e6) / 86_400.0


def life_days(i0_ua: float, acquisitions_per_day: float, q_as: float,
              cell_mah: float, eta: float) -> float:
    i_avg_ma = average_current_ua(i0_ua, acquisitions_per_day, q_as) / 1000.0
    return (eta * cell_mah / i_avg_ma) / 24.0


def max_event_wakes(i0_ua: float, q_as: float, cell_mah: float, eta: float,
                    target_days: float) -> float:
    """Event wakes per day that still just meet the target."""
    i_budget_ma = eta * cell_mah / (target_days * 24.0)
    i_event_ua = i_budget_ma * 1000.0 - i0_ua
    total = i_event_ua * 86_400.0 / (q_as * 1e6)
    return max(0.0, total - PERIODIC_PER_DAY)


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__,
                               formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--i0-ua", type=float, default=I0_UA_DEFAULT,
                   help="sleep + wake-circuit current, uA (MEASURE THIS)")
    p.add_argument("--q-as", type=float, default=Q_AS_DEFAULT,
                   help="charge per acquisition, A*s")
    p.add_argument("--cell-mah", type=float, default=CELL_MAH_DEFAULT)
    p.add_argument("--eta", type=float, default=ETA_DEFAULT)
    args = p.parse_args()

    print("BeeSense node battery life -- PDR eq. (3)")
    print(f"  I0 = {args.i0_ua:.1f} uA   Q = {args.q_as:.2f} A*s   "
          f"cell = {args.cell_mah:.0f} mAh   eta = {args.eta:.2f}")
    print(f"  periodic acquisitions = {PERIODIC_PER_DAY}/day (one per 15 min)\n")

    print("  event wakes/day   acquisitions/day   I_avg (mA)   life (days)   6-month target")
    for events in (0, 10, 20, 40, 60, 80, 120, 200):
        acquisitions = PERIODIC_PER_DAY + events
        i_avg = average_current_ua(args.i0_ua, acquisitions, args.q_as) / 1000.0
        days = life_days(args.i0_ua, acquisitions, args.q_as, args.cell_mah, args.eta)
        verdict = "PASS" if days >= TARGET_DAYS else "FAIL"
        print(f"  {events:>15}   {acquisitions:>16}   {i_avg:>10.3f}   "
              f"{days:>11.0f}   {verdict}")

    headroom = max_event_wakes(args.i0_ua, args.q_as, args.cell_mah, args.eta,
                               TARGET_DAYS)
    print(f"\n  Six months holds up to ~{headroom:.0f} event wakes/day.")
    print("  This is what sets the 20 min hold-off and the <= 20 false wakes/hive-day budget.")
    print("\n  TODO: re-run with the measured I0 and Q; plot this as PDR Fig. 2.")


if __name__ == "__main__":
    main()
