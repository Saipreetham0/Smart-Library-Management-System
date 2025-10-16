# Smart Library Management System - User Manual

## Table of Contents
1. [System Overview](#system-overview)
2. [Hardware Setup](#hardware-setup)
3. [Initial Configuration](#initial-configuration)
4. [How to Use Each Feature](#how-to-use-each-feature)
5. [LCD Display Messages](#lcd-display-messages)
6. [Buzzer Sound Guide](#buzzer-sound-guide)
7. [Firebase Dashboard](#firebase-dashboard)
8. [Troubleshooting](#troubleshooting)

---

## System Overview

The Smart Library Management System automates library operations using RFID/NFC technology with real-time monitoring and cloud database integration.

**Key Components:**
- ESP32 Microcontroller
- RFID RC522 Reader (for student cards only)
- NFC PN532 Reader (for book tags - borrowing/returning/lookup)
- 16x2 LCD Display (I2C)
- IR Sensors (Entry & Exit)
- Sound Sensor
- Buzzer
- Firebase Realtime Database

---

## Hardware Setup

### Pin Connections

| Component | Pin | ESP32 Pin |
|-----------|-----|-----------|
| **RFID RC522** |
| SS | → | GPIO 5 |
| RST | → | GPIO 4 |
| MOSI | → | GPIO 23 (SPI) |
| MISO | → | GPIO 19 (SPI) |
| SCK | → | GPIO 18 (SPI) |
| 3.3V | → | 3.3V |
| GND | → | GND |
| **NFC PN532** |
| SDA | → | GPIO 21 (I2C) |
| SCL | → | GPIO 22 (I2C) |
| VCC | → | 3.3V |
| GND | → | GND |
| **LCD 16x2 (I2C)** |
| SDA | → | GPIO 21 (I2C) |
| SCL | → | GPIO 22 (I2C) |
| VCC | → | 5V |
| GND | → | GND |
| **Other Components** |
| Buzzer | → | GPIO 15 |
| IR Entry Sensor | → | GPIO 32 |
| IR Exit Sensor | → | GPIO 33 |
| Sound Sensor | → | GPIO 34 (Analog) |

### Physical Setup
1. Mount IR sensors at library entrance (one on each side)
2. Place **RFID reader** at checkout counter (for student check-in/out)
3. Place **NFC reader** at checkout counter (for book operations)
4. Install LCD display at visible location
5. Position sound sensor in main reading area

---

## Initial Configuration

### 1. WiFi Setup
Edit in `src/main.cpp` (lines 25-26):
```cpp
const char* WIFI_SSID = "YourWiFiName";
const char* WIFI_PASSWORD = "YourWiFiPassword";
```

### 2. Firebase Setup
Edit in `src/main.cpp` (lines 29-32):
```cpp
#define API_KEY "your-firebase-api-key"
#define DATABASE_URL "your-firebase-database-url"
#define USER_EMAIL "your-email@example.com"
#define USER_PASSWORD "your-password"
```

### 3. Upload Code
```bash
# Using PlatformIO
pio run --target upload

# Monitor serial output
pio device monitor
```

### 4. System Initialization
After upload, the system will:
1. Initialize RFID and NFC modules
2. Connect to WiFi
3. Sync with NTP time server
4. Connect to Firebase
5. Load sample data (3 students, 2 books)
6. Display "Library System Ready!"

---

## How to Use Each Feature

### Feature 1: Student Check-In / Check-Out

**Purpose:** Track students entering and leaving the library

**Steps:**
1. Wait for LCD to show "Library System Ready!"
2. Student taps their RFID card on the RFID reader
3. System displays:
   - **First tap:** "Welcome! [Student Name]" (Check-In)
   - **Second tap:** "Goodbye! [Student Name]" (Check-Out)
4. Buzzer beeps once (200ms) for confirmation
5. People count updates automatically

**Sample Student Cards:**
| Student Name | Student ID | RFID Card UID |
|--------------|-----------|---------------|
| Student 1 | S001 | 13E31EA8 |
| Student 2 | S002 | D31333AD |
| Student 3 | S003 | 833620AD |

**Serial Monitor Output:**
```
✅ STUDENT CHECK-IN
   Name: Student 1
   ID: S001
   Time: 2025-10-16 14:30:45
✅ Data saved to Firebase
```

---

### Feature 2: Borrow a Book

**Purpose:** Issue books to students

**Steps:**
1. Scan the **book's NFC tag** using the NFC reader (PN532)
2. LCD shows: "Scan Student RFID Card"
3. Within 10 seconds, scan the **student's RFID card** using the RFID reader (RC522)
4. LCD shows: "Book Borrowed [Book Title]"
5. Buzzer beeps once (200ms)
6. Transaction logged to Firebase

**Available Books:**
| Book Title | Book ID | NFC Tag UID | Shelf Location |
|------------|---------|-------------|----------------|
| Arduino Guide | B001 | E7D2B865 | A1 |
| ESP32 Projects | B002 | 7340AFFD | A2 |

**Error Messages:**
- "Timeout! Try Again" - No student card scanned within 10 seconds
- "Book Not Found" - NFC tag not in database
- "Unknown Card" - Student RFID card not in database

**Serial Monitor Output:**
```
📖 BOOK BORROWED
   Book: Arduino Guide
   Student: Student 1
   Method: NFC Tag -> RFID Card
✅ Data saved to Firebase
```

---

### Feature 3: Return a Book

**Purpose:** Accept returned books

**Steps:**
1. Scan the **book's NFC tag** using the NFC reader (PN532)
2. LCD shows: "Scan Student RFID Card"
3. Scan the **student's RFID card** using the RFID reader (RC522) - must be the same student who borrowed it
4. LCD shows: "Book Returned [Book Title]"
5. Buzzer beeps once (200ms)
6. Book marked as available in database

**Error Messages:**
- "Wrong Student! Not your book" - Different student trying to return (double beep alert)
- "Timeout! Try Again" - No student card scanned within 10 seconds

**Serial Monitor Output:**
```
📚 BOOK RETURNED
   Book: Arduino Guide
   Student: Student 1
   Method: NFC Tag -> RFID Card
✅ Data saved to Firebase
```

---

### Feature 4: Book Information Lookup (NFC)

**Purpose:** Find book details and shelf location

**Steps:**
1. Student taps **book's NFC tag** on NFC reader
2. LCD shows:
   - Line 1: Book title
   - Line 2: Shelf location (e.g., "Shelf: A1")
3. Buzzer beeps once (150ms)
4. Display clears after 3 seconds

**Serial Monitor Output:**
```
📚 BOOK FOUND
   Title: Arduino Guide
   Author: Tech Author
   ID: B001
   Location: A1
   Status: Available
```

---

### Feature 5: Occupancy Monitoring

**Purpose:** Track number of people in library

**How It Works:**
- **Entry IR Sensor** detects person entering → Count increases
- **Exit IR Sensor** detects person leaving → Count decreases
- LCD shows: "Entry/Exit Detected Count: X"
- Buzzer beeps once
- Real-time count synced to Firebase

**Automatic Updates:**
- Student check-in also increments count
- Student check-out also decrements count
- Count never goes below 0

**View Live Count:**
- Firebase path: `/stats/peopleCount`

---

### Feature 6: Noise Level Monitoring

**Purpose:** Maintain quiet environment

**How It Works:**
- Sound sensor continuously monitors ambient noise
- **Threshold:** 500 (adjustable in code line 87)
- When noise exceeds threshold:
  - LCD shows: "QUIET PLEASE! Noise: XXX"
  - Double beep alert (100ms + 100ms)
  - Alert logged to Firebase
  - 5-second cooldown before next alert

**Adjust Threshold:**
Edit in `src/main.cpp` (line 87):
```cpp
int noiseThreshold = 500;  // Change value (0-4095)
```

**Firebase Alert Log:**
- Path: `/alerts/noise/{timestamp}`
- Contains: noise level value

---

## LCD Display Messages

| Message | Meaning | Action Required |
|---------|---------|-----------------|
| "Library System Ready!" | System idle | Scan a card/tag |
| "Welcome! [Name]" | Student checked in | None |
| "Goodbye! [Name]" | Student checked out | None |
| "Scan Student RFID Card" | Waiting for student card | Scan student RFID card |
| "Book Borrowed [Title]" | Book issued | None |
| "Book Returned [Title]" | Book returned | None |
| "Wrong Student! Not your book" | Return error | Scan correct student |
| "Timeout! Try Again" | No card scanned | Restart transaction |
| "Unknown Card" | Student card not registered | Check database |
| "Book Not Found" | Book NFC tag not registered | Check database |
| "Entry Detected Count: X" | Someone entered | None |
| "Exit Detected Count: X" | Someone exited | None |
| "QUIET PLEASE! Noise: XXX" | Noise alert | Lower noise |
| "[Book Title] Shelf: XX" | Book info lookup | None |

---

## Buzzer Sound Guide

| Beep Pattern | Meaning | Context |
|--------------|---------|---------|
| **Single Beep (200ms)** | Success | Check-in, Check-out, Book borrow/return |
| **Single Beep (150ms)** | Info Found | Book lookup via NFC |
| **Single Beep (100ms)** | Entry/Exit | Person detected by IR sensors |
| **Double Beep (100ms + 100ms)** | Error/Alert | Wrong card, Unknown card, Noise alert |

---

## Firebase Dashboard

### Access Your Database
URL: `https://[your-project]-default-rtdb.asia-southeast1.firebasedatabase.app/`

### Database Structure

```
📁 Root
├── 📁 students/
│   ├── 📁 S001/
│   │   ├── name: "Student 1"
│   │   ├── rfidCard: "13E31EA8"
│   │   ├── isCheckedIn: true/false
│   │   ├── booksBorrowed: 2
│   │   ├── lastCheckIn: "2025-10-16 14:30:45"
│   │   └── lastCheckOut: "2025-10-16 16:20:10"
│   ├── 📁 S002/
│   └── 📁 S003/
│
├── 📁 books/
│   ├── 📁 B001/
│   │   ├── title: "Arduino Guide"
│   │   ├── author: "Tech Author"
│   │   ├── rfidTag: "E7D2B865" (NFC tag UID)
│   │   ├── shelf: "A1"
│   │   ├── isAvailable: true/false
│   │   ├── borrowedBy: "S001"
│   │   ├── borrowedTime: "2025-10-16 14:35:20"
│   │   └── returnedTime: "2025-10-16 16:15:30"
│   └── 📁 B002/
│
├── 📁 transactions/
│   ├── 📁 1697456789123/
│   │   ├── type: "CHECK_IN" / "BORROW" / "RETURN" / "CHECK_OUT"
│   │   ├── studentId: "S001"
│   │   ├── studentName: "Student 1"
│   │   ├── bookId: "B001"
│   │   ├── bookTitle: "Arduino Guide"
│   │   └── timestamp: "2025-10-16 14:35:20"
│   └── 📁 1697456790456/
│
├── 📁 stats/
│   ├── totalStudents: 3
│   ├── totalBooks: 2
│   ├── peopleCount: 5
│   ├── totalTransactions: 0
│   └── lastSync: "2025-10-16 16:30:00"
│
└── 📁 alerts/
    └── 📁 noise/
        ├── 📁 1697456791789/
        │   └── soundLevel: 650
        └── 📁 1697456792123/
```

### Useful Firebase Queries

**Check who is currently in library:**
```
/students → Filter by isCheckedIn: true
```

**View all borrowed books:**
```
/books → Filter by isAvailable: false
```

**See today's transactions:**
```
/transactions → Sort by timestamp (descending)
```

**Monitor noise alerts:**
```
/alerts/noise → View all noise incidents
```

**Live occupancy count:**
```
/stats/peopleCount
```

---

## Troubleshooting

### Problem: LCD shows "Initializing..." forever

**Causes:**
- LCD not connected properly
- Wrong I2C address

**Solutions:**
1. Check LCD I2C connections (SDA=21, SCL=22)
2. Verify LCD address (default: 0x27)
3. Run I2C scanner to find correct address:
```cpp
// Upload I2C scanner sketch to find LCD address
```

---

### Problem: RFID cards not detected

**Causes:**
- Wrong SPI wiring
- Card too far from reader
- Power supply issue

**Solutions:**
1. Check RFID wiring (SS=5, RST=4, SPI pins correct)
2. Hold card within 2-3cm of reader
3. Verify 3.3V power supply to RFID module
4. Check Serial Monitor for "✅ RFID Initialized"

---

### Problem: "Unknown Card" or "Book Not Found" message

**Causes:**
- Student RFID card UID not in database
- Book NFC tag UID not in database
- Different card/tag than sample data

**Solutions:**
1. Read card/tag UID from Serial Monitor:
   - `[RFID SCANNED] UID: XXXXXXXX` (for student cards)
   - `[NFC SCANNED] UID: XXXXXXXX` (for book tags)
2. Add card/tag to database in `initializeSampleData()` function (line 685)
3. Update student RFID UID or book NFC UID with actual card/tag UID
4. Re-upload code

**Example for Student:**
```cpp
students[0] = {"S001", "Student 1", "YOUR_RFID_UID_HERE", 0, false, 0, {"", "", "", "", ""}};
```

**Example for Book:**
```cpp
books[0] = {"B001", "Arduino Guide", "Tech Author", "YOUR_NFC_UID_HERE", true, "", 0, 0, "A1"};
```

---

### Problem: WiFi connection fails

**Causes:**
- Wrong SSID/Password
- WiFi out of range
- 5GHz network (ESP32 needs 2.4GHz)

**Solutions:**
1. Verify WiFi credentials in code (lines 25-26)
2. Check WiFi signal strength
3. Use 2.4GHz WiFi network only
4. System works offline - Firebase features disabled

---

### Problem: Firebase not connecting

**Causes:**
- Wrong API key or Database URL
- WiFi not connected
- Authentication failed

**Solutions:**
1. Check Firebase credentials (lines 29-32)
2. Verify WiFi connected first (check Serial Monitor)
3. Check Firebase console for authentication errors
4. Ensure Firebase user email/password correct
5. Check Firebase rules allow read/write access

---

### Problem: NFC not working

**Causes:**
- PN532 not detected
- I2C conflict with LCD
- Wrong NFC module mode

**Solutions:**
1. Check Serial Monitor for "⚠️ No PN532 NFC module found!"
2. Verify I2C connections (both NFC and LCD on same bus)
3. Ensure PN532 is in I2C mode (DIP switches)
4. NFC uses different frequency - some cards may not work

---

### Problem: IR sensors always triggered

**Causes:**
- Sensors too sensitive
- Ambient light interference
- Wrong sensor type (active vs passive)

**Solutions:**
1. Adjust IR sensor potentiometer (if available)
2. Shield sensors from direct sunlight
3. Check sensor output with Serial Monitor
4. Verify sensor type (active IR with reflector)

---

### Problem: Noise alerts constantly triggering

**Causes:**
- Threshold too low
- Sensor too sensitive
- Electrical noise

**Solutions:**
1. Increase noise threshold (line 87):
```cpp
int noiseThreshold = 800;  // Increase value
```
2. Check sound sensor analog reading in Serial Monitor
3. Add capacitor to sensor output (100nF)
4. Move sensor away from ESP32/buzzer

---

### Problem: Buzzer not beeping

**Causes:**
- Wrong pin connection
- Passive buzzer (needs tone)
- Power issue

**Solutions:**
1. Check buzzer connected to GPIO 15
2. Use **active buzzer** (has built-in oscillator)
3. Test buzzer directly with 3.3V
4. Check buzzer polarity (+ to GPIO, - to GND)

---

### Problem: "Timeout! Try Again" when borrowing/returning

**Cause:**
- Student RFID card not scanned within 10 seconds after scanning book NFC tag

**Solution:**
- Scan book NFC tag first, then quickly scan student RFID card within 10 seconds
- Adjust timeout in code (line 476):
```cpp
while (millis() - waitStart < 15000) {  // Increase to 15 seconds
```

---

### Problem: Serial Monitor shows garbage characters

**Cause:**
- Wrong baud rate

**Solution:**
- Set Serial Monitor to **115200 baud** (line 120)

---

## Adding New Students/Books

### Add New Student

Edit `initializeSampleData()` function (line 685):

```cpp
students[3] = {"S004", "John Doe", "ABCD1234", 0, false, 0, {"", "", "", "", ""}};
studentCount = 4;  // Update count
```

**Format:**
```cpp
{"StudentID", "Name", "RFID_UID", checkInTime, isCheckedIn, booksBorrowed, borrowedBooks}
```

### Add New Book

Edit `initializeSampleData()` function (line 691):

```cpp
books[2] = {"B003", "Python Basics", "Code Master", "12345678", true, "", 0, 0, "B1"};
bookCount = 3;  // Update count
```

**Format:**
```cpp
{"BookID", "Title", "Author", "NFC_UID", isAvailable, borrowedBy, borrowedTime, dueTime, "ShelfLocation"}
```

**Note:** Books use NFC tags, not RFID tags. Obtain the NFC UID from Serial Monitor when scanning.

---

## System Maintenance

### Regular Tasks

**Daily:**
- Check Firebase transaction logs
- Verify occupancy count accuracy
- Test RFID readers

**Weekly:**
- Review noise alert frequency
- Check WiFi connection stability
- Backup Firebase data

**Monthly:**
- Clean RFID/NFC readers
- Check all sensor connections
- Update student/book database

---

## Technical Specifications

| Component | Specification |
|-----------|--------------|
| **Microcontroller** | ESP32 (dual-core, WiFi/BT) |
| **RFID Reader** | MFRC522 (13.56MHz, ISO14443A) |
| **NFC Reader** | PN532 (13.56MHz, I2C mode) |
| **LCD Display** | 16x2 Character, I2C (PCF8574) |
| **IR Sensors** | Active IR obstacle sensor |
| **Sound Sensor** | Analog microphone (0-4095) |
| **Buzzer** | Active 5V buzzer |
| **Power** | 5V/2A USB or external supply |
| **Operating Temp** | 0°C to 50°C |

---

## Contact & Support

**Serial Monitor Commands:**
- Open: `pio device monitor`
- Baud Rate: 115200
- Shows all events, errors, and confirmations

**Firebase Console:**
- https://console.firebase.google.com/

**PlatformIO Commands:**
```bash
pio run              # Compile
pio run -t upload    # Upload to ESP32
pio device monitor   # Serial monitor
pio run -t clean     # Clean build
```

---

## Quick Reference Card

### 🔵 Student Operations
| Action | Steps |
|--------|-------|
| Check In | Tap student RFID card → Welcome message |
| Check Out | Tap student RFID card again → Goodbye message |

### 📚 Book Operations
| Action | Steps |
|--------|-------|
| Borrow | Scan book NFC tag → Scan student RFID card → Confirmed |
| Return | Scan book NFC tag → Scan student RFID card → Confirmed |
| Search | Tap book NFC tag → See title & shelf |

### 👥 Monitoring
| Feature | How It Works |
|---------|--------------|
| Occupancy | IR sensors auto-count entry/exit |
| Noise | Alert when sound > threshold |

### 📱 Live Data
Firebase: `/stats/peopleCount` - Live occupancy
Firebase: `/transactions/` - All activity logs

---

**System Version:** 1.0
**Last Updated:** October 2025
**Platform:** PlatformIO + ESP32 + Firebase

---

## License & Credits

This is a mini project for educational purposes.

**Components Used:**
- ESP32 DevKit
- MFRC522 RFID Module
- PN532 NFC Module
- LCD1602 with I2C
- IR Sensors
- Sound Sensor Module
- Active Buzzer
- Firebase Realtime Database

---

*End of User Manual*
