#!/usr/bin/env python3
"""Stage 1 bench test: the weighing chain.

Target: weight error <= +-20 g over 0-200 kg (4 x 50 kg cells, one HX711).

The HX711's own noise is worth about 3 g, so it is not the limit. The limit is
mechanical and thermal, which is what this test is really after:
  * cells must be mounted so only their centre carries load,
  * drift with temperature is expected to dominate the error budget.
"""


def main() -> None:
    print("bench_hx711 -- weighing chain")
    print("  1. tare the empty platform, record raw counts and spread")
    print("  2. calibrate with one reference mass -> counts_per_g")
    print("  3. sweep reference masses across 0-200 kg; record error at each point")
    print("  4. repeat the sweep, reloading the SAME mass -> hysteresis and repeatability")
    print("  5. temperature sweep at constant load (fridge -> sun) -> drift in g/degC")
    print("  6. corner-load test: same mass at each corner -> cell matching / mount quality")
    print()
    print("  PASS: |error| <= 20 g across the range, and drift correctable from the ambient probe")
    print("  TODO: talk to the node over serial, log (reference_g, raw, temp_c) to CSV")


if __name__ == "__main__":
    main()
