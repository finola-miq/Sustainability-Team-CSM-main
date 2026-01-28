#include <SPI.h>
#include <MFRC522.h>

#define RELAY1   A0
#define RELAY2   A1
#define RELAY3   A2
#define RELAY4   A3

#define RST_PIN  9          
#define SS_PIN1  2
#define SS_PIN2  3
#define SS_PIN3  4
#define SS_PIN4  5      

#define NR_OF_READERS 4

byte ssPins[] = {SS_PIN1, SS_PIN2, SS_PIN3, SS_PIN4};

MFRC522 rfid[NR_OF_READERS];   // Create MFRC522 instance.

const byte GREEN_CARD1[4] = {99, 90, 150, 13};
const byte RED_CARD1[4] = {115, 181, 132, 13};
const byte GREEN_CARD2[4] = {179, 96, 69, 16};
const byte RED_CARD2[4] = {211, 50, 93, 16};



void setup() {
  Serial.begin(9600);
  while (!Serial);
  SPI.begin();
  
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    rfid[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    rfid[reader].PCD_DumpVersionToSerial();
  }
  

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
}

bool locked = false;

void loop() {
  for (int i = 0; i < 4; i++) {
    if (!locked && PICC_IsAnyCardPresent(rfid[i])) {
      if (rfid[i].PICC_ReadCardSerial()) {
        locked = true;
        Serial.print(F("locked! NUID tag: "));
        printHex(rfid[i].uid.uidByte, rfid[i].uid.size);
        Serial.println();

        if (CheckCardUID(rfid[0].uid.uidByte, GREEN_CARD)) {
          Serial.println("RFID1 GREEN");
          digitalWrite(RELAY1, HIGH);
        } else if (CheckCardUID(rfid[0].uid.uidByte, RED_CARD)) {
          Serial.println("RFID1 RED");
          digitalWrite(RELAY2, HIGH);
        } else {
          digitalWrite(RELAY1, LOW);
          digitalWrite(RELAY2, LOW);
        }

        if (CheckCardUID(rfid[1].uid.uidByte, GREEN_CARD)) {
          digitalWrite(RELAY3, HIGH);
          Serial.println("RFID2 GREEN");
        } else if (CheckCardUID(rfid[1].uid.uidByte, RED_CARD)) {
          digitalWrite(RELAY4, HIGH);
          Serial.println("RFID2 RED");
        } else {
          digitalWrite(RELAY3, LOW);
          digitalWrite(RELAY4, LOW);
        }

        rfid[i].PICC_HaltA();
        rfid[i].PCD_StopCrypto1();
        break; // Exit loop after detecting a card
      }
    }
  }

  if (locked) {
    delay(1000); // Delay to simulate locked state
    locked = false;
    digitalWrite(RELAY1, LOW);
    digitalWrite(RELAY2, LOW);
    digitalWrite(RELAY3, LOW);
    digitalWrite(RELAY4, LOW);
    Serial.println(F("unlocked!"));
  }
}

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(((buffer[i]) >> 4) & 0x0F, HEX);
    Serial.print(buffer[i] & 0x0F, HEX);
    Serial.print(" ");
  }
}

bool CheckCardUID(byte *buffer, byte* cardUID) {
  for (byte i = 0; i < 4; i++) {
    if (buffer[i] != cardUID[i]) {
      return false;
    }
  }
  Serial.println(F("Card Match."));
  return true;
}

bool PICC_IsAnyCardPresent(MFRC522 &rfid) {
  byte bufferATQA[2];
  byte bufferSize = sizeof(bufferATQA);
  
  rfid.PCD_WriteRegister(rfid.TxModeReg, 0x00);
  rfid.PCD_WriteRegister(rfid.RxModeReg, 0x00);
  rfid.PCD_WriteRegister(rfid.ModWidthReg, 0x26);
  
  MFRC522::StatusCode result = rfid.PICC_WakeupA(bufferATQA, &bufferSize);
  return (result == MFRC522::STATUS_OK || result == MFRC522::STATUS_COLLISION);
}
