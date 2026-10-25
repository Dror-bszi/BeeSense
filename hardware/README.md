# Hardware

Schematics and board files for the parts that are not off-the-shelf modules.

The component catalog — every part, order reference, price and per-component
note — lives in the docs repo at
[BeeSense-Docs/hardware](https://github.com/Dror-bszi/BeeSense-Docs/tree/main/hardware).
This directory is for the circuits built around those parts.

## To be drawn

| | |
|---|---|
| `wake-circuit/` | The nanopower piezo wake circuit: nanopower amp (e.g. TLV8541) + comparator (e.g. TLV3691), adaptive threshold tracking the background, output to an RTC-capable GPIO. Budget ~2 µA. **This is the circuit the whole power story rests on.** |
| `node-board/` | Node carrier: ESP32-C3 SuperMini, piezo amp path (MCP6002 + bias + protection diodes), HX711 header, 1-Wire, battery holder, low-Iq regulator (MCP1700/HT7333), IP65 gland entries. |
| `hub-board/` | Hub carrier: ESP32-S3, cellular modem, solar charge path (CN3791, 18 V in), 18650 pack. |
| `scale-mechanics/` | Load-cell mounts and base board. Cells must sit so only their centre carries load and the edges are free to move — not glued flat to a board. |

## Decisions already made

- **ESP-NOW, not LoRa**, between nodes and hub — the hives stand close together.
  LoRa stays the documented fallback if the range measurement disappoints.
- **Piezo in the comb as the primary sensor**, microphone as the cross-check.
  Comb vibration is 1–5 µm/s² at night, below typical MEMS accelerometer noise,
  which rules out the accelerometer originally considered.
- **Node antenna outside the hive** — honey and wood attenuate 2.4 GHz.
- **Hub gets the solar panel, nodes get cells.** The hub is the receiver, so it
  cannot sleep; 15 W against ~0.5 W of draw is comfortable.
