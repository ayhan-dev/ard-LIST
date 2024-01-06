
#include <SPI.h>
#include <MFRC522.h>
#include <avr/wdt.h>


#define SS_PIN 5
#define RST_PIN 9
#define RELAY 3
#define BUZZER 2 
#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000

MFRC522 mfrc522(SS_PIN, RST_PIN);   


void setup() {
  Serial.begin(9600);  
  SPI.begin();          
  mfrc522.PCD_Init();   

  
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  digitalWrite(RELAY, HIGH);
  Serial.println("Put your card to the reader for scanning");
  Serial.println();
}


void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  
 
  if (!mfrc522.PICC_ReadCardSerial()){
    return;
  }


  Serial.print("UID :");
  String content= "";
  byte letter;
  
  for (byte i = 0; i < mfrc522.uid.size; i++) {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  
  if (content.substring(1) == "E2 2F 3C 1B"){
    
    wdt_enable(WDTO_2S); 
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    digitalWrite(RELAY, LOW);
    delay(ACCESS_DELAY);
    digitalWrite(RELAY, HIGH);
    wdt_reset();
    wdt_disable();
  }
  
          else if (content.substring(1) == "64 34 565 4454") {
           wdt_enable(WDTO_2S);
          Serial.println("Authorized access");
          Serial.println();
          delay(500);
          digitalWrite(RELAY, LOW);
          delay(ACCESS_DELAY);
          digitalWrite(RELAY, HIGH);
          wdt_reset();
          wdt_disable();
          } else {
   wdt_enable(WDTO_2S);
    Serial.println(" Access denied");
    tone(BUZZER, 300);
    delay(DENIED_DELAY);
    noTone(BUZZER);
       
           wdt_reset();
          wdt_disable();
     }
}
