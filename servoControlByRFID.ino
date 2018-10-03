#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include <Servo.h>
 
#define RST_PIN 9
#define SS_PIN 10
#define chair 6
 
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
Servo myServo;
 
String card1 = "F3 BD CD 8B"; //임산부입니다.
String card2 = "99 2E CA 55"; //임산부가 아닙니다.
String currentTagId = "";
boolean state = false;
 
void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(chair, OUTPUT);
  myServo.attach(6);
  myServo.write(0);
  
  Serial.println("Please Tagging the Reader");
  Serial.println("--------------------------");
  Serial.println();
}
 
void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    currentTagId += mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ";
    currentTagId += String(mfrc522.uid.uidByte[i], HEX);
  }

  currentTagId.trim();
  currentTagId.toUpperCase();
 
  Serial.print("Card 1: ");
  Serial.println(card1);
  Serial.print("Card 2: ");
  Serial.println(card2);
  Serial.print("Check : ");
  Serial.println(currentTagId);

  
  Serial.println();

  if (currentTagId == card1) {
    // Serial.println(state); //임산부 의자 개폐 확인용
    if (state == false) {
      myServo.write(90);
      Serial.println("Pragnent Card");
      state = true;
      currentTagId = "";
      delay(500);
    } else {
      myServo.write(0);
      Serial.println("Close the Chair");
      state = false;
      currentTagId = "";
      delay(500);
    }
  }
  if (currentTagId == card2) {
    myServo.write(0);
    Serial.println("No Pragnent Card");
    state = false;
    currentTagId = "";
    delay(500);
  }
  Serial.println();
  delay(200);
 
}
 
