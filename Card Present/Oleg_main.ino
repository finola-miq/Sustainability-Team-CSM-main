#include <SPI.h>
#include <MFRC522.h>

#define RED_LED   3
#define GREEN_LED 4

#define RST_PIN         9          
#define SS_PIN          10      

MFRC522 rfid(SS_PIN, RST_PIN); 

const byte GREEN_CARD[4] = {211, 50, 93, 16};
const byte RED_CARD[4] = {179, 39, 96, 16};

void setup() {
	Serial.begin(9600);		          // Initialize serial communications with the PC
	while (!Serial);		            // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			              // Init SPI bus
	rfid.PCD_Init();		            // Init MFRC522
	delay(4);				                // Optional delay. Some board do need more time after init to be ready, see Readme
	rfid.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
}

bool locked = false;


void loop() {
  bool cardPresent = PICC_IsAnyCardPresent(rfid);
  
  if (! locked && ! cardPresent)
    return;

  MFRC522::StatusCode result = rfid.PICC_Select(&rfid.uid,8*rfid.uid.size);

  if(!locked && result == MFRC522::STATUS_OK)
  {
    locked=true;
    // Action on card detection.
    Serial.print(F("locked! NUID tag: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();

    if (CheckCardUID(rfid.uid.uidByte, GREEN_CARD)) {
      digitalWrite(GREEN_LED, HIGH);
    } else if (CheckCardUID(rfid.uid.uidByte, RED_CARD)) {
      digitalWrite(RED_LED, HIGH);
    } else {
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, HIGH);
    }
  } else if(locked && result != MFRC522::STATUS_OK)
  {
    locked=false;
    rfid.uid.size = 0;
    // Action on card removal.
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);

    Serial.print(F("unlocked! Reason for unlocking: "));
    Serial.println(rfid.GetStatusCodeName(result));
  } else if(!locked && result != MFRC522::STATUS_OK)
  {
    rfid.uid.size = 0;
  }
  rfid.PICC_HaltA();
}

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(((buffer[i])>>4)&0x0F,  HEX);
    Serial.print(buffer[i]&0x0F, HEX);
    Serial.print(" ");
  }
}

bool CheckCardUID(byte *buffer, byte* cardUID) {
  bool match = false;
   if (buffer[0] == cardUID[0] &&
    buffer[1] == cardUID[1] &&
    buffer[2] == cardUID[2] &&
    buffer[3] == cardUID[3] )
    {
      Serial.println(F("Card Match."));
      return true;
    }

    return false;
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
} // End PICC_IsAnyCardPresent()
