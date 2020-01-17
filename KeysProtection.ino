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



void loop() {
  delay(200);
  bool isCard = 0;
  for (int i = 0; i < KEYS_SIZE; i++){
    byte pinValue;
    pinValue = digitalRead(keys[i].pin);
    isCard = digitalRead(PIN_CARD);
    
    if(isCard == false && pinValue == LOW && keys[i].isKey == true){
      //ALERT
      Serial.println("ALERT!!!");
    }
    if(isCard == true){
      if (pinValue == LOW && keys[i].isKey == true){
        //ключ взяли
        Serial.println(keys[i].room, DEC);
        Serial.println("ключ забрали");
        keys[i].isKey == false;
        //посылаем на сервер
      }
      if (pinValue == HIGH && keys[i].isKey == false){
        //ключ принесли
        Serial.println(keys[i].room, DEC);
        Serial.println("ключ принесли");
        keys[i].isKey == true;
        //посылаем на сервер
      }
    }
  }
}
