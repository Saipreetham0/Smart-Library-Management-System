# 🏷️ How to Setup Tags - SIMPLE VERSION

## 3 Easy Steps!

### Step 1: Upload the Scanner

1. Open Arduino IDE
2. Open file: `tools/tag_config_tool.ino`
3. Select Board: **ESP32 Dev Module**
4. Click **Upload**

### Step 2: Scan Your Tags

1. Open **Serial Monitor** (115200 baud)
2. **Place tag** near the reader
3. You'll see this:

```
✅ TAG FOUND!
════════════════════════════════════════
Type: RFID (MIFARE 1K)
UID:  04:A3:2D:C2:5E:3F:80

📝 COPY THIS:
────────────────────────────────────────
// For Student Card:
students[X].rfidCard = "04:A3:2D:C2:5E:3F:80";

// For Book Tag:
books[X].rfidCard = "04:A3:2D:C2:5E:3F:80";
════════════════════════════════════════
```

### Step 3: Add to Your Main Code

1. Open `src/main.cpp`
2. Find function `initializeSampleData()` (around line 640)
3. **Paste** the line you copied:

```cpp
void initializeSampleData() {
  // Student 1
  studentCount = 1;
  students[0].studentId = "STU001";
  students[0].name = "John Doe";
  students[0].rfidCard = "04:A3:2D:C2:5E:3F:80";  // ← PASTE HERE
  students[0].isCheckedIn = false;
  students[0].booksBorrowed = 0;

  // Book 1
  bookCount = 1;
  books[0].bookId = "BOOK001";
  books[0].title = "Arduino Guide";
  books[0].author = "Jane Smith";
  books[0].rfidTag = "04:B5:1A:D3:2F:4C:90";  // ← PASTE HERE
  books[0].isAvailable = true;
  books[0].shelfLocation = "A-101";
}
```

4. **Upload** your main code
5. **Test** - Scan the tag and it should work!

---

## That's It! ✅

- ✅ **ONE tool** to scan all tags
- ✅ **Copy & paste** - no typing UIDs manually
- ✅ **Works** for both RFID and NFC
- ✅ **Simple** - just 3 steps

---

## Don't Need to Write Tags! ❌

**You DON'T need to write data to tags!**

Just:
1. Scan the tag UID
2. Copy it
3. Add to your code

The UID is already on the tag from the factory!

---

## Wiring (Quick Reference)

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

---

## Troubleshooting

**Q: Tag not detected?**
- Move tag closer (< 3cm)
- Check wiring
- Try different orientation

**Q: Wrong UID format?**
- Just copy what the tool shows
- Don't edit it manually

**Q: Still not working?**
- Make sure you uploaded the main code after adding UIDs
- Check that X in `students[X]` matches your array index
- Verify WiFi and Firebase are connected

---

**That's all you need! 🎉**
