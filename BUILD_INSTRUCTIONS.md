# Waveshare 2.9" E-Paper Display - Build and Flash Instructions

## Prerequisites
1. ESP-IDF must be installed and configured
2. ESP32 toolchain must be in your PATH
3. Arduino component for ESP-IDF must be installed

## Quick Start Commands

### 1. Set Target to ESP32
```powershell
idf.py set-target esp32
```

### 2. Build the Project
```powershell
idf.py build
```

### 3. Flash to ESP32 (replace COM3 with your port)
```powershell
idf.py -p COM3 flash
```

### 4. Monitor Serial Output
```powershell
idf.py -p COM3 monitor
```

### 5. Build, Flash, and Monitor (all in one)
```powershell
idf.py -p COM3 flash monitor
```

## Finding Your COM Port
In PowerShell, run:
```powershell
Get-WmiObject Win32_SerialPort | Select-Object Name,DeviceID
```

Or check in Device Manager under "Ports (COM & LPT)"

## Expected Serial Output
```
=================================
Waveshare 2.9" e-Paper Display Test
=================================
SPI initialized
Initializing display...
Display initialized
Drawing to display...
Display updated successfully!
Content: Hello World!
Display in sleep mode.
=================================
```

## Troubleshooting

### Arduino.h not found
Install Arduino component for ESP-IDF:
```bash
cd %IDF_PATH%\components
git clone https://github.com/espressif/arduino-esp32.git arduino
cd arduino
git submodule update --init --recursive
```

### Build errors
Clean and rebuild:
```powershell
idf.py fullclean
idf.py build
```

### Display shows nothing
1. Check all pin connections
2. Ensure VCC is connected to 3.3V
3. Verify BUSY pin is properly connected
4. Check serial monitor for error messages

### Display shows garbled output
1. Try different rotation values (0-3)
2. Verify SPI pins are correctly connected
3. Check if MOSI (DIN) and CLK are properly wired

## Exit Monitor Mode
Press `Ctrl + ]` to exit the serial monitor.
