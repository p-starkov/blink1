#include <Wire.h>           // библиотека для работы с I2C (нужна LiquidCrystal_I2C)
#include <LiquidCrystal_I2C.h> // библиотека для управления LCD-дисплеем по I2C

// ─── Пины ────────────────────────────────────────────────────────────────────

const int HALL_PIN = 2;    // пин датчика Холла
const int BUTTON_PIN = 3;  // пин кнопки включения/выключения дисплея

// ─── Защита от дребезга ───────────────────────────────────────────────────────

const unsigned long DEBOUNCE_DELAY = 50; // минимальный интервал между срабатываниями (мс)
unsigned long lastTriggerTime = 0;       // время последнего срабатывания датчика Холла
unsigned long lastButtonTrigger = 0;     // время последнего нажатия кнопки

// ─── Состояние программы ─────────────────────────────────────────────────────

int counter = 0;             // счётчик срабатываний датчика Холла
int lastHallResult = HIGH;   // предыдущее состояние датчика Холла
int buttonStatus = HIGH;     // предыдущее состояние кнопки
bool currentLcdOn = true;    // true = дисплей включён, false = выключен

// ─── Объект дисплея ───────────────────────────────────────────────────────────

// Создаём объект lcd с адресом 0x27, размер дисплея 16 столбцов × 2 строки
// Если дисплей не работает — попробуйте адрес 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ─── Инициализация ────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(9600); // запускаем Serial Monitor для отладки

    // Настраиваем пин датчика Холла как вход с подтяжкой к HIGH
    // INPUT_PULLUP: без магнита — HIGH, при срабатывании — LOW
    pinMode(HALL_PIN, INPUT_PULLUP);
    lastHallResult = digitalRead(HALL_PIN); // запоминаем начальное состояние датчика

    // Настраиваем пин кнопки как вход с подтяжкой к HIGH
    // INPUT_PULLUP: кнопка не нажата — HIGH, нажата — LOW
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    buttonStatus = digitalRead(BUTTON_PIN); // запоминаем начальное состояние кнопки
    currentLcdOn = true;                    // считаем дисплей включённым по умолчанию

    lcd.init();      // инициализируем дисплей
    lcd.backlight(); // включаем подсветку
    lcd.print("Privet!"); // выводим приветствие при старте
}

// ─── Основной цикл ────────────────────────────────────────────────────────────

void loop() {
    int hallResult = digitalRead(HALL_PIN); // читаем текущее состояние датчика Холла
    unsigned long now = millis();           // текущее время в миллисекундах с момента включения

    // ── Обработка датчика Холла ──────────────────────────────────────────────

    // Срабатывание: сигнал перешёл с HIGH на LOW (магнит приблизился)
    if (hallResult == LOW && lastHallResult == HIGH) {

        // Проверка дребезга: игнорируем срабатывание, если прошло меньше DEBOUNCE_DELAY мс
        if (now - lastTriggerTime > DEBOUNCE_DELAY) {
            counter++;                  // увеличиваем счётчик
            lastTriggerTime = now;      // запоминаем время этого срабатывания

            Serial.println(counter);    // выводим счётчик в Serial Monitor

            if (currentLcdOn) {         // обновляем дисплей только если он включён
                lcd.clear();
                lcd.setCursor(0, 0);    // курсор в начало первой строки
                lcd.print("Count: ");
                lcd.print(counter);     // выводим число отдельно — так работает правильно
            }
        }
    }

    lastHallResult = hallResult; // сохраняем текущее состояние для следующей итерации

    // ── Обработка кнопки ─────────────────────────────────────────────────────

    int currentButtonStatus = digitalRead(BUTTON_PIN); // читаем текущее состояние кнопки

    // Нажатие: сигнал перешёл с HIGH на LOW (кнопка нажата)
    if (currentButtonStatus == LOW && buttonStatus == HIGH) {

        // Проверка дребезга кнопки
        if (now - lastButtonTrigger > DEBOUNCE_DELAY) {
            lastButtonTrigger = now; // запоминаем время нажатия

            if (currentLcdOn) {
                // Дисплей был включён — выключаем
                lcd.noBacklight();           // гасим подсветку
                Serial.println("Display off");
                currentLcdOn = false;
            } else {
                // Дисплей был выключен — включаем
                lcd.backlight();             // включаем подсветку
                Serial.println("Display on");
                currentLcdOn = true;

                // Восстанавливаем последнее значение счётчика на экране
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Count: ");
                lcd.print(counter);
            }
        }
    }

    buttonStatus = currentButtonStatus; // сохраняем состояние кнопки для следующей итерации
}