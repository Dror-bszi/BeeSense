#!/usr/bin/env python3
"""Stage 1 field test: ESP-NOW range and delivery at the apiary.

Target: packet delivery >= 95%.

Two known difficulties: the SuperMini's chip antenna is small, and honey and
wood attenuate 2.4 GHz badly -- which is why the node's antenna sits outside
the hive. The documented fallback if range does not hold is LoRa.
"""


def main() -> None:
    print("bench_espnow_range -- node to hub link")
    print("  1. bench back-to-back: delivery and RSSI at 1 m -> the reference")
    print("  2. open ground sweep: 5 / 10 / 25 / 50 / 100 m, N packets each")
    print("  3. antenna inside the hive vs outside -> quantify the honey/wood loss")
    print("  4. at the real apiary, from each hive position to the planned hub spot")
    print("  5. with the hives populated and the boxes closed (bees and comb change it)")
    print("  6. retry behaviour: does the node's retry budget recover a marginal link?")
    print()
    print("  PASS: >= 95% delivery from every hive position, with margin")
    print("  FAIL -> move the hub, add an external antenna, or fall back to LoRa")
    print("  TODO: drive a node into a send loop over serial, tally acks and RSSI")


if __name__ == "__main__":
    main()
