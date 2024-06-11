#include <Arduino.h>

hw_timer_t *timer = NULL;
const int intervaloMicrossegundos = 1000000;

const int ledPins[] = {GPIO_NUM_0, GPIO_NUM_1, GPIO_NUM_2, GPIO_NUM_3};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);
int currentLed = 0;

const int potPin = GPIO_NUM_9;
volatile int valorPot = 0;
volatile unsigned long previousMillis = 0;

void IRAM_ATTR onTimer() {
  valorPot = analogRead(potPin); // Executar a leitura do potenciômetro
  Serial.print("Frequencia do LED: ");
  Serial.println(valorPot/5);
}

void setup() {
  Serial.begin(115200);

  // Inicializar o temporizador
  timer = timerBegin(0, 80, true);

  // Anexar a função de interrupção ao temporizador
  timerAttachInterrupt(timer, &onTimer, true);

  // Definir a frequência do temporizador (intervalo de recarga)
  timerAlarmWrite(timer, intervaloMicrossegundos, true);

  // Ativar a interrupção por estouro do temporizador
  timerAlarmEnable(timer);

  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  // Verifica se tempo suficiente passou para mudar para o próximo LED
  if (millis() - previousMillis >= valorPot) {
    previousMillis = millis();

    // Lógica para acionar LEDs
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], LOW);
    }

    digitalWrite(ledPins[currentLed], HIGH);

    currentLed = (currentLed + 1) % numLeds;
  }
}