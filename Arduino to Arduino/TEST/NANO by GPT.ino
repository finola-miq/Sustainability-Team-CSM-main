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
const byte RED_CARD[4] = {115, 181, 132, 13};

void setup() {
	Serial.begin(9600);		          // Initialize serial communications with the PC
	while (!Serial);		            // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			              // Init SPI bus
	
    for (uint8_t reader = 0; reader < NR_OF_READERS; reader++)
    {
    rfid[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print(F("RFID Reader # "));
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
    //---------------------OLEG'S CODE-------------------
//   bool cardPresent = PICC_IsAnyCardPresent(rfid);
  
//   if (! locked && ! cardPresent)
//     return;

//   MFRC522::StatusCode result = rfid.PICC_Select(&rfid.uid,8*rfid.uid.size);

//   if(!locked && result == MFRC522::STATUS_OK)
//   {
//     locked=true;
//     // Action on card detection.
//     Serial.print(F("locked! NUID tag: "));
//     printHex(rfid.uid.uidByte, rfid.uid.size);
//     Serial.println();
//------------------------CHAT GPT CODE-------------------
for (int i = 0; i < NR_OF_READERS; i++) 
{
    if (PICC_IsAnyCardPresent(rfid[i])) 
    {
    MFRC522::StatusCode result = rfid[i].PICC_Select(&rfid[i].uid);
      if (result == MFRC522::STATUS_OK) 
      {
        Serial.print(F("Card detected! NUID tag: "));
        printHex(rfid[i].uid.uidByte, rfid[i].uid.size);
        Serial.println();
//----------------------OLEG'S CODE------------------------
//       if (CheckCardUID(rfid.uid.uidByte, GREEN_CARD)) {
//       digitalWrite(GREEN_LED, HIGH);
//     } else if (CheckCardUID(rfid.uid.uidByte, RED_CARD)) {
//       digitalWrite(RED_LED, HIGH);
//     } else {
//       digitalWrite(GREEN_LED, HIGH);
//       digitalWrite(RED_LED, HIGH);
//     }
//   } else if(locked && result != MFRC522::STATUS_OK)
//   {
//     locked=false;
//     rfid.uid.size = 0;
//     // Action on card removal.
//     digitalWrite(GREEN_LED, LOW);
//     digitalWrite(RED_LED, LOW);
//--------------------CHAT GPT CODE--------------------------
        if (i == 0) 
         { // RFID 1
         if (checkCardUID(rfid[i].uid.uidByte, RED_CARD)) {
         digitalWrite(RED_LED_PIN, HIGH);
         } 
         else 
         {
         digitalWrite(RED_LED_PIN, LOW);
         }
         } 
         else if (i == 1) { // RFID 2
         if (checkCardUID(rfid[i].uid.uidByte, GREEN_CARD)) 
         {
         digitalWrite(GREEN_LED_PIN, HIGH);
         } 
         else 
         {
         digitalWrite(GREEN_LED_PIN, LOW);
         }
         if (i == 3)
         {
          // RFID 3
         if (checkCardUID(rfid[i].uid.uidByte, RED_CARD)) {
         digitalWrite(RED_LED_PIN, HIGH);
         } 
         else 
         {
         digitalWrite(RED_LED_PIN, LOW);  
         }
                }
            }
            rfid[i].PICC_HaltA(); // Halt PICC
        }
    }
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
