#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN A0 //Define o pino do sensor de temperatura
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Definições dos pinos
#define LEDA 9
#define LEDB 10
#define LEDC 11
#define BUZZER 8
#define TRIGGER A14
#define ECHO 6
#define TEMP A0
#define PRES 7
#define RGB_VERMELHO A9
#define RGB_VERDE A10
#define RGB_AZUL A11

//Definição das chaves binárias
#define Ch1 2
#define Ch2 3
#define Ch3 4
#define Ch4 5

// Chave enter e seletora
#define ENTER 12
#define SELETORA A4

void setup() {

  pinMode(SELETORA, INPUT);
  pinMode(ENTER, INPUT_PULLUP);
  pinMode(LEDA, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDC, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  dht.begin();
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(RGB_VERMELHO, OUTPUT);
  pinMode(RGB_VERDE, OUTPUT);
  pinMode(RGB_AZUL, OUTPUT);

  Serial.begin(9600);

  // Configura o Trigger
  pinMode(TRIGGER, OUTPUT);
  digitalWrite(TRIGGER, LOW);
  pinMode(ECHO, INPUT); // ECHO entrada
}

void loop() {
  int enter, chave1, chave2, chave3, chave4;
  enter = digitalRead(ENTER);
  chave1 = digitalRead(Ch1);
  chave2 = digitalRead(Ch2);
  chave3 = digitalRead(Ch3);
  chave4 = digitalRead(Ch4);

  int modo_operacao = digitalRead(SELETORA);


  //MODO BINARIO  
  if (!modo_operacao) {
    if (!(enter) && (chave1 == 1) && (chave2 == 1) && (chave3 == 0) && (chave4 == 0) ){
      Serial.println("Você está operando no modo binário");

      while ( !((chave1 == 1) && (chave2 == 1) && (chave3 == 0) && (chave4 == 1) && !(enter)) ){
          enter = digitalRead(ENTER);
          chave1 = digitalRead(Ch1);
          chave2 = digitalRead(Ch2);
          chave3 = digitalRead(Ch3);
          chave4 = digitalRead(Ch4);

          if ( !(enter) && (chave1 == 0) && (chave2 == 0) && (chave3 == 0) && (chave4 == 0)){
            ligarLed('A');
          }

          if ( !(enter) && (chave1 == 0) && (chave2 == 0) && (chave3 == 0) && (chave4 == 1)){
            desligarLed('A');
          }

          if ( !(enter) && (chave1 == 0) && (chave2 == 0) && (chave3 == 1) && (chave4 == 0)){
            ligarLed('B');
          }

          if ( !(enter) && (chave1 == 0) && (chave2 == 0) && (chave3 == 1) && (chave4 == 1)){
            desligarLed('B');
          }

          if ( !(enter) && (chave1 == 0) && (chave2 == 1) && (chave3 == 0) && (chave4 == 0)){
            ligarBuzzer();
          }

          if ( !(enter) && (chave1 == 0) && (chave2 == 1) && (chave3 == 0) && (chave4 == 1)){
            desligarBuzzer();
          }

          if ( !(enter) && (chave1 == 0) && (chave2 == 1) && (chave3 == 1) && (chave4 == 0)){
            lerTemperatura();
          }

          if ( !(enter) && (chave1 == 0) && (chave2 == 1) && (chave3 == 1) && (chave4 == 1)){
            lerDistancia();
          }

          if ( !(enter) && (chave1 == 1) && (chave2 == 0) && (chave3 == 0) && (chave4 == 0)){
            lerPresenca();
          }

          if ( !(enter) && (chave1 == 1) && (chave2 == 0) && (chave3 == 0) && (chave4 == 1)){
            RGB_vermelho();
          }

          if ( !(enter) && (chave1 == 1) && (chave2 == 0) && (chave3 == 1) && (chave4 == 0)){
            RGB_verde();
          }

          if ( !(enter) && (chave1 == 1) && (chave2 == 0) && (chave3 == 1) && (chave4 == 1)){
            RGB_azul();
          }

          if ( !(enter) && (chave1 == 1) && (chave2 == 1) && (chave3 == 0) && (chave4 == 1)){
            Serial.println("Fim do modo binário");
            break;
          }

          if ( !(enter) && (chave1 == 1) && (chave2 == 1) && (chave3 == 1) && (chave4 == 0)){
            ligarLed('C');
          }

          if ( !(enter) && (chave1 == 1) && (chave2 == 1) && (chave3 == 1) && (chave4 == 1)){
            desligarLed('C');
          }

          delay(1200); //esperar

      }
    }
  }

  //MODO MNEMÔNICO
  if (modo_operacao){
    String mnemonico;

    while (!Serial.available() && (modo_operacao)){
      // Espera que o usuário insira um valor
    }

    mnemonico = Serial.readString();
    mnemonico.trim(); // tratamento de string

    if (mnemonico == "INICIO_PROG") {
      Serial.println("Modo Monitor Serial ativado");

      while (mnemonico != "FIM_PROG"){

        while (!Serial.available() && (modo_operacao)){
        // Espera que o usuário insira um valor
        }

        mnemonico = Serial.readString();
        mnemonico.trim(); // tratamento de string

        Serial.println("O Arduino está trabalhando na resposta...");
        delay(1000);

        if (mnemonico == "LED_ON A"){
          ligarLed('A');
        }

        if (mnemonico == "LED_OFF A"){
          desligarLed('A');
        }

        if (mnemonico == "LED_ON B"){
          ligarLed('B');
        }

        if (mnemonico == "LED_OFF B"){
          desligarLed('B');
        }

        if (mnemonico == "BUZZ_ON"){
          ligarBuzzer();
        }

        if (mnemonico == "BUZZ_OFF"){
          desligarBuzzer();
        }

        if (mnemonico == "TEMP_READ A"){
          lerTemperatura();
        }

        if (mnemonico == "DIST_CHECK A"){
          lerDistancia();
        }

        if (mnemonico == "PRES_READ A"){
          lerPresenca();
        }

        if (mnemonico == "RGB_SET_COLOR A RED"){
          RGB_vermelho();
        }

        if (mnemonico == "RGB_SET_COLOR A GREEN"){
          RGB_verde();
        }

        if (mnemonico == "RGB_SET_COLOR A BLUE"){
          RGB_azul();
        }

        if (mnemonico == "LED_ON C"){
          ligarLed('C');
        }

        if (mnemonico == "LED_OFF C"){
          desligarLed('C');
        }
      }

      Serial.println("Modo serial desativado");

    }
  }

  delay(1000);
}

// Ligar e desligar os leds
void ligarLed(char led) {
  if (led == 'A') {
    digitalWrite(LEDA, HIGH);
  }
  else if (led == 'B') {
    digitalWrite(LEDB, HIGH);
  }
  else if (led == 'C') {
    digitalWrite(LEDC, HIGH);
  }
}

void desligarLed(char led) {
  if (led == 'A') {
    digitalWrite(LEDA, LOW);
  }
  else if (led == 'B') {
    digitalWrite(LEDB, LOW);
  }
  else if (led == 'C') {
    digitalWrite(LEDC, LOW);
  }
}

// Ligar e desligar o buzzer
void ligarBuzzer() {
  tone(BUZZER, 50); // Liga o buzzer
}

void desligarBuzzer() {
  noTone(BUZZER); // Desliga o buzzer
}

// Le a temperatura
void lerTemperatura() {
  float t = dht.readTemperature();
  if(isnan(t)) {
    Serial.println("Não foi possível ler a temperatura");
  }
  else {
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" C");
  }

}

//Le a distancia
void lerDistancia() {

  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);


  long duracao = pulseIn(ECHO, HIGH);
  float distancia = duracao * 0.034 / 2;
  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");
}

// Sensor presença
void lerPresenca() {
  int estado = digitalRead(PRES);
  if (estado == HIGH) {
    Serial.println("Presença detectada!");
  } else {
    Serial.println("Nenhuma presença.");
  }
}

// RGB cores
void RGB_vermelho() {
  digitalWrite(RGB_AZUL, LOW);
  digitalWrite(RGB_VERDE, LOW);
  digitalWrite(RGB_VERMELHO, HIGH);
}

void RGB_azul() {
  digitalWrite(RGB_AZUL, HIGH);
  digitalWrite(RGB_VERDE, LOW);
  digitalWrite(RGB_VERMELHO, LOW);
}

void RGB_verde() {
  digitalWrite(RGB_AZUL, LOW);
  digitalWrite(RGB_VERDE, HIGH);
  digitalWrite(RGB_VERMELHO, LOW);
}