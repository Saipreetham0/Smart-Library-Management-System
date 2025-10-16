#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_PN532.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// Provide the token generation process info
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info
#include "addons/RTDBHelper.h"

// ─── PIN CONFIGURATION ───────────────────────────────
#define RFID_SS_PIN 5
#define RFID_RST_PIN 4
#define BUZZER_PIN 15

#define IR_ENTRY 32
#define IR_EXIT 33
#define SOUND_SENSOR 34

// ─── WiFi CONFIGURATION ──────────────────────────────
const char* WIFI_SSID = "KSP";
const char* WIFI_PASSWORD = "Admin@123";

// ─── FIREBASE CONFIGURATION ──────────────────────────
#define API_KEY "AIzaSyCI0gN-FwbTzfS43_LURWjHAr_JfwDaglM"
#define DATABASE_URL "https://smart-library-using-esp32-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define USER_EMAIL "user@Kspelectronics.in"
#define USER_PASSWORD "123456789"

// ─── RFID SETUP ──────────────────────────────────────
MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);

// ─── LCD SETUP ───────────────────────────────────────
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ─── NFC (PN532 I2C) SETUP ───────────────────────────
// Using I2C mode (SDA=21, SCL=22) - No IRQ/RESET pins needed
Adafruit_PN532 nfc(-1, -1);

// ─── FIREBASE SETUP ──────────────────────────────────
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool firebaseReady = false;

// ─── DATA STRUCTURES ─────────────────────────────────
struct Student {
  String studentId;
  String name;
  String rfidCard;
  unsigned long checkInTime;
  bool isCheckedIn;
  int booksBorrowed;
  String borrowedBooks[5];
};

struct Book {
  String bookId;
  String title;
  String author;
  String rfidTag;
  bool isAvailable;
  String borrowedBy;
  unsigned long borrowedTime;
  unsigned long dueTime;
  String shelfLocation;
};

// ─── DATABASE (In-Memory Storage with Firebase Sync) ─
#define MAX_STUDENTS 50
#define MAX_BOOKS 100

Student students[MAX_STUDENTS];
Book books[MAX_BOOKS];

int studentCount = 0;
int bookCount = 0;
int transactionCount = 0;

// ─── SYSTEM VARIABLES ────────────────────────────────
int peopleCount = 0;
int noiseThreshold = 500;
unsigned long lastScan = 0;
String currentStudentRFID = "";
unsigned long lastNoiseAlert = 0;
unsigned long lastFirebaseSync = 0;

// NTP Time Server
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;

// ─── FUNCTION DECLARATIONS ───────────────────────────
String readRFID();
String readNFC();
void beep(int duration);
void displayStatus(String line1, String line2);
void handleOccupancy();
void checkNoise();
void initializeFirebase();
void initializeSampleData();
String getFormattedTime();
void handleStudentCheckInOut(String rfidCard);
void handleBookTransaction(String bookRFID);
void findBookByNFC(String nfcTag);
int findStudentByRFID(String rfid);
int findBookByRFID(String rfid);
void syncStudentToFirebase(int index);
void syncBookToFirebase(int index);
void syncStatsToFirebase();
void addTransactionToFirebase(String studentId, String bookId, String type);

// ─── SETUP ───────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  Serial.println("\n\n========================================");
  Serial.println("   SMART LIBRARY MANAGEMENT SYSTEM");
  Serial.println("   Firebase Realtime Database");
  Serial.println("========================================\n");

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  displayStatus("Initializing...", "Please Wait");

  // Initialize SPI for RFID
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("✅ RFID Initialized");

  // Initialize I2C for NFC and LCD
  Wire.begin();
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("⚠️  No PN532 NFC module found!");
  } else {
    nfc.SAMConfig();
    Serial.println("✅ NFC Ready");
  }

  // Initialize Pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(IR_ENTRY, INPUT);
  pinMode(IR_EXIT, INPUT);
  pinMode(SOUND_SENSOR, INPUT);

  // Initialize WiFi
  displayStatus("Connecting WiFi", "...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");

  int wifiAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
    delay(500);
    Serial.print(".");
    wifiAttempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi Connected!");
    Serial.print("   IP Address: ");
    Serial.println(WiFi.localIP());
    displayStatus("WiFi Connected", WiFi.localIP().toString());
    delay(2000);

    // Configure time (required for Firebase)
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    Serial.print("⏰ Syncing time");
    int timeAttempts = 0;
    while (time(nullptr) < 100000 && timeAttempts < 20) {
      Serial.print(".");
      delay(500);
      timeAttempts++;
    }
    Serial.println("\n✅ NTP Time Configured");

    // Initialize Firebase
    initializeFirebase();
  } else {
    Serial.println("\n⚠️  WiFi Connection Failed");
    displayStatus("WiFi Failed", "Offline Mode");
    delay(2000);
  }

  // Initialize sample data
  initializeSampleData();

  displayStatus("Library System", "Ready!");
  beep(200);
  delay(1000);
  Serial.println("\n========================================");
  Serial.println("System Ready - Scan RFID/NFC Cards");
  Serial.println("Firebase Console:");
  Serial.println(DATABASE_URL);
  Serial.println("========================================\n");
}

// ─── LOOP ───────────────────────────────────────────
void loop() {
  // Periodic Firebase sync every 30 seconds
  if (firebaseReady && (millis() - lastFirebaseSync > 30000)) {
    syncStatsToFirebase();
    lastFirebaseSync = millis();
  }

  // Check for RFID scan (Student Cards or Book Tags)
  String uidRFID = readRFID();
  if (uidRFID != "") {
    Serial.println("\n[RFID SCANNED] UID: " + uidRFID);

    // Check if it's a student card
    int studentIndex = findStudentByRFID(uidRFID);
    if (studentIndex != -1) {
      handleStudentCheckInOut(uidRFID);
    } else {
      // Check if it's a book
      int bookIndex = findBookByRFID(uidRFID);
      if (bookIndex != -1) {
        handleBookTransaction(uidRFID);
      } else {
        displayStatus("Unknown Card", uidRFID.substring(0, 12));
        beep(100);
        delay(100);
        beep(100);
        Serial.println("⚠️  Unknown RFID Tag");
        delay(2000);
        displayStatus("Library System", "Ready!");
      }
    }

    lastScan = millis();
  }

  // Check for NFC scan (Book Search)
  String uidNFC = readNFC();
  if (uidNFC != "") {
    Serial.println("\n[NFC SCANNED] UID: " + uidNFC);
    findBookByNFC(uidNFC);
    lastScan = millis();
  }

  // Handle occupancy sensors
  handleOccupancy();

  // Check noise levels
  checkNoise();

  delay(100);
}

// ─── RFID READER ─────────────────────────────────────
String readRFID() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return "";
  }

  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  return uid;
}

// ─── NFC READER ──────────────────────────────────────
String readNFC() {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100);

  if (success && uidLength == 4) {
    String uidStr = "";
    for (byte i = 0; i < uidLength; i++) {
      if (uid[i] < 0x10) uidStr += "0";
      uidStr += String(uid[i], HEX);
    }
    uidStr.toUpperCase();
    return uidStr;
  }

  return "";
}

// ─── DISPLAY & BEEPER ────────────────────────────────
void beep(int duration) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration);
  digitalWrite(BUZZER_PIN, LOW);
}

void displayStatus(String line1, String line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

// ─── OCCUPANCY HANDLING ──────────────────────────────
void handleOccupancy() {
  static bool lastEntryState = HIGH;
  static bool lastExitState = HIGH;
  static unsigned long lastDebounce = 0;

  bool entryState = digitalRead(IR_ENTRY);
  bool exitState = digitalRead(IR_EXIT);

  if (millis() - lastDebounce > 500) {
    if (entryState == LOW && lastEntryState == HIGH) {
      peopleCount++;
      Serial.println("👤 Person Entered | Count: " + String(peopleCount));
      displayStatus("Entry Detected", "Count: " + String(peopleCount));
      beep(100);

      if (firebaseReady) {
        Firebase.RTDB.setInt(&fbdo, "/stats/peopleCount", peopleCount);
      }

      delay(1000);
      displayStatus("Library System", "Ready!");
      lastDebounce = millis();
    }

    if (exitState == LOW && lastExitState == HIGH) {
      if (peopleCount > 0) peopleCount--;
      Serial.println("👋 Person Exited | Count: " + String(peopleCount));
      displayStatus("Exit Detected", "Count: " + String(peopleCount));
      beep(100);

      if (firebaseReady) {
        Firebase.RTDB.setInt(&fbdo, "/stats/peopleCount", peopleCount);
      }

      delay(1000);
      displayStatus("Library System", "Ready!");
      lastDebounce = millis();
    }
  }

  lastEntryState = entryState;
  lastExitState = exitState;
}

// ─── NOISE DETECTION ─────────────────────────────────
void checkNoise() {
  int soundLevel = analogRead(SOUND_SENSOR);

  if (soundLevel > noiseThreshold && millis() - lastNoiseAlert > 5000) {
    Serial.println("🔊 High Noise Detected: " + String(soundLevel));
    displayStatus("QUIET PLEASE!", "Noise: " + String(soundLevel));
    beep(100);
    delay(100);
    beep(100);

    if (firebaseReady) {
      String timestamp = String(millis());
      Firebase.RTDB.setInt(&fbdo, ("/alerts/noise/" + timestamp).c_str(), soundLevel);
    }

    lastNoiseAlert = millis();
    delay(2000);
    displayStatus("Library System", "Ready!");
  }
}

// ─── STUDENT CHECK IN/OUT ────────────────────────────
void handleStudentCheckInOut(String rfidCard) {
  int index = findStudentByRFID(rfidCard);
  if (index == -1) return;

  Student &student = students[index];

  if (!student.isCheckedIn) {
    // Check In
    student.isCheckedIn = true;
    student.checkInTime = millis();
    peopleCount++;

    displayStatus("Welcome!", student.name);
    beep(200);

    Serial.println("\n✅ STUDENT CHECK-IN");
    Serial.println("   Name: " + student.name);
    Serial.println("   ID: " + student.studentId);
    Serial.println("   Time: " + getFormattedTime());

    if (firebaseReady) {
      String path = "/students/" + student.studentId;
      Firebase.RTDB.setString(&fbdo, (path + "/name").c_str(), student.name);
      Firebase.RTDB.setString(&fbdo, (path + "/rfidCard").c_str(), student.rfidCard);
      Firebase.RTDB.setBool(&fbdo, (path + "/isCheckedIn").c_str(), true);
      Firebase.RTDB.setString(&fbdo, (path + "/lastCheckIn").c_str(), getFormattedTime());
      Firebase.RTDB.setInt(&fbdo, (path + "/booksBorrowed").c_str(), student.booksBorrowed);

      Firebase.RTDB.setInt(&fbdo, "/stats/peopleCount", peopleCount);

      String txPath = "/transactions/" + String(millis());
      Firebase.RTDB.setString(&fbdo, (txPath + "/type").c_str(), "CHECK_IN");
      Firebase.RTDB.setString(&fbdo, (txPath + "/studentId").c_str(), student.studentId);
      Firebase.RTDB.setString(&fbdo, (txPath + "/studentName").c_str(), student.name);
      Firebase.RTDB.setString(&fbdo, (txPath + "/timestamp").c_str(), getFormattedTime());

      Serial.println("✅ Data saved to Firebase");
    }

    delay(2000);
  } else {
    // Check Out
    student.isCheckedIn = false;
    if (peopleCount > 0) peopleCount--;

    displayStatus("Goodbye!", student.name);
    beep(200);

    Serial.println("\n👋 STUDENT CHECK-OUT");
    Serial.println("   Name: " + student.name);
    Serial.println("   ID: " + student.studentId);

    if (firebaseReady) {
      String path = "/students/" + student.studentId;
      Firebase.RTDB.setBool(&fbdo, (path + "/isCheckedIn").c_str(), false);
      Firebase.RTDB.setString(&fbdo, (path + "/lastCheckOut").c_str(), getFormattedTime());

      Firebase.RTDB.setInt(&fbdo, "/stats/peopleCount", peopleCount);

      String txPath = "/transactions/" + String(millis());
      Firebase.RTDB.setString(&fbdo, (txPath + "/type").c_str(), "CHECK_OUT");
      Firebase.RTDB.setString(&fbdo, (txPath + "/studentId").c_str(), student.studentId);
      Firebase.RTDB.setString(&fbdo, (txPath + "/studentName").c_str(), student.name);
      Firebase.RTDB.setString(&fbdo, (txPath + "/timestamp").c_str(), getFormattedTime());

      Serial.println("✅ Data saved to Firebase");
    }

    delay(2000);
  }

  displayStatus("Library System", "Ready!");
}

// ─── BOOK TRANSACTION ────────────────────────────────
void handleBookTransaction(String bookRFID) {
  int bookIndex = findBookByRFID(bookRFID);
  if (bookIndex == -1) return;

  Book &book = books[bookIndex];

  // Need to scan student card first
  displayStatus("Scan Student", "Card Now");
  beep(100);

  unsigned long waitStart = millis();
  while (millis() - waitStart < 10000) {
    String studentRFID = readRFID();
    if (studentRFID != "") {
      int studentIndex = findStudentByRFID(studentRFID);
      if (studentIndex != -1) {
        Student &student = students[studentIndex];

        if (book.isAvailable) {
          // Borrow Book
          book.isAvailable = false;
          book.borrowedBy = student.studentId;
          book.borrowedTime = millis();
          student.booksBorrowed++;

          displayStatus("Book Borrowed", book.title.substring(0, 16));
          beep(200);

          Serial.println("\n📖 BOOK BORROWED");
          Serial.println("   Book: " + book.title);
          Serial.println("   Student: " + student.name);

          if (firebaseReady) {
            String bookPath = "/books/" + book.bookId;
            Firebase.RTDB.setString(&fbdo, (bookPath + "/title").c_str(), book.title);
            Firebase.RTDB.setString(&fbdo, (bookPath + "/author").c_str(), book.author);
            Firebase.RTDB.setString(&fbdo, (bookPath + "/rfidTag").c_str(), book.rfidTag);
            Firebase.RTDB.setString(&fbdo, (bookPath + "/shelf").c_str(), book.shelfLocation);
            Firebase.RTDB.setBool(&fbdo, (bookPath + "/isAvailable").c_str(), false);
            Firebase.RTDB.setString(&fbdo, (bookPath + "/borrowedBy").c_str(), student.studentId);
            Firebase.RTDB.setString(&fbdo, (bookPath + "/borrowedTime").c_str(), getFormattedTime());

            String studentPath = "/students/" + student.studentId;
            Firebase.RTDB.setInt(&fbdo, (studentPath + "/booksBorrowed").c_str(), student.booksBorrowed);

            String txPath = "/transactions/" + String(millis());
            Firebase.RTDB.setString(&fbdo, (txPath + "/type").c_str(), "BORROW");
            Firebase.RTDB.setString(&fbdo, (txPath + "/studentId").c_str(), student.studentId);
            Firebase.RTDB.setString(&fbdo, (txPath + "/studentName").c_str(), student.name);
            Firebase.RTDB.setString(&fbdo, (txPath + "/bookId").c_str(), book.bookId);
            Firebase.RTDB.setString(&fbdo, (txPath + "/bookTitle").c_str(), book.title);
            Firebase.RTDB.setString(&fbdo, (txPath + "/timestamp").c_str(), getFormattedTime());

            Serial.println("✅ Data saved to Firebase");
          }

          delay(2000);
        } else {
          // Return Book
          if (book.borrowedBy == student.studentId) {
            book.isAvailable = true;
            book.borrowedBy = "";
            if (student.booksBorrowed > 0) student.booksBorrowed--;

            displayStatus("Book Returned", book.title.substring(0, 16));
            beep(200);

            Serial.println("\n📚 BOOK RETURNED");
            Serial.println("   Book: " + book.title);
            Serial.println("   Student: " + student.name);

            if (firebaseReady) {
              String bookPath = "/books/" + book.bookId;
              Firebase.RTDB.setBool(&fbdo, (bookPath + "/isAvailable").c_str(), true);
              Firebase.RTDB.setString(&fbdo, (bookPath + "/borrowedBy").c_str(), "");
              Firebase.RTDB.setString(&fbdo, (bookPath + "/returnedTime").c_str(), getFormattedTime());

              String studentPath = "/students/" + student.studentId;
              Firebase.RTDB.setInt(&fbdo, (studentPath + "/booksBorrowed").c_str(), student.booksBorrowed);

              String txPath = "/transactions/" + String(millis());
              Firebase.RTDB.setString(&fbdo, (txPath + "/type").c_str(), "RETURN");
              Firebase.RTDB.setString(&fbdo, (txPath + "/studentId").c_str(), student.studentId);
              Firebase.RTDB.setString(&fbdo, (txPath + "/studentName").c_str(), student.name);
              Firebase.RTDB.setString(&fbdo, (txPath + "/bookId").c_str(), book.bookId);
              Firebase.RTDB.setString(&fbdo, (txPath + "/bookTitle").c_str(), book.title);
              Firebase.RTDB.setString(&fbdo, (txPath + "/timestamp").c_str(), getFormattedTime());

              Serial.println("✅ Data saved to Firebase");
            }

            delay(2000);
          } else {
            displayStatus("Wrong Student!", "Not your book");
            beep(100);
            delay(100);
            beep(100);
            delay(2000);
          }
        }

        displayStatus("Library System", "Ready!");
        return;
      }
    }
    delay(100);
  }

  displayStatus("Timeout!", "Try Again");
  beep(100);
  delay(2000);
  displayStatus("Library System", "Ready!");
}

// ─── FIND BOOK BY NFC ────────────────────────────────
void findBookByNFC(String nfcTag) {
  int bookIndex = findBookByRFID(nfcTag);

  if (bookIndex != -1) {
    Book &book = books[bookIndex];

    displayStatus(book.title.substring(0, 16), "Shelf: " + book.shelfLocation);
    beep(150);

    Serial.println("\n📚 BOOK FOUND");
    Serial.println("   Title: " + book.title);
    Serial.println("   Author: " + book.author);
    Serial.println("   ID: " + book.bookId);
    Serial.println("   Location: " + book.shelfLocation);
    Serial.println("   Status: " + String(book.isAvailable ? "Available" : "On Loan"));

    delay(3000);
    displayStatus("Library System", "Ready!");
  } else {
    displayStatus("Book Not Found", nfcTag.substring(0, 12));
    beep(100);
    delay(100);
    beep(100);
    Serial.println("⚠️  Book not found in database");
    delay(2000);
    displayStatus("Library System", "Ready!");
  }
}

// ─── HELPER FUNCTIONS ────────────────────────────────
int findStudentByRFID(String rfid) {
  for (int i = 0; i < studentCount; i++) {
    if (students[i].rfidCard == rfid) return i;
  }
  return -1;
}

int findBookByRFID(String rfid) {
  for (int i = 0; i < bookCount; i++) {
    if (books[i].rfidTag == rfid) return i;
  }
  return -1;
}

String getFormattedTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "Time N/A";
  }

  char timeStr[20];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(timeStr);
}

// ─── FIREBASE FUNCTIONS ──────────────────────────────
void initializeFirebase() {
  Serial.println("\n─── Initializing Firebase ───");

  // Assign the API key
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL
  config.database_url = DATABASE_URL;

  // Assign the callback function for token generation
  config.token_status_callback = tokenStatusCallback;

  // Initialize the library
  Firebase.begin(&config, &auth);

  // Enable auto reconnect
  Firebase.reconnectWiFi(true);

  // Wait for token to be ready
  Serial.print("Getting Firebase token");
  displayStatus("Firebase", "Getting token...");

  unsigned long startTime = millis();
  while (!Firebase.ready() && millis() - startTime < 30000) {
    delay(100);
    Serial.print(".");
  }

  if (Firebase.ready()) {
    firebaseReady = true;
    Serial.println("\n✅ Firebase Connected!");
    Serial.println("   Database: " + String(DATABASE_URL));
    displayStatus("Firebase", "Connected!");
    delay(1500);

    // Initialize database structure
    Serial.println("   Initializing database structure...");
    Firebase.RTDB.setInt(&fbdo, "/stats/totalStudents", 3);
    Firebase.RTDB.setInt(&fbdo, "/stats/totalBooks", 2);
    Firebase.RTDB.setInt(&fbdo, "/stats/peopleCount", 0);
    Firebase.RTDB.setInt(&fbdo, "/stats/totalTransactions", 0);

    Serial.println("✅ Firebase Ready!");
  } else {
    Serial.println("\n⚠️  Firebase Connection Failed");
    Serial.println("   Check your WiFi and Firebase credentials");
    firebaseReady = false;
  }
}

void initializeSampleData() {
  Serial.println("\n─── Initializing Sample Data ───");

  // Students with RFID Cards (UIDs WITHOUT colons)
  students[0] = {"S001", "Student 1", "13E31EA8", 0, false, 0, {"", "", "", "", ""}};
  students[1] = {"S002", "Student 2", "D31333AD", 0, false, 0, {"", "", "", "", ""}};
  students[2] = {"S003", "Student 3", "833620AD", 0, false, 0, {"", "", "", "", ""}};
  studentCount = 3;

  // Books with NFC Tags (UIDs WITHOUT colons)
  books[0] = {"B001", "Arduino Guide", "Tech Author", "E7D2B865", true, "", 0, 0, "A1"};
  books[1] = {"B002", "ESP32 Projects", "IoT Expert", "7340AFFD", true, "", 0, 0, "A2"};
  bookCount = 2;

  Serial.println("✅ Sample Data Initialized");
  Serial.println("   Students: " + String(studentCount));
  Serial.println("   Books: " + String(bookCount));

  // Upload initial data to Firebase
  if (firebaseReady) {
    Serial.println("   Uploading to Firebase...");

    for (int i = 0; i < studentCount; i++) {
      String path = "/students/" + students[i].studentId;
      Firebase.RTDB.setString(&fbdo, (path + "/name").c_str(), students[i].name);
      Firebase.RTDB.setString(&fbdo, (path + "/rfidCard").c_str(), students[i].rfidCard);
      Firebase.RTDB.setBool(&fbdo, (path + "/isCheckedIn").c_str(), false);
      Firebase.RTDB.setInt(&fbdo, (path + "/booksBorrowed").c_str(), 0);
      Firebase.RTDB.setString(&fbdo, (path + "/lastCheckIn").c_str(), "");
    }

    for (int i = 0; i < bookCount; i++) {
      String path = "/books/" + books[i].bookId;
      Firebase.RTDB.setString(&fbdo, (path + "/title").c_str(), books[i].title);
      Firebase.RTDB.setString(&fbdo, (path + "/author").c_str(), books[i].author);
      Firebase.RTDB.setString(&fbdo, (path + "/rfidTag").c_str(), books[i].rfidTag);
      Firebase.RTDB.setString(&fbdo, (path + "/shelf").c_str(), books[i].shelfLocation);
      Firebase.RTDB.setBool(&fbdo, (path + "/isAvailable").c_str(), true);
      Firebase.RTDB.setString(&fbdo, (path + "/borrowedBy").c_str(), "");
    }

    Serial.println("✅ Data uploaded to Firebase");
  }
}

void syncStatsToFirebase() {
  if (!firebaseReady) return;

  Firebase.RTDB.setInt(&fbdo, "/stats/totalStudents", studentCount);
  Firebase.RTDB.setInt(&fbdo, "/stats/totalBooks", bookCount);
  Firebase.RTDB.setInt(&fbdo, "/stats/peopleCount", peopleCount);
  Firebase.RTDB.setString(&fbdo, "/stats/lastSync", getFormattedTime());

  Serial.println("🔄 Stats synced to Firebase");
}

void syncStudentToFirebase(int index) {
  if (!firebaseReady || index < 0 || index >= studentCount) return;

  Student &student = students[index];
  String path = "/students/" + student.studentId;

  Firebase.RTDB.setString(&fbdo, (path + "/name").c_str(), student.name);
  Firebase.RTDB.setString(&fbdo, (path + "/rfidCard").c_str(), student.rfidCard);
  Firebase.RTDB.setBool(&fbdo, (path + "/isCheckedIn").c_str(), student.isCheckedIn);
  Firebase.RTDB.setInt(&fbdo, (path + "/booksBorrowed").c_str(), student.booksBorrowed);
}

void syncBookToFirebase(int index) {
  if (!firebaseReady || index < 0 || index >= bookCount) return;

  Book &book = books[index];
  String path = "/books/" + book.bookId;

  Firebase.RTDB.setString(&fbdo, (path + "/title").c_str(), book.title);
  Firebase.RTDB.setString(&fbdo, (path + "/author").c_str(), book.author);
  Firebase.RTDB.setString(&fbdo, (path + "/rfidTag").c_str(), book.rfidTag);
  Firebase.RTDB.setString(&fbdo, (path + "/shelf").c_str(), book.shelfLocation);
  Firebase.RTDB.setBool(&fbdo, (path + "/isAvailable").c_str(), book.isAvailable);
  Firebase.RTDB.setString(&fbdo, (path + "/borrowedBy").c_str(), book.borrowedBy);
}

void addTransactionToFirebase(String studentId, String bookId, String type) {
  if (!firebaseReady) return;

  String path = "/transactions/" + String(millis());
  Firebase.RTDB.setString(&fbdo, (path + "/studentId").c_str(), studentId);
  Firebase.RTDB.setString(&fbdo, (path + "/bookId").c_str(), bookId);
  Firebase.RTDB.setString(&fbdo, (path + "/type").c_str(), type);
  Firebase.RTDB.setString(&fbdo, (path + "/timestamp").c_str(), getFormattedTime());
}
