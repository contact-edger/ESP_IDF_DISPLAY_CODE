# Wiring Diagram - Waveshare 2.9" E-Paper Display V2 to ESP32 WROOM

```
┌─────────────────────────────────────────────────────────────────┐
│                    WAVESHARE 2.9" E-PAPER V2                    │
│                         (296x128 pixels)                        │
│                                                                 │
│  ┌─────────────────────────────────────────────────────────┐   │
│  │                                                         │   │
│  │                                                         │   │
│  │                    HELLO WORLD!                         │   │
│  │                                                         │   │
│  │              Waveshare 2.9" E-Paper V2                  │   │
│  │               ESP32 WROOM Dev Kit                       │   │
│  │                  NiswiQ Display                         │   │
│  │                                                         │   │
│  └─────────────────────────────────────────────────────────┘   │
│                                                                 │
│  [BUSY] [RST] [DC] [CS] [CLK] [DIN] [GND] [VCC]              │
│    │     │     │    │     │     │     │     │                  │
└────┼─────┼─────┼────┼─────┼─────┼─────┼─────┼──────────────────┘
     │     │     │    │     │     │     │     │
     │     │     │    │     │     │     │     │
     ▼     ▼     ▼    ▼     ▼     ▼     ▼     ▼
┌────────────────────────────────────────────────────────────────┐
│                     ESP32 WROOM DEV KIT                        │
│                                                                │
│   [D4]  [D15] [D19] [D5] [D18] [D23] [GND] [3V3]             │
│   GPIO4 GPIO15 GPIO19 GPIO5 GPIO18 GPIO23                     │
│                                                                │
│   ┌──────────────────────────────────────────────────┐       │
│   │                                                  │       │
│   │              ESP32-WROOM-32                      │       │
│   │                                                  │       │
│   └──────────────────────────────────────────────────┘       │
│                                                                │
│   [USB]  ← Connect to Computer for Power & Programming       │
│    │                                                          │
└────┼───────────────────────────────────────────────────────────┘
     │
     └──→ USB Cable to Computer
```

## Pin Mapping Table

| Function      | E-Paper Pin | ESP32 Pin | GPIO   | Notes                |
|---------------|-------------|-----------|--------|----------------------|
| Busy Signal   | BUSY        | D4        | GPIO4  | Input - Display busy |
| Reset         | RST         | D15       | GPIO15 | Output - Reset line  |
| Data/Command  | DC          | D19       | GPIO19 | Output - D/C select  |
| Chip Select   | CS          | D5        | GPIO5  | Output - SPI CS      |
| SPI Clock     | CLK         | D18       | GPIO18 | Output - SPI CLK     |
| SPI Data In   | DIN         | D23       | GPIO23 | Output - SPI MOSI    |
| Ground        | GND         | GND       | GND    | Common ground        |
| Power         | VCC         | 3V3       | 3.3V   | **Important: 3.3V!** |

## Important Notes

### ⚠️ VOLTAGE WARNING
- **ONLY use 3.3V** for VCC connection
- **DO NOT** connect to 5V - this will damage the display
- The ESP32 outputs 3.3V logic levels, which is perfect for this display

### Connection Tips
1. Use short wires to minimize interference
2. Ensure solid connections - loose wires cause display issues
3. Connect GND first, then VCC, then signal pins
4. Double-check all connections before powering on

### SPI Bus Info
- SPI Mode: Mode 0 (CPOL=0, CPHA=0)
- Max Speed: 10 MHz (configured automatically)
- MOSI (Master Out Slave In): DIN on display
- No MISO needed - display doesn't send data back

### Testing
After uploading the code:
1. Display should initialize (BUSY pin goes HIGH then LOW)
2. Display refreshes (takes 2-3 seconds)
3. "Hello World!" appears on screen
4. Display enters sleep mode
5. Image remains on screen even after power off

## Breadboard Layout Suggestion

```
ESP32                          E-Paper Display
[D4] ────(wire)────────────── [BUSY]
[D15]────(wire)────────────── [RST]
[D19]────(wire)────────────── [DC]
[D5] ────(wire)────────────── [CS]
[D18]────(wire)────────────── [CLK]
[D23]────(wire)────────────── [DIN]
[GND]────(wire)────────────── [GND]
[3V3]────(wire)────────────── [VCC]
```

## Color Coding Recommendation (if using colored wires)
- Red: VCC (3.3V)
- Black: GND
- Blue: Data lines (BUSY, DC, DIN)
- Green: Clock (CLK)
- Yellow: Control (CS, RST)
