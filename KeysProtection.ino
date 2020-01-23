
#define KEYS_SIZE 1
#define PIN_CARD 7

struct Key{
  int room;
  bool isKey;
  byte pin; 
};

Key keys[KEYS_SIZE];

void setup() {
  Serial.begin(9600);
  keys[0].room = 100;
  keys[0].pin = 8;


  for (auto &i : keys){
    i.isKey = 1;
    pinMode(i.pin, INPUT);
  }
  pinMode(PIN_CARD, INPUT);
}

#define d(x) Serial.print("#x "); Serial.print(x, DEC");
#define dd d(isCard) d(pinValue) d(keys[i].isKey)

void loop() {
  delay(200);
  bool isCard = 0;
  for (int i = 0; i < KEYS_SIZE; i++){
    byte pinValue;
    pinValue = digitalRead(keys[i].pin);
    isCard = digitalRead(PIN_CARD);
    
    if(isCard == false && pinValue == LOW && keys[i].isKey == true){
      
      while (isCard == false){
      //ALERT
      isCard = digitalRead(PIN_CARD);
      Serial.println("ALERT!!!");
      }
      keys[i].isKey = false;
    }
    else if (isCard == true && keys[i].isKey == true && pinValue == LOW){
      Serial.println("Key get out");
      keys[i].isKey = false;
      }
       if (keys[i].isKey == false && pinValue == HIGH){
        Serial.println("Key get up");
        keys[i].isKey = true;
        }
  }
}
