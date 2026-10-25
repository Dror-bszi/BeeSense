#!/usr/bin/env python3
"""Stage 1 bench test: piezo chain and the nanopower wake circuit.

Two questions, and the first one gates the whole project:

  1. SENSITIVITY. Night-time comb vibration is 1-5 um/s^2 per frequency bin,
     and ten times weaker on the hive wall. Can the piezo-plus-amp chain see
     that above its own noise floor? If not, the in-comb placement and the amp
     design have to change before anything else matters.

  2. THE WAKE CIRCUIT. It must fire on a real vibration burst at ~2 uA, with an
     adaptive threshold that tracks the background rather than a fixed one --
     a hive is far louder at noon than at 3 am.
"""


def main() -> None:
    print("bench_piezo -- vibration chain and wake circuit")
    print(" sensitivity:")
    print("  1. amp noise floor with the piezo shorted -> the number to beat")
    print("  2. known tones through a speaker coupled to a frame, 50-800 Hz sweep")
    print("  3. response vs frequency; confirm the 100-300 Hz swarming band is covered")
    print("  4. piezo in comb vs on the hive wall -> quantify the ~10x the literature reports")
    print(" wake circuit:")
    print("  5. measure quiescent current (target ~2 uA) -- the 6-month life depends on it")
    print("  6. burst amplitude at which it triggers; vary the background level")
    print("  7. false-trigger rate over an hour of ambient noise (budget: <= 20/hive-day)")
    print("  8. confirm it wakes the C3 from deep sleep on an RTC-capable GPIO")
    print()
    print("  TODO: drive the speaker from this script, capture the node's ADC dump over serial")


if __name__ == "__main__":
    main()
