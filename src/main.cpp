/*
3 - Alterar projeto 1 para que sejam as duas tasks no nucleo 1, 
    com a mesma prioridade, se alternando. Usar semaforo. 
    O que ocorre se uma task tiver com prioridade maior que a outra?
    Testes de caso: considerando frequências diferentes 500 e 300

    teste1: mesma prioridade, mesmo núcleo, sem vTaskDelay nas tasks, sem semaforo

    teste2: prioridade diferentes, mesmo núcleo, com vTaskDelay de 1 TICK na task de maior prioridade, sem semaforo

    teste3: mesma prioridade, mesmo núcleo, com semaforo binario, sem vTaskDelay nas tasks

    teste4: mesma prioridade, mesmo núcleo, com semaforo binario, com vTaskDelay (1 tick min) nas tasks
    Observações: deste delay depende dos prints seriais que existem na task (testar 10 ticks caso não funcione)

    teste5: similar ao anterior, porém com prioridades diferentes e vTaskDelay apenas na de maior prioridade 
*/

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

SemaphoreHandle_t SMF;
TaskHandle_t handlePWM_t, handleBlink_t;

//Definição dos pinos
#define LedG 2 //pino do led Verde
#define LedR 5 //pino do led Amarelo
#define ButtonPin 13 //Pino do botão

//Rotina de interrupção
void IRAM_ATTR Pressed(){
  xSemaphoreGiveFromISR(SMF, NULL);
}

void handlePWM(void* z);
void handleBlink(void* z);



void setup() {
  //inicialização dos pinos
  pinMode(LedG, OUTPUT);
  pinMode(LedR, OUTPUT);
  pinMode(ButtonPin, INPUT_PULLUP);

  //inicia interrupção
  attachInterrupt(ButtonPin, Pressed, FALLING);

  //inicia semaforo binário
  SMF = xSemaphoreCreateBinary();
  xTaskCreatePinnedToCore(handlePWM, "handlePWM", 4096, NULL, 1, &handlePWM_t, 1);
  xTaskCreatePinnedToCore(handleBlink, "handleBlink", 4096, NULL, 1, &handleBlink_t, 1);
}

void loop() {
  
}

void handlePWM(void* z){
  while(true){
    for(byte i=0; i<=255 ; i++){
      analogWrite(LedG, i);
      delay(50);
    }
    for(byte i=255; i>=0 ; i--){
      analogWrite(LedG, i);
      delay(50);
    }
  }
}

void handleBlink(void* z){
  digitalWrite(LedR, HIGH);
  delay(300);
  digitalWrite(LedR,LOW);
  delay(300);
}