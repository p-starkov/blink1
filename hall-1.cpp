

const int HALL_PIN = 2;
//номер пина с подключенным датчиком

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  // встроенный светодиод - вывод
  pinMode(HALL_PIN, INPUT); // Холл - ввод
}

// the loop function runs over and over again forever
void loop() {
    int hallResult;
    hallResult = digitalRead(HALL_PIN);  // получение значения датчика
    if (hallResult == HIGH) { // если проищошло срабатывание - включить светодиод
        digitalWrite(LED_BUILTIN, HIGH);
    }

    delay(1000); //через секунду после проверки светодиод выключить при любом варианте
    digitalWrite(LED_BUILTIN, LOW);
  
}