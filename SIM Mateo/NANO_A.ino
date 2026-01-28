// NANO A - Using hardware Serial on TX1 (TX) and RX0 (RX)
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN_1 9
#define SS_PIN_1 2
#define SS_PIN_2 3

MFRC522 rfid1(SS_PIN_1, RST_PIN_1);
MFRC522 rfid2(SS_PIN_2, RST_PIN_1);

unsigned long lastReadRFID1 = 0;
unsigned long lastReadRFID2 = 0;
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
  rfid1.PCD_Init();
  Serial.println("RFID A1 connected");
  rfid2.PCD_Init();
  Serial.println("RFID A2 connected");
}

void loop() {
  unsigned long currentMillis = millis();

  // Process RFID reader 1
  if (rfid1.PICC_IsNewCardPresent() && rfid1.PICC_ReadCardSerial()) {
    // Check if debounce interval has passed
    if (currentMillis - lastReadRFID1 > debounceDelay) {
      String uid = readUID(rfid1);
      uid.toLowerCase();

      if (uid == "53135a10" || uid == "a35abb10" || uid == "d3f74310" ||
          uid == "c3c55510" || uid == "d3325d10" || uid == "7312d927" ||
          uid == "63873029" || uid == "a3b96d29" || uid == "436d2d10") {
        Serial.println("A1_RED");
      }
      else if (uid == "43864828" || uid == "93552512" || uid == "73463010" ||
               uid == "530e5c10" || uid == "73606210" || uid == "297ac54a" ||
               uid == "b3604510" || uid == "435e4410" || uid == "73281e2a" ||
               uid == "7348ace4" || uid == "b3276010") {
        Serial.println("A1_GREEN");
      }
      else if (uid == "e3d168f7") {
        Serial.println("RESTART");
      }
      // Update the last read time for RFID1
      lastReadRFID1 = currentMillis;
    }
    rfid1.PICC_HaltA(); 
  }

  // Process RFID reader 2
  if (rfid2.PICC_IsNewCardPresent() && rfid2.PICC_ReadCardSerial()) {
    if (currentMillis - lastReadRFID2 > debounceDelay) {
      String uid = readUID(rfid2);
      uid.toLowerCase();

      if (uid == "53135a10" || uid == "a35abb10" || uid == "d3f74310" ||
          uid == "c3c55510" || uid == "d3325d10" || uid == "7312d927" ||
          uid == "63873029" || uid == "a3b96d29" || uid == "436d2d10") {
        Serial.println("A2_RED");
      }
      else if (uid == "43864828" || uid == "93552512" || uid == "73463010" ||
               uid == "530e5c10" || uid == "73606210" || uid == "297ac54a" ||
               uid == "b3604510" || uid == "435e4410" || uid == "73281e2a" ||
               uid == "7348ace4" || uid == "b3276010") {
        Serial.println("A2_GREEN");
      }
      else if (uid == "e3d168f7"){
        Serial.println("RESTART");
      }
      lastReadRFID2 = currentMillis;
    }
    rfid2.PICC_HaltA();
  }
}
