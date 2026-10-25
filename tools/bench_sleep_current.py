#!/usr/bin/env python3
"""Stage 1 bench test: the node's real sleep current.

I0 ~= 22 uA is a datasheet-level estimate and is the single most load-bearing
assumption in the project: the six-month target comes straight out of it.

On a SuperMini board the power LED and the onboard regulator can each draw
more than the sleeping chip, so the measured figure may be an order of
magnitude worse than the datasheet. Finding that out early is the point --
the fix (desolder the LED, replace the regulator with an MCP1700/HT7333) is
cheap now and expensive after the enclosures are sealed.
"""


def main() -> None:
    print("bench_sleep_current -- node power, measured")
    print("  1. bare SuperMini in deep sleep -> baseline board current")
    print("  2. desolder/disable the power LED, re-measure -> LED cost")
    print("  3. swap the regulator for a low-Iq part, re-measure -> regulator cost")
    print("  4. add each peripheral in turn (HX711 parked, piezo amp off, mic stopped,")
    print("     DS18B20 idle, wake circuit live) -> per-peripheral contribution to I0")
    print("  5. capture one full acquisition with a power analyzer; integrate -> Q (A*s)")
    print("  6. confirm the radio and the amp really are off before sleep")
    print()
    print("  Then: battery_life.py --i0-ua <measured> --q-as <measured>")
    print("  TODO: power-analyzer capture import and integration")


if __name__ == "__main__":
    main()
