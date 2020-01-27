#include <SPI.h>
#include <MFRC522.h> // библиотека "RFID".
#define SS_PIN 10
#define RST_PIN 9

#define DEBUG 

#ifdef DEBUG
  #define d(x) Serial.print(#x); Serial.print(" = "); Serial.println(x, DEC);
  #define m(x) Serial.println(x);
#else
  #define d(x) ;
  #define m(x) ;
#endif

#define KEYS_SIZE 2

#define TIMER_PIN 300
#define TIMER_CARD 10000
unsigned long timerStart;

struct Key{
  int room;
  bool isKey;
  byte pin; 
};

Key keys[KEYS_SIZE];

MFRC522 mfrc522(SS_PIN, RST_PIN);
unsigned long uidDec, uidDecTemp;  // для храниения номера метки в десятичном формате

void setup() {
  Serial.begin(9600);
  int i = 0;
  #define addKey(room1, pin1) keys[i].room = room1; keys[i].pin = pin1; i++;
  addKey(108, 8)
  addKey(107, 7)

  for (auto &i : keys){
    i.isKey = 1;
    pinMode(i.pin, INPUT);
  }
  Serial.println("Waiting for card...");
  SPI.begin();  //  инициализация SPI
  mfrc522.PCD_Init();     // инициализация MFRC522
}

bool isCard = 0;

void alert(){
  m("ALERT!!!")
}

void scanCard(){
  if (isCard == false && mfrc522.PICC_IsNewCardPresent()){
    if (mfrc522.PICC_ReadCardSerial()) {
      m("карта считана")
      isCard = true;
      timerStart = millis();

      // Выдача серийного номера метки.
      for (byte i = 0; i < mfrc522.uid.size; i++)
      {
        uidDecTemp = mfrc522.uid.uidByte[i];
        uidDec = uidDec * 256 + uidDecTemp;
      }
      m("card UID: ")
      m(uidDec)
    }
  }
}

void resetCard(){
 isCard = false;
 uidDec = 0;
}

void loop(){
  m(" ")
  
  if (isCard == true && timerStart + TIMER_CARD < millis()){
    isCard = false;
    uidDec = 0;
  }
  
  scanCard();
  d(isCard)
  d(uidDec)
  delay(600);
  
  for (int i = 0; i < KEYS_SIZE; i++){
    byte pinValue;
    pinValue = digitalRead(keys[i].pin);
    if(pinValue != keys[i].isKey){
      delay(TIMER_PIN);
      pinValue = digitalRead(keys[i].pin);
    }
    
    if(isCard == false && pinValue == LOW && keys[i].isKey == true){
      while (pinValue == LOW){
        pinValue = digitalRead(keys[i].pin);
        if(pinValue == HIGH){
            delay(TIMER_PIN);
            pinValue = digitalRead(keys[i].pin);
        }
        scanCard();
        if (isCard == true) return;
        alert();
      }
      resetCard();
    }
    else if (isCard == true && keys[i].isKey == true && pinValue == LOW){
      Serial.println("Key get out");
      Serial.println(keys[i].room);
      keys[i].isKey = false;
      resetCard();
    }
    if (keys[i].isKey == false && pinValue == HIGH){
      Serial.println("Key get up");
      Serial.println(keys[i].room);
      keys[i].isKey = true;
    }
  }
}
