#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int HALL_PIN = 2;
//номер пина с подключенным датчиком

int counter = 0;
//счетчик срабатываний

int lastHallResult = HIGH;
//последнее состояние датчика

// Укажите адрес дисплея: обычно 0x27 или 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);  // встроенный светодиод - вывод
  pinMode(HALL_PIN, INPUT_PULLUP); // Холл - ввод
  lastHallResult = digitalRead(HALL_PIN);
  
  // Инициализация дисплея
  lcd.init();
  
  // Включить подсветку
  lcd.backlight();
    
  // Вывести текст
  lcd.print("Privet!");
}

// the loop function runs over and over again forever
void loop() {
    int hallResult = digitalRead(HALL_PIN);  // получение значения датчика
    if (hallResult == LOW && lastHallResult == HIGH) { // если произошла смена на LOW
        counter = counter + 1;
        Serial.println(counter);
        lcd.clear();
        lcd.print(String("Count: " + counter));
    }
    
    lastHallResult = hallResult;
  
}