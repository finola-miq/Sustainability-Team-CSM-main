
//RELAYS ON BOARD A and C
#define RELAY_A_R1 46 //Let's pretend it is Board A and name for relay = RELAY_A_R1 where A-name of the board and R1-Red LED number 1 - SHORT LEFT LED STRIP
#define RELAY_A_G1 48 //Let's pretend it is Board A and name for relay = RELAY_A_G1 where A-name of the board and G1-Green LED number 1 - SHORT LEFT LED STRIP
#define RELAY_A_R2 50 // SHORT RIGHT RED LED STRIP
#define RELAY_A_G2 52 // SHORT RIGHT GREEN LED STRIP
#define RELAY_C_G1 45
#define RELAY_C_R1 43
#define RELAY_C_G2 51 
#define RELAY_C_R2 53


//RELAYS ON BOARD D
#define RELAY_E_G1 47 //CLOSER TO THE EDGE
#define RELAY_E_R1 49 //CLOSER TO THE EDGE
#define RELAY_E_G2 41 //COSTCO CARD
#define RELAY_E_R2 39 //COSTCO CARD
// #define RELAY_F_G1 
// #define RELAY_F_R1

//INPUT PINS
#define RFID1_E_R 2 //RFID1_E_G means that rfid reader number 1 is located on board E and it manages green LED
#define RFID1_E_G 3 //RFID1_E_R means that rfid reader number 1 is located on board E and it manages red LED
#define RFID2_E_R 4
#define RFID2_E_G 5
#define RFID1_F_R 6
#define RFID1_F_G 7

#define RFID1_A_G 8
#define RFID1_A_R 9
#define RFID2_A_G 10
#define RFID2_A_R 11
#define RFID1_C_R 12
#define RFID1_C_G 13
#define RFID2_C_R 22
#define RFID2_C_G 24




void setup() 
{
  //start serial connection
  Serial.begin(9600);
  //configure pins as an input and enable the internal pull-up resistor
  
// INPUT PINS FOR BOARD D
  pinMode(RFID1_E_G, INPUT_PULLUP);
  pinMode(RFID1_E_R, INPUT_PULLUP);
  pinMode(RFID2_E_G, INPUT_PULLUP);
  pinMode(RFID2_E_R, INPUT_PULLUP);
  // pinMode(RFID3_F_G, INPUT_PULLUP);
  // pinMode(RFID3_F_R, INPUT_PULLUP);
// INPUT PINS FOR BOARD A and C
  pinMode(RFID1_A_G, INPUT_PULLUP);
  pinMode(RFID1_A_R, INPUT_PULLUP);
  pinMode(RFID2_A_G, INPUT_PULLUP);
  pinMode(RFID2_A_R, INPUT_PULLUP);
  pinMode(RFID1_C_G, INPUT_PULLUP);
  pinMode(RFID1_C_R, INPUT_PULLUP);
  pinMode(RFID2_C_G, INPUT_PULLUP);
  pinMode(RFID2_C_R, INPUT_PULLUP);

//OUTPINS FOR BOARD E and F
  pinMode(RELAY_E_G1, OUTPUT);
  pinMode(RELAY_E_R1, OUTPUT);
  pinMode(RELAY_E_G2, OUTPUT);
  pinMode(RELAY_E_R2, OUTPUT);
  // pinMode(RELAY_F_G1, OUTPUT);
  // pinMode(RELAY_F_R1, OUTPUT);

//OUTPINS FOR BOARD A and C
  pinMode(RELAY_A_R1, OUTPUT);
  pinMode(RELAY_A_G1, OUTPUT);
  pinMode(RELAY_A_R2, OUTPUT);
  pinMode(RELAY_A_G2, OUTPUT);
  pinMode(RELAY_C_G1, OUTPUT);
  pinMode(RELAY_C_R1, OUTPUT);
  pinMode(RELAY_C_G2, OUTPUT);
  pinMode(RELAY_C_R2, OUTPUT);


// --------TEST ALL RELAYS/LEDs ON-OFF--------
  digitalWrite(RELAY_A_R1, HIGH);
  delay(200);
  digitalWrite(RELAY_A_G1, HIGH);
  delay(600);
  digitalWrite(RELAY_A_R1, LOW);
  delay(200);
  digitalWrite(RELAY_A_G1, LOW);
  delay(200);
  digitalWrite(RELAY_A_R2, HIGH);
  delay(200);
  digitalWrite(RELAY_A_G2, HIGH);
  delay(200);
  digitalWrite(RELAY_A_R2, LOW);
  delay(200);
  digitalWrite(RELAY_A_G2, LOW);
  delay(500);

  digitalWrite(RELAY_E_G1, HIGH);
  delay(200);
  digitalWrite(RELAY_E_R1, HIGH);
  delay(200);
  digitalWrite(RELAY_E_G2, HIGH);
  delay(200);
  digitalWrite(RELAY_E_R2, HIGH);
  delay(200);
  digitalWrite(RELAY_C_G2, HIGH);
  delay(200);
  digitalWrite(RELAY_C_R2, HIGH);
  delay(200);
  digitalWrite(RELAY_C_G1, HIGH);
  delay(200);
  digitalWrite(RELAY_C_R1, HIGH);
  delay(200);

}

void loop() {

int RFID1_A_R_status = digitalRead(RFID1_A_R);
int RFID1_A_G_status = digitalRead(RFID1_A_G);
int RFID2_A_R_status = digitalRead(RFID2_A_R);
int RFID2_A_G_status = digitalRead(RFID2_A_G);
int RFID1_C_G_status = digitalRead(RFID1_C_G);
int RFID1_C_R_status = digitalRead(RFID1_C_R);
int RFID2_C_G_status = digitalRead(RFID2_C_G);
int RFID2_C_R_status = digitalRead(RFID2_C_R);

int RFID1_E_G_status = digitalRead(RFID1_E_G);
int RFID1_E_R_status = digitalRead(RFID1_E_R);
int RFID2_E_G_status = digitalRead(RFID2_E_G);
int RFID2_E_R_status = digitalRead(RFID2_E_R);
int RFID1_F_G_status = digitalRead(RFID1_F_G);
int RFID1_F_R_status = digitalRead(RFID1_F_R);

  //----RED ON RFID 1 BOARD E and F
  if (RFID1_E_R_status == HIGH)
    {
      Serial.print("RFID1_E_R pin: ");
      Serial.println(RFID1_E_R);
      digitalWrite(RELAY_E_R1, LOW);
    }
    else if (RFID1_E_R_status == LOW)
    {
          digitalWrite(RELAY_E_R1, HIGH); 
    } 
   //---------GREEN on RFID 1 BOARD E and F
  if (RFID1_E_G_status == HIGH)
  {
    Serial.print("RFID2_E_G pin: ");
    Serial.println(RFID1_E_G);
    digitalWrite(RELAY_E_G1, LOW);
  }
  else if (RFID1_E_G_status == LOW)
  {
    digitalWrite(RELAY_E_G1, HIGH); 
  } 

  //-----------------------------------------RED ON RFID 2 BOARD E and F
  if (RFID2_E_R_status == HIGH)
    {
      Serial.print("RFID2_E_R pin: ");
      Serial.println(RFID2_E_R);
      digitalWrite(RELAY_E_R2, LOW);
    }
    else if (RFID1_E_R_status == LOW)
    {
          digitalWrite(RELAY_E_R2, HIGH); 
    } 
   //----------------------------------------GREEN on RFID 2 BOARD E and F
  if (RFID2_E_G_status == HIGH)
  {
    Serial.print("RFID2_E_G pin: ");
    Serial.println(RFID2_E_G);
    digitalWrite(RELAY_E_G2, LOW);
  }
  else if (RFID2_E_G_status == LOW)
  {
    digitalWrite(RELAY_E_G2, HIGH); 
  } 

  //--------------RED ON RFID 1 BOARD A and C
  if (RFID1_A_G_status == HIGH)
  {
    Serial.print("RFID1_A_R pin 8: ");
    Serial.println(RFID1_A_R);
    digitalWrite(RELAY_A_R1, HIGH);
  }
   else if (RFID1_A_G_status == LOW)
  {
        digitalWrite(RELAY_A_R1, LOW); 
  } 
  //---------------GREEN on RFID 1 BOARD A and C
  if (RFID1_A_R_status == HIGH)
  {
    Serial.print("RFID1_A_G pin 9: ");
    Serial.println(RFID1_A_G);
    digitalWrite(RELAY_A_G1, HIGH);
  }
  else if (RFID1_A_R_status == LOW)
  {
    digitalWrite(RELAY_A_G1, LOW); 
  } 

//-------------------------------RED ON RFID 2 BOARD A and C
  if (RFID2_A_G_status == HIGH)
  {
    Serial.print("RFID2_A_R pin 10: ");
    Serial.println(RFID2_A_R);
    digitalWrite(RELAY_A_R2, HIGH);
  }
   else if (RFID2_A_G_status == LOW)
  {
        digitalWrite(RELAY_A_R2, LOW); 
  } 
  //-----------------------------GREEN on RFID 2 BOARD A and C
  if (RFID2_A_R_status == HIGH)
  {
    Serial.print("RFID2_A_G pin 11: ");
    Serial.println(RFID2_A_G);
    digitalWrite(RELAY_A_G2, HIGH);
  }
  else if (RFID2_A_R_status == LOW)
  {
    digitalWrite(RELAY_A_G2, LOW); 
  } 

//--------------------------------------------------RED ON RFID 3 BOARD A and C
  if (RFID1_C_R_status == HIGH)
  {
    Serial.print("RFID1_C_R pin 12: ");
    Serial.println(RFID1_C_R);
    digitalWrite(RELAY_C_R1, LOW);
  }
   else if (RFID1_C_R_status == LOW)
  {
        digitalWrite(RELAY_C_R1, HIGH); 
  } 
  //------------------------------------------- ----GREEN on RFID 3 BOARD A and C
  if (RFID1_C_G_status == HIGH)
  {
    Serial.print("RFID1_C_G pin 13: ");
    Serial.println(RFID1_C_G);
    digitalWrite(RELAY_C_G1, LOW);
  }
  else if (RFID1_C_G_status == LOW)
  {
    digitalWrite(RELAY_C_G1, HIGH); 
  } 
//------------------------------------------------------------------RED ON RFID 4 BOARD A and C - check why voltage on PIN A6 and A7
  // if (RFID2_C_R_status == HIGH)
  // {
  //   Serial.print("RFID2_C_R pin: ");
  //   Serial.println(RFID2_C_R);
  //   digitalWrite(RELAY_C_R2, LOW);
  // }
  //  else if (RFID2_C_R_status == LOW)
  // {
  //       digitalWrite(RELAY_C_R2, HIGH); 
  // } 
  // //-----------------------------------------------------------------GREEN on RFID 4 BOARD A and C
  // if (RFID2_C_G_status == HIGH)
  // {
  //   Serial.print("RFID2_C_G pin: ");
  //   Serial.println(RFID2_C_G);
  //   digitalWrite(RELAY_C_G2, LOW);
  // }
  // else if (RFID2_C_G_status == LOW)
  // {
  //   digitalWrite(RELAY_C_G2, HIGH); 
  // } 


}
