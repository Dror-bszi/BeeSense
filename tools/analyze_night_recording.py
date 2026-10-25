#!/usr/bin/env python3
"""Stage 1, the decisive measurement: a night-time recording in a real hive.

At night the colony is quiet and the comb signal is at its weakest, so this is
where the circuit noise floor is either good enough or it is not. The PDR
commits to starting the field work with exactly this comparison.

Everything downstream -- the band vector, the baseline, the swarm rule --
assumes the signal clears the noise. This script is where that is checked
against real data rather than against datasheet numbers.
"""


def main() -> None:
    print("analyze_night_recording -- hive signal vs circuit noise floor")
    print("  input: raw piezo and mic captures from a node in a populated hive,")
    print("         plus a reference capture with the sensor disconnected")
    print()
    print("  1. PSD of the hive recording and of the noise reference, same scaling")
    print("  2. per-band SNR across 50-800 Hz -> which of the 24 bands are usable")
    print("  3. convert the piezo PSD to um/s^2 and compare with the 1-5 um/s^2 literature level")
    print("  4. piezo vs microphone on the same hive, same minute -> which channel to trust")
    print("  5. compute the normalized e_k (eq. 1) and check stability over hours")
    print("  6. look for queen piping and the daily activity cycle as sanity checks")
    print()
    print("  Output: usable band list and measured SNR, which set the detection thresholds")
    print("  TODO: WAV/CSV loader, Welch PSD, calibration from the piezo+amp transfer function")


if __name__ == "__main__":
    main()
