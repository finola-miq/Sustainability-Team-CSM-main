// BOARD E & F
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN1 2
#define SS_PIN2 3
#define SS_PIN3 4
#define RST_PIN 9

#define NR_OF_READERS 3
#define NR_OF_CARDS 10
#define CARD_OFF_RETRY_MAX 2
#define NR_OF_RELAYS 7

enum RFState {
  eCardOff = 0,
  eCardOn
};

enum CardType {
  e_InvalidCard = 0,
  e_GreenCard,
  e_RedCard
};

byte ssPins[] = { SS_PIN1, SS_PIN2, SS_PIN3 };
MFRC522 mfrc522[NR_OF_READERS];
bool lockedCard[NR_OF_READERS];
RFState rfReaderState[NR_OF_READERS];
int rfReaderCardOffRetries[NR_OF_READERS];

const byte GREEN_CARD1[] = { 99, 90, 150, 13 };
const byte GREEN_CARD3[] = { 227, 209, 104, 247};
const byte GREEN_CARD5[] = { 227, 102, 184, 13};
const byte GREEN_CARD6[] = { 67, 94, 68, 16};
const byte GREEN_CARD7[] = { 41, 122, 197, 74};
const byte GREEN_CARD8[] = { 179, 96, 69, 16 };
const byte RED_CARD2[] = { 115, 181, 132, 13 };
const byte RED_CARD6[] = { 211, 50, 93, 16 };
const byte RED_CARD7[] = { 195, 197, 85, 16 };
const byte RED_CARD8[] = { 211, 247, 67, 16 };




byte* greenCards[] = { GREEN_CARD1, GREEN_CARD3, GREEN_CARD5, GREEN_CARD6, GREEN_CARD7, GREEN_CARD8 };
byte* redCards[] = { RED_CARD2, RED_CARD6, RED_CARD7, RED_CARD8 };

const int RF0_RELAY_RED = A0;    // A0 = PIN_A0; #define PIN_A0   (14)
const int RF0_RELAY_GREEN = A1;  // A1 = PIN_A1; #define PIN_A1   (15)
const int RF1_RELAY_RED = A2;    // A2 = PIN_A2; #define PIN_A2   (16)
const int RF1_RELAY_GREEN = A3;  // A3 = PIN_A3; #define PIN_A3   (17)
const int RF2_RELAY_RED = A4;
const int RF2_RELAY_GREEN = A5;


#define REDRELAY 0
#define GREENRELAY 1
int Reader0Relays[] = { RF0_RELAY_RED, RF0_RELAY_GREEN };
int Reader1Relays[] = { RF1_RELAY_RED, RF1_RELAY_GREEN };
int Reader2Relays[] = { RF2_RELAY_RED, RF2_RELAY_GREEN };
int* ReadersRelays[] = { Reader0Relays, Reader1Relays, Reader2Relays };

void setup() {
  Serial.begin(9600);
  SPI.begin();  // Init SPI bus

  Serial.println(F("Clean City demo code... Copyright protected"));

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN);  // Init each MFRC522 card
    mfrc522[reader].uid.size = 0;                       // Clear the information stored about locked cards.
    delay(4);

    Serial.print(F("Initialized RFReader "));
    Serial.println(reader);

    lockedCard[reader] = false;
    rfReaderState[reader] = eCardOff;
    rfReaderCardOffRetries[reader] = 0;
  }

  InitOutputPins();
}

void loop() {

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    MFRC522& rfid = mfrc522[reader];
    bool& locked = lockedCard[reader];
    RFState& rfState = rfReaderState[reader];
    int& cardOffRetries = rfReaderCardOffRetries[reader];

    // Wake up all cards present within the sensor/reader range.
    bool cardPresent = PICC_IsAnyCardPresent(rfid);
    // Serial.print(F("cardPresent: reader#"));
    // Serial.print(reader);
    // Serial.print(F(": "));
    // Serial.println(cardPresent);
    delay(10);

    bool readSerial = false;
    if (cardPresent) {
      int iter = 1;
      for (iter = 1; iter <= 5; iter++) {
        readSerial = rfid.PICC_ReadCardSerial();
        if (readSerial)
          break;

        delay(100);
      }
    }

    switch (rfState) {
      case eCardOff:
        if (cardPresent && readSerial) {
          rfState = eCardOn;

          Serial.println(F("**********"));
          Serial.print(F("Card present on reader #"));
          Serial.println(reader);
          DisplayCardUID(rfid);
        }

        break;
      case eCardOn:
        if (cardPresent && !locked) {
          locked = true;
          Serial.println(F("^^^^^^^^^^"));
          Serial.print(F("Card locked on reader #"));
          Serial.println(reader);

          CardType cardType = ValidateCardPresent(rfid);
          switch (cardType) {
            case e_GreenCard:
              ActivateGreenRelay(reader);
              break;
            case e_RedCard:
              ActivateRedRelay(reader);
              break;
            case e_InvalidCard:
              ProcessInvalidCard();
              break;
          }
        } else {
          cardOffRetries++;
          if (cardOffRetries > CARD_OFF_RETRY_MAX) {
            cardOffRetries = 0;
            rfState = eCardOff;
            locked = false;
            Serial.println(F("**********"));
            Serial.print(F("Card removed on reader #"));
            Serial.println(reader);

            DeactivateRelays(reader);

            rfid.uid.size = 0;
          }
        }
        if (cardPresent)
          cardOffRetries = 0;
        break;
    }

    rfid.PICC_HaltA();
  }
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void printHex(byte* buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(((buffer[i]) >> 4) & 0x0F, HEX);
    Serial.print(buffer[i] & 0x0F, HEX);
    Serial.print(" ");
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte* buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(' ');
    Serial.print(buffer[i], DEC);
  }
}
// This convenience function could be added to the library in the future

/**
 * Returns true if a PICC responds to PICC_CMD_WUPA.
 * All cards in state IDLE or HALT are invited.
 *
 * @return bool
 */
bool PICC_IsAnyCardPresent(MFRC522& rfid) {
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

void DisplayCardUID(MFRC522& rfid) {
  Serial.println(F("The NUID tag is:"));
  Serial.print(F("In hex: "));
  printHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();
  Serial.print(F("In dec: "));
  printDec(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();
}

CardType ValidateCardPresent(MFRC522 rfid) {
  CardType cardType = e_InvalidCard;

  if (CheckCards(rfid.uid.uidByte, greenCards)) {
    Serial.println("GREEN");
    cardType = e_GreenCard;
  } else if (CheckCards(rfid.uid.uidByte, redCards)) {
    Serial.println("RED");
    cardType = e_RedCard;
  }

  return cardType;
}

bool CheckCardUID(byte* buffer, byte* cardUID) {
  for (byte i = 0; i < 4; i++) {
    // Serial.print(F("buf:"));
    // Serial.print(buffer[i]);
    // Serial.print(F(" Card ID:"));
    // Serial.println(cardUID[i]);
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

void ActivateGreenRelay(uint8_t reader) {
  Serial.println(F("ActivateGreen"));
  int releayPin = ReadersRelays[reader][GREENRELAY];
  digitalWrite(releayPin, HIGH);
  Serial.print(F("Set to HIGH Green Relay pin: "));
  Serial.println(releayPin);
}

void ActivateRedRelay(uint8_t reader) {
  Serial.println(F("ActivateRed"));
  int releayPin = ReadersRelays[reader][REDRELAY];
  digitalWrite(releayPin, HIGH);
  Serial.print(F("Set to HIGH Red Relay pin: "));
  Serial.println(releayPin);
}

void ProcessInvalidCard() {
  Serial.println(F("InvalidCard"));
}

void DeactivateRelays(uint8_t reader) {
  Serial.println(F("Lights Off"));

  int releayPinGreen = ReadersRelays[reader][GREENRELAY];
  digitalWrite(releayPinGreen, LOW);
  Serial.print(F("Set to LOW Green Relay pin: "));
  Serial.println(releayPinGreen);

  int releayPinRed = ReadersRelays[reader][REDRELAY];
  digitalWrite(releayPinRed, LOW);
  Serial.print(F("Set to LOW Red Relay pin: "));
  Serial.println(releayPinRed);
}

void InitOutputPins() {
  for (int i = 0; i < NR_OF_READERS; i++) {
    Serial.print(F("Reader #"));
    Serial.println(i);

    for (int j = 0; j < NR_OF_RELAYS; j++) {
      int pin = ReadersRelays[i][j];
      if (j == REDRELAY) {
        Serial.print(F("Red Relay output pin#"));
        Serial.println(pin);
      } else {
        Serial.print(F("Green Relay output pin#"));
        Serial.println(pin);
      }
      pinMode(ReadersRelays[i][j], OUTPUT);
    }
  }
}
