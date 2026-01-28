// NANO E - Using hardware Serial on TX1 (TX) and RX0 (RX)

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN_1 9
#define SS_PIN_1 2
#define SS_PIN_2 3
#define SS_PIN_3 4

MFRC522 rfid1(SS_PIN_1, RST_PIN_1);
MFRC522 rfid2(SS_PIN_2, RST_PIN_1);
MFRC522 rfid3(SS_PIN_3, RST_PIN_1);

unsigned long lastReadRFID1 = 0;
unsigned long lastReadRFID2 = 0;
unsigned long lastReadRFID3 = 0;
const unsigned long debounceDelay = 1000; // 1000 ms debounce interval

String readUID(MFRC522 &rfid) {
  String uidString = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uidString += String(rfid.uid.uidByte[i], HEX);
  }
  return uidString;
}

void setup() {
  Serial.begin(9600);
  SPI.begin();

  // Set chip select pins as outputs and deselect all readers initially.
  pinMode(SS_PIN_1, OUTPUT);
  pinMode(SS_PIN_2, OUTPUT);
  pinMode(SS_PIN_3, OUTPUT);
  digitalWrite(SS_PIN_1, HIGH);
  digitalWrite(SS_PIN_2, HIGH);
  digitalWrite(SS_PIN_3, HIGH);

  // Initialize all RFID modules.
  rfid1.PCD_Init();
  Serial.println("RFID E1 connected");
  rfid2.PCD_Init();
  Serial.println("RFID E2 connected");
  rfid3.PCD_Init();
  Serial.println("RFID F1 connected");
}

void loop() {
  unsigned long currentMillis = millis();

  // -----------------------------
  // Process RFID reader 1 (D1)
  // -----------------------------
  // Deselect all readers.
  digitalWrite(SS_PIN_1, HIGH);
  digitalWrite(SS_PIN_2, HIGH);
  digitalWrite(SS_PIN_3, HIGH);
  // Activate reader 1.
  digitalWrite(SS_PIN_1, LOW);
  delay(5); // Allow chip select to settle

  if (currentMillis - lastReadRFID1 > debounceDelay) {
    if (rfid1.PICC_IsNewCardPresent() && rfid1.PICC_ReadCardSerial()) {
      String uid = readUID(rfid1);
      uid.toLowerCase();  // Convert UID to lowercase for consistency
      
      // Check for red card UIDs
      if (uid == "53135a10" || uid == "a35abb10" || uid == "d3f74310" ||
          uid == "c3c55510" || uid == "d3325d10" || uid == "7312d927" ||
          uid == "63873029" || uid == "a3b96d29" || uid == "436d2d10") {
        Serial.println("E1_RED");
      }
      // Check for green card UIDs
      else if (uid == "43864828" || uid == "93552512" || uid == "73463010" ||
               uid == "530e5c10" || uid == "73606210" || uid == "297ac54a" ||
               uid == "b3604510" || uid == "435e4410" || uid == "73281e2a" ||
               uid == "7348ace4" || uid == "b3276010") {
        Serial.println("E1_GREEN");
      }
      // Check for the RESTART UID
      else if (uid == "e3d168f7") {
        Serial.println("RESTART");
      }
      rfid1.PICC_HaltA();
      lastReadRFID1 = currentMillis; // Update timestamp for reader 1
    }
  }
  digitalWrite(SS_PIN_1, HIGH);

  // -----------------------------
  // Process RFID reader 2 (D2)
  // -----------------------------
  digitalWrite(SS_PIN_1, HIGH);
  digitalWrite(SS_PIN_2, HIGH);
  digitalWrite(SS_PIN_3, HIGH);
  digitalWrite(SS_PIN_2, LOW);
  delay(5); // Allow chip select to settle

  if (currentMillis - lastReadRFID2 > debounceDelay) {
    if (rfid2.PICC_IsNewCardPresent() && rfid2.PICC_ReadCardSerial()) {
      String uid = readUID(rfid2);
      uid.toLowerCase();  // Convert UID to lowercase for consistency
      
      // Check for red card UIDs
      if (uid == "53135a10" || uid == "a35abb10" || uid == "d3f74310" ||
          uid == "c3c55510" || uid == "d3325d10" || uid == "7312d927" ||
          uid == "63873029" || uid == "a3b96d29" || uid == "436d2d10") {
        Serial.println("E2_RED");
      }
      // Check for green card UIDs
      else if (uid == "43864828" || uid == "93552512" || uid == "73463010" ||
               uid == "530e5c10" || uid == "73606210" || uid == "297ac54a" ||
               uid == "b3604510" || uid == "435e4410" || uid == "73281e2a" ||
               uid == "7348ace4" || uid == "b3276010") {
        Serial.println("E2_GREEN");
      }
      // Check for the RESTART UID
      else if (uid == "e3d168f7") {
        Serial.println("RESTART");
      }
      rfid2.PICC_HaltA();
      lastReadRFID2 = currentMillis; // Update timestamp for reader 2
    }
  }
  digitalWrite(SS_PIN_2, HIGH);

  // -----------------------------
  // Process RFID reader 3 (D3)
  // -----------------------------
  digitalWrite(SS_PIN_1, HIGH);
  digitalWrite(SS_PIN_2, HIGH);
  digitalWrite(SS_PIN_3, HIGH);
  digitalWrite(SS_PIN_3, LOW);
  delay(5); // Allow chip select to settle

  if (currentMillis - lastReadRFID3 > debounceDelay) {
    if (rfid3.PICC_IsNewCardPresent() && rfid3.PICC_ReadCardSerial()) {
      String uid = readUID(rfid3);
      uid.toLowerCase();  // Convert UID to lowercase for consistency
      
      // Check for red card UIDs
      if (uid == "53135a10" || uid == "a35abb10" || uid == "d3f74310" ||
          uid == "c3c55510" || uid == "d3325d10" || uid == "7312d927" ||
          uid == "63873029" || uid == "a3b96d29" || uid == "436d2d10") {
        Serial.println("F1_RED");
      }
      // Check for green card UIDs
      else if (uid == "43864828" || uid == "93552512" || uid == "73463010" ||
               uid == "530e5c10" || uid == "73606210" || uid == "297ac54a" ||
               uid == "b3604510" || uid == "435e4410" || uid == "73281e2a" ||
               uid == "7348ace4" || uid == "b3276010") {
        Serial.println("F1_GREEN");
      }
      // Check for the RESTART UID
      else if (uid == "e3d168f7") {
        Serial.println("RESTART");
      }
      rfid3.PICC_HaltA();
      lastReadRFID3 = currentMillis; // Update timestamp for reader 3
    }
  }
  digitalWrite(SS_PIN_3, HIGH);
}
