# 🚀 QUICK START GUIDE
## Waveshare 2.9" E-Paper Display with ESP32

### ✅ What You've Got
A complete ESP-IDF project that displays "Hello World!" on your Waveshare 2.9" e-paper display.

---

## 📋 Step-by-Step Setup

### 1️⃣ Hardware Setup (5 minutes)
Connect the display to your ESP32 following this table:

| Display | ESP32 Pin | Description |
|---------|-----------|-------------|
| BUSY    | D4        | Busy signal |
| RST     | D15       | Reset |
| DC      | D19       | Data/Command |
| CS      | D5        | Chip Select |
| CLK     | D18       | SPI Clock |
| DIN     | D23       | SPI Data |
| GND     | GND       | Ground |
| VCC     | 3V3       | ⚠️ **3.3V ONLY!** |

**📄 See WIRING_DIAGRAM.md for detailed connections**

---

### 2️⃣ Software Build (2 minutes)

#### Option A: Using ESP-IDF (Recommended)
```powershell
# 1. Set target
idf.py set-target esp32

# 2. Build
idf.py build

# 3. Flash (replace COM3 with your port)
idf.py -p COM3 flash monitor
```

#### Option B: Using PlatformIO
```powershell
# 1. Rename platformio.ini.example to platformio.ini
# 2. Build and upload
pio run --target upload
pio device monitor
```

**📄 See BUILD_INSTRUCTIONS.md for detailed build steps**

---

### 3️⃣ What to Expect

#### Serial Output (115200 baud):
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

#### On the Display:
```
┌──────────────────────────┐
│                          │
│      Hello World!        │
│                          │
│  Waveshare 2.9" E-Paper  │
│   ESP32 WROOM Dev Kit    │
│     NiswiQ Display       │
│                          │
└──────────────────────────┘
```

---

## 🎨 Customization Ideas

### Change Text Size
```cpp
display.setTextSize(3);  // Larger text (1-4)
```

### Change Text Position
```cpp
display.setCursor(x, y);  // x and y in pixels
```

### Add More Text
```cpp
display.setCursor(50, 200);
display.print("Your custom text here!");
```

### Change Orientation
```cpp
display.setRotation(0);  // Try 0, 1, 2, or 3
```

---

## 🔧 Troubleshooting

| Problem | Solution |
|---------|----------|
| Display blank | Check VCC is 3.3V, verify all connections |
| Build errors | Ensure ESP-IDF is installed, Arduino component available |
| Upload fails | Check COM port, try different USB cable |
| No serial output | Verify baud rate is 115200 |
| Garbled display | Check SPI connections (CLK, DIN, CS) |

---

## 📚 Documentation Files

- **README.md** - Complete project overview and features
- **WIRING_DIAGRAM.md** - Detailed pin connections with diagrams
- **BUILD_INSTRUCTIONS.md** - Build and flash instructions
- **platformio.ini.example** - PlatformIO configuration (optional)

---

## 🎯 Next Steps

1. ✅ Wire up the display
2. ✅ Build and flash the code
3. ✅ See "Hello World!" on display
4. 🎨 Customize the text and layout
5. 🚀 Build your own e-paper project!

---

## 📁 Project Structure
```
NiswiQ_display_esp_code_test/
├── main/
│   ├── main.cpp              ← Your application code
│   └── CMakeLists.txt
├── components/
│   └── GxEPD2/               ← Display driver library
│       ├── GxEPD2_BW.h
│       ├── GxEPD2_EPD.cpp
│       ├── GxEPD2_EPD.h
│       ├── GxEPD2_290_T94_V2.cpp
│       ├── GxEPD2_290_T94_V2.h
│       └── CMakeLists.txt
├── CMakeLists.txt            ← Main project config
├── README.md                 ← Project documentation
├── BUILD_INSTRUCTIONS.md     ← How to build
├── WIRING_DIAGRAM.md         ← Pin connections
└── QUICK_START.md            ← This file!
```

---

## ⚡ Key Features
- ✅ Simple "Hello World" example
- ✅ Proper pin configuration for your exact setup
- ✅ Low power - display sleeps after update
- ✅ Clean, commented code
- ✅ Complete documentation

---

## 💡 Tips
- E-paper displays are **slow** (2-3 seconds to update) - this is normal!
- The image **stays on screen** even when powered off
- Avoid updating too frequently - limit to once every few seconds
- The display draws power **only during updates**

---

## 🎉 Success!
If you see "Hello World!" on your display, **congratulations!** 
You've successfully set up your ESP32 with e-paper display.

---

**Made with ❤️ for NiswiQ Display Project**  
*October 25, 2025*
