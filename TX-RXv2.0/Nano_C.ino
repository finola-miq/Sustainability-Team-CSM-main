// NANO C

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN_1 9
#define SS_PIN_1 2
#define SS_PIN_2 3


MFRC522 rfid1(SS_PIN_1, RST_PIN_1);
MFRC522 rfid2(SS_PIN_2, RST_PIN_1);


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
  rfid2.PCD_Init();

}

void loop() {
  if (rfid1.PICC_IsNewCardPresent() && rfid1.PICC_ReadCardSerial()) {
    String uid = readUID(rfid1);
    if (uid == "c3c55510" || uid == "d3325d10" || uid == "d3f74310") {
      Serial.println("C1_RED");
    } else if (uid == "435e4410" || uid == "b3604510" || uid == "73606210") {
      Serial.println("C1_GREEN");
    }
    rfid1.PICC_HaltA();
  }

  if (rfid2.PICC_IsNewCardPresent() && rfid2.PICC_ReadCardSerial()) {
    String uid = readUID(rfid2);
    if (uid == "c3c55510" || uid == "d3325d10" || uid == "d3f74310") {
      Serial.println("C2_RED");
    } else if (uid == "435e4410" || uid == "b3604510" || uid == "73606210") {
      Serial.println("C2_GREEN");
    }
    rfid2.PICC_HaltA();
  }


}
