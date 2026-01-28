#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9   // Configurable, see typical pin layout above
#define SS_1_PIN 2  // Configurable, take a unused pin, only HIGH/LOW required, must be different to SS 2
#define SS_2_PIN 3  // Configurable, take a unused pin, only HIGH/LOW required, must be different to SS 1

#define NR_OF_READERS 2
#define NR_OF_CARDS 2
#define NR_OF_RELAYS 2

//-------------------!!!!!!!!!!-----------------------
int RELAY_RED1 = A0;
int RELAY_GREEN1 = A1;
#define REDRELAY 0
#define GREENRELAY 1
int Reader1Relays[] = { RELAY_RED1, RELAY_GREEN1 };
int RELAY_RED2 = A2;
int RELAY_GREEN2 = A3;
int Reader2Relays[] = { RELAY_RED2, RELAY_GREEN2 };
int* ReaderRelays[] = { Reader1Relays, Reader2Relays };

const byte GREEN_CARD1[] = { 99, 90, 150, 13 };
const byte GREEN_CARD8[] = { 179, 96, 69, 16 };
const byte RED_CARD2[] = { 115, 181, 132, 13 };
const byte RED_CARD7[] = { 195, 197, 85, 16 };

byte* greenCards[] = { GREEN_CARD1, GREEN_CARD8 };
byte* redCards[] = { RED_CARD2, RED_CARD7 };

byte ssPins[] = { SS_1_PIN, SS_2_PIN };

MFRC522 mfrc522[NR_OF_READERS];  // Create MFRC522 instance.
bool cardPresent[NR_OF_READERS];

/**
 * Initialize.
 */
void setup() {

  Serial.begin(9600);  // Initialize serial communications with the PC
  while (!Serial)
    ;  // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  SPI.begin();  // Init SPI bus

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN);  // Init each MFRC522 card
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();

    cardPresent[reader] = false;
  }

  for (int i = 0; i < NR_OF_READERS; i++)
    for (int j = 0; j < NR_OF_RELAYS; j++) {
      pinMode(ReaderRelays[i][j], OUTPUT);
    }
}

int count = 0;

/**
 * Main loop.
 */
void loop() {

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    // Look for new cards

    // if (PICC_IsAnyCardPresent(mfrc522[reader])) {
    //   Serial.print(F("card present on RFID: "));
    //   Serial.print(reader);
    //   Serial.print(F(" #"));
    //   Serial.println(count++);
    //   Serial.print(F("present: "));
    //   Serial.println(cardPresent[reader]);
    // }
    //delay(100);

    if (PICC_IsAnyCardPresent(mfrc522[reader]) && mfrc522[reader].PICC_ReadCardSerial()) {

      Serial.print(F("card present on RFID: "));
      Serial.print(reader);
      Serial.print(F(" #"));
      Serial.println(count++);
      Serial.print(F("present: "));
      Serial.println(cardPresent[reader]);

      if (cardPresent[reader])
        continue;

      cardPresent[reader] = true;

      Serial.print(F("Reader "));
      Serial.print(reader);
      // Show some details of the PICC (that is: the tag/card)
      Serial.print(F(": Card UID:"));
      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
      Serial.println();
      Serial.print(F("PICC type: "));
      MFRC522::PICC_Type piccType = mfrc522[reader].PICC_GetType(mfrc522[reader].uid.sak);
      Serial.println(mfrc522[reader].PICC_GetTypeName(piccType));

      Serial.print("CheckCards");
      if (CheckCards(mfrc522[reader].uid.uidByte, greenCards)) {
        Serial.println("GREEN");
        int releayPin = ReaderRelays[reader][GREENRELAY];
        digitalWrite(releayPin, HIGH);
        //digitalWrite(RELAY1, HIGH);
      } else if (CheckCards(mfrc522[reader].uid.uidByte, redCards)) {
        Serial.println("RED");
                int releayPin = ReaderRelays[reader][REDRELAY];
        digitalWrite(releayPin, HIGH);
        //digitalWrite(RELAY1, HIGH);
      }

      // Halt PICC
      mfrc522[reader].PICC_HaltA();
      // Stop encryption on PCD
      mfrc522[reader].PCD_StopCrypto1();
    }  //if (mfrc522[reader].PICC_IsNewC
    else {
      cardPresent[reader] = false;
      Serial.print(F("present: "));
      Serial.println(cardPresent[reader]);

      int releayPin = ReaderRelays[reader][GREENRELAY];
      digitalWrite(releayPin, LOW);
      releayPin = ReaderRelays[reader][REDRELAY];
      digitalWrite(releayPin, LOW);
    }

  }  //for(uint8_t reader
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte* buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

bool PICC_IsAnyCardPresent(MFRC522 rfid) {
  byte bufferATQA[2];
  byte bufferSize = sizeof(bufferATQA);

  // Reset baud rates
  rfid.PCD_WriteRegister(rfid.TxModeReg, 0x00);
  rfid.PCD_WriteRegister(rfid.RxModeReg, 0x00);
  // Reset ModWidthReg
  rfid.PCD_WriteRegister(rfid.ModWidthReg, 0x26);

  MFRC522::StatusCode result = rfid.PICC_WakeupA(bufferATQA, &bufferSize);
  return (result == MFRC522::STATUS_OK || result == MFRC522::STATUS_COLLISION);
}  // End PICC_IsAnyCardPresent()

bool CheckCardUID(byte* buffer, byte* cardUID) {
  for (byte i = 0; i < 4; i++) {
    Serial.print(F("buf:"));
    Serial.print(buffer[i]);
    Serial.print(F(" Card ID:"));
    Serial.println(cardUID[i]);
    if (buffer[i] != cardUID[i]) {
      return false;
    }
  }
  Serial.println(F("Card Match."));
  return true;
}

bool CheckCards(byte* buffer, byte** cards) {
  for (int i = 0; i < NR_OF_CARDS; i++) {
    if (CheckCardUID(buffer, cards[i]))
      return true;
  }
  return false;
}
