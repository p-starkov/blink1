

const int HALL_PIN = 2;
//номер пина с подклчюенным датчиком

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  // встроенный светодиод - вывод
  pinMode(HALL_PIN, INPUT); // Холл - ввод
}

// the loop function runs over and over again forever
void loop() {
  hallResult = digitalRead(HALL_PIN);  // получение значения датчика
  if (hallResult = 1) {
    digitalWrite(LED_BUILTIN, HIGH);
  }                      
  
}