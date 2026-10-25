#!/usr/bin/env python3
"""Stage 1 bench test: fixed-point FFT runtime on the ESP32-C3.

The C3 is a single-core RISC-V with NO floating-point unit, so float spectra
are emulated in software and would be slow. Two things come out of this test:

  * the run time of a Q15 FFT at 1024 points / 2 kHz, which decides whether
    the band vector can be computed on the node at all, and
  * the energy per acquisition Q, since CPU time during the record is the
    dominant term in the battery model (feeds battery_life.py).

If 1024 points proves too slow, the fallbacks are fewer points, a lower sample
rate, or shipping decimated samples to the hub and banding them there.
"""


def main() -> None:
    print("bench_fft_timing -- Q15 FFT on the C3")
    print("  1. time a 1024-point Q15 FFT (esp-dsp vs own radix-2), averaged over many runs")
    print("  2. sweep sizes 256 / 512 / 1024 / 2048 -> runtime and RAM")
    print("  3. time the full path: window -> FFT -> |X|^2 -> 24 bands -> normalize")
    print("  4. multiply by the window count for a 10 s record -> CPU time per acquisition")
    print("  5. check Q15 headroom: does the magnitude accumulator saturate on a loud hive?")
    print("  6. cross-check the band vector against a float numpy reference on the same input")
    print()
    print("  Feed the resulting Q (A*s per acquisition) into battery_life.py")
    print("  TODO: on-target timing harness + the numpy reference implementation")


if __name__ == "__main__":
    main()
