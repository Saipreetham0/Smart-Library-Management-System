/**
 * ════════════════════════════════════════════════════════
 * SIMPLE TAG CONFIGURATION TOOL
 * Smart Library Management System
 * ════════════════════════════════════════════════════════
 *
 * ONE TOOL FOR EVERYTHING:
 * - Scan RFID tags
 * - Scan NFC tags
 * - Get UIDs
 * - Copy & paste to your code
 *
 * SUPER EASY TO USE!
 * ════════════════════════════════════════════════════════
 */

#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <Adafruit_PN532.h>

// Pin Configuration
#define RFID_SS_PIN 5
#define RFID_RST_PIN 4
#define PN532_IRQ 2
#define PN532_RESET 3

// Initialize readers
MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n\n");
  Serial.println("════════════════════════════════════════");
  Serial.println("   SIMPLE TAG SCANNER");
  Serial.println("   Just scan and copy the UID!");
  Serial.println("════════════════════════════════════════\n");

  // Initialize RFID
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("✅ RFID Reader ready");

  // Initialize NFC
  Wire.begin();
  nfc.begin();
  uint32_t version = nfc.getFirmwareVersion();
  if (version) {
    nfc.SAMConfig();
    Serial.println("✅ NFC Reader ready");
  } else {
    Serial.println("⚠️  NFC Reader not found (optional)");
  }

  Serial.println("\n📋 INSTRUCTIONS:");
  Serial.println("   1. Place tag near reader");
  Serial.println("   2. Copy the UID shown");
  Serial.println("   3. Paste in your code");
  Serial.println("\n════════════════════════════════════════");
  Serial.println("Waiting for tags...\n");
}

void loop() {
  // Try RFID first
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    showRFIDTag();
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    delay(2000);
    return;
  }

  // Try NFC
  uint8_t uid[7];
  uint8_t uidLength;
  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100)) {
    showNFCTag(uid, uidLength);
    delay(2000);
    return;
  }

  delay(100);
}

void showRFIDTag() {
  Serial.println("\n✅ TAG FOUND!");
  Serial.println("════════════════════════════════════════");

  // Get UID WITHOUT colons (matches what the system reads)
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uid += "0";
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();

  Serial.print("Type: RFID (");
  Serial.print(rfid.PICC_GetTypeName(rfid.PICC_GetType(rfid.uid.sak)));
  Serial.println(")");
  Serial.print("UID:  ");
  Serial.println(uid);
  Serial.println();

  // Show copy-paste code
  Serial.println("📝 COPY THIS:");
  Serial.println("────────────────────────────────────────");
  Serial.println("// For Student Card:");
  Serial.println("students[X].rfidCard = \"" + uid + "\";");
  Serial.println();
  Serial.println("// For Book Tag:");
  Serial.println("books[X].rfidTag = \"" + uid + "\";");
  Serial.println("════════════════════════════════════════\n");
}

void showNFCTag(uint8_t* uid, uint8_t uidLength) {
  Serial.println("\n✅ TAG FOUND!");
  Serial.println("════════════════════════════════════════");

  // Get UID WITHOUT colons (matches what the system reads)
  String uidStr = "";
  for (byte i = 0; i < uidLength; i++) {
    if (uid[i] < 0x10) uidStr += "0";
    uidStr += String(uid[i], HEX);
  }
  uidStr.toUpperCase();

  Serial.print("Type: NFC (");
  Serial.print(uidLength);
  Serial.println(" bytes)");
  Serial.print("UID:  ");
  Serial.println(uidStr);
  Serial.println();

  // Show copy-paste code
  Serial.println("📝 COPY THIS:");
  Serial.println("────────────────────────────────────────");
  Serial.println("// For Book Tag:");
  Serial.println("books[X].rfidTag = \"" + uidStr + "\";");
  Serial.println("════════════════════════════════════════\n");
}
