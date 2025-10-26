# 📚 Smart Library Management System

ESP32-based library system with RFID/NFC, Firebase, and real-time monitoring.

## 🚀 Quick Setup

### 1. Configure WiFi & Firebase

Edit `src/main.cpp` (lines 22-29):

```cpp
// WiFi
const char* WIFI_SSID = "YourWiFiName";
const char* WIFI_PASSWORD = "YourPassword";

// Firebase (already configured)
#define API_KEY "AIzaSyCI0gN-FwbTzfS43_LURWjHAr_JfwDaglM"
#define DATABASE_URL "https://smart-library-using-esp32-default-rtdb.firebaseio.com/"
```

### 2. Setup Your Tags

**See: [HOW_TO_SETUP_TAGS.md](HOW_TO_SETUP_TAGS.md)** ⭐

Quick version:
1. Upload `tools/tag_config_tool.ino` to ESP32
2. Scan tags → Copy UIDs
3. Paste in `initializeSampleData()` function

### 3. Upload Main Code

```bash
platformio run --target upload
```

## 📁 Project Structure

```
├── src/main.cpp              ← Main library system
├── tools/
│   └── tag_config_tool.ino   ← Simple tag scanner (USE THIS!)
├── HOW_TO_SETUP_TAGS.md      ← Setup guide ⭐
└── README.md                 ← This file
```

## 🔌 Hardware Wiring

### MFRC522 RFID Reader
```
MFRC522 → ESP32
SDA   → GPIO 5
SCK   → GPIO 18
MOSI  → GPIO 23
MISO  → GPIO 19
RST   → GPIO 4
GND   → GND
3.3V  → 3.3V
```

### PN532 NFC Reader (Optional)
```
PN532 → ESP32
SDA   → GPIO 21
SCL   → GPIO 22
GND   → GND
3.3V  → 3.3V
```

### Other Components
```
LCD I2C    → GPIO 21 (SDA), GPIO 22 (SCL)
Buzzer     → GPIO 15
IR Entry   → GPIO 32
IR Exit    → GPIO 33
Sound Sensor → GPIO 34
```

## ✨ Features

- ✅ RFID student check-in/out
- ✅ Book borrowing/returning
- ✅ NFC book search
- ✅ People counter (IR sensors)
- ✅ Noise monitoring
- ✅ Firebase real-time sync
- ✅ LCD display
- ✅ Transaction logging

## 🔧 Libraries Required

Already configured in `platformio.ini`:
- MFRC522
- LiquidCrystal_I2C
- Adafruit_PN532
- ArduinoJson
- FirebaseClient

## 🌐 Web Dashboard

A modern Next.js web dashboard is included for real-time monitoring and management!

### Features:
- 📊 Real-time statistics dashboard
- 👥 Student management (add, edit, delete)
- 📚 Book inventory management
- 📝 Transaction history
- 🔔 Noise alerts monitoring
- 📱 Mobile responsive design

### Quick Start:
```bash
cd web-dashboard
pnpm install
pnpm dev
```
Open [http://localhost:3000](http://localhost:3000)

### Deploy to Vercel (FREE):
```bash
cd web-dashboard
vercel
```

**See: [VERCEL_DEPLOY.md](VERCEL_DEPLOY.md)** for deployment guide

## 📖 Documentation

- **[HOW_TO_SETUP_TAGS.md](HOW_TO_SETUP_TAGS.md)** - Tag setup guide (START HERE!)
- **[WEB_DASHBOARD_INFO.md](WEB_DASHBOARD_INFO.md)** - Web dashboard guide
- **[VERCEL_DEPLOY.md](VERCEL_DEPLOY.md)** - Deploy to Vercel
- **`tools/tag_config_tool.ino`** - Simple tag scanner tool

## 🆘 Need Help?

**Tag not working?**
→ See [HOW_TO_SETUP_TAGS.md](HOW_TO_SETUP_TAGS.md)

**WiFi/Firebase issues?**
→ Check credentials in `src/main.cpp` lines 22-29

**Compilation errors?**
→ Run `platformio run` and check output

## 📊 System Status

- ✅ Main code: Clean and working
- ✅ Tag config tool: Simple one-file solution
- ✅ Documentation: Simplified
- ✅ Build: Successful (RAM: 21%, Flash: 87.9%)

---

**Made simple! 🎉**
