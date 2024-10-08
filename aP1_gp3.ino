// Augusto, 202051692553 - TA
// Barbara Malta, 202402898892 - TA
// Breno Chaves, 202402798502 - TA
// Guilherme Pardelhas, 20242697668 - TA
// João Gabriel, 202402697706 - TA
// Júlia Curto, 202402192477 - TA
// Lucca Barcelos, 202402630776 - TA

#include <DHT11.h>
#include <Ultrasonic.h>
#include <DHT.h>
 
#define DHTPIN A3 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
 
int pinoR = 11;
int pinoG = 10;

#define pinoR 11
#define pinoG 10


// Definições dos pinos e variáveis de controle
#define BIN1 46
#define BIN2 48
#define BIN3 50
#define BIN4 52

#define TEMP 10
#define ECHO_PIN 11

#define LEDA 5
#define LEDB 6
#define LEDC 7

#define BUZZ 2

#define trig 22
#define echo 24
#define CHAVE 12

#define sensorPresenca 8

#define ENTER 3

DHT dht(DHTPIN, DHTTYPE); 

bool isProgramActive = false;
int modoAtual = 0;

bool medirDistancia = false;  // Variável para controlar a execução contínua da medição de distância
bool detectarMov = false;     // Variável para controlar a execução contínua do sensor de movimento
bool detecTemp = false;


double distancia = 0;


int VAL1 = 0;
int VAL2 = 0;
int VAL3 = 0;
int VAL4 = 0;
int VAL5 = 0;
int VAL6 = digitalRead(ENTER);
int valorSensor = 0;

// Função para desligar todos os componentes
void desligarTudo() {
    digitalWrite(LEDA, LOW);
    digitalWrite(LEDB, LOW);
    digitalWrite(LEDC, LOW);
    digitalWrite(BUZZ, LOW);
    digitalWrite(pinoR, LOW);
    digitalWrite(pinoG, LOW);
    while (true) {
        // Fica preso aqui para garantir que o programa pare
    }
}

// Função para medir a distância usando sensor ultrassônico
void distancias() {
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    distancia = pulseIn(echo, HIGH);
    distancia = distancia * 340;
    distancia = distancia / 2;
    distancia = distancia / 10000;
    Serial.println(distancia);
}

void detectarMovimento() {
  valorSensor = digitalRead(sensorPresenca);
  if (valorSensor == true) {
    Serial.println("TEM GENTE");
  } else if (valorSensor == false) {
    Serial.println("NÃO TEM GENTE");
    
  } else {
    Serial.println("NÃO TEM GENTE");

  }
}

void temperatura() {
  // A leitura da temperatura e umidade pode levar 250ms!
  // O atraso do sensor pode chegar a 2 segundos.
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // testa se retorno é valido, caso contrário algo está errado.
  if (isnan(t) || isnan(h)) 
  {
    Serial.println("Failed to read from DHT");
  } 
  else
  {
    Serial.print("Umidade: ");
    Serial.print(h);
    Serial.print(" %t");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" °C");
    delay(1000);
  }
}


void acenderLedVermelho() {
  digitalWrite(pinoR, HIGH);
  digitalWrite(pinoG, LOW);
  Serial.println("LED Vermelho Aceso");
}

void acenderLedVerde() {
  digitalWrite(pinoR, LOW);
  digitalWrite(pinoG, HIGH);
  Serial.println("LED Verde Aceso");
}

// Função para processar comandos recebidos via monitor serial
void serialTeste() {
    if (Serial.available() > 0) {
        String comando = Serial.readStringUntil('\n'); // Lê a linha de comando até nova linha
        comando.trim(); // Remove espaços em branco extras

        if (comando == "FIM_PROG") {
            Serial.println("Programação desativada via comando serial.");
            desligarTudo();
            modoAtual = 0;
        } else if (comando == "DIST_CHECKA") {
            distancias();
        } else if (comando == "PRES_READA") {
            detectarMovimento();
        } else if (comando == "LED_ONA") {
            digitalWrite(LEDA, HIGH);
        } else if (comando == "LED_OFFA") {
            digitalWrite(LEDA, LOW);
        } else if (comando == "LED_ONB") {
            digitalWrite(LEDB, HIGH);
        } else if (comando == "LED_OFFB") {
            digitalWrite(LEDB, LOW);
        } else if (comando == "LED_ONC") {
            digitalWrite(LEDC, HIGH);
        } else if (comando == "LED_OFFC") {
            digitalWrite(LEDC, LOW);
        } else if (comando == "BUZZ_ON") {
            digitalWrite(BUZZ, HIGH);
        } else if (comando == "BUZZ_OFF") {
            digitalWrite(BUZZ, LOW);
        } else if (comando == "TEMP_READA") {
            temperatura();   
        } else if (comando == "RGB_SET_COLORARED") {
          acenderLedVermelho(); 
        } else if (comando == "RGB_SET_COLORAGREEN"){
          acenderLedVerde();
        }
    }
}


// Função para processar comandos recebidos via binário
void binarioTeste() {
    // Verifica se a combinação é 1101 para encerrar a programação
    if (VAL1 == 1 && VAL2 == 0 && VAL3 == 1 && VAL4 == 1) {
        Serial.println("Programação encerrada");
        isProgramActive = false;
        desligarTudo();
        modoAtual = 0;
    }

    if (isProgramActive) {
        if((VAL1 == 0) && (VAL2 == 0) && (VAL3 == 0) && (VAL4 == 0)) {
          Serial.println("Digite enter para continuar: ");
          if(VAL6 == 0){
            digitalWrite(LEDA, HIGH);

          }

        }
        else if ((VAL1 == 1) && (VAL2 == 0) && (VAL3 == 0) && (VAL4 == 0) && !(VAL6)) {
            digitalWrite(LEDA, LOW);
        } 
        else if ((VAL1 == 0) && (VAL2 == 1) && (VAL3 == 0) && (VAL4 == 0) && !(VAL6)) {
            digitalWrite(LEDB, HIGH);
        }
        else if ((VAL1 == 1) && (VAL2 == 1) && (VAL3 == 0) && (VAL4 == 0) && !(VAL6)) {
            digitalWrite(LEDB, LOW);
        } 
        else if ((VAL1 == 0) && (VAL2 == 0) && (VAL3 == 1) && (VAL4 == 0) && !(VAL6)) {
            digitalWrite(BUZZ, HIGH);
        }
        else if ((VAL1 == 1) && (VAL2 == 0) && (VAL3 == 1) && (VAL4 == 0) && !(VAL6)) {
            digitalWrite(BUZZ, LOW);
        }
        else if ((VAL1 == 1) && (VAL2 == 1) && (VAL3 == 1) && (VAL4 == 0) && !(VAL6)) {
            distancias(); // Executa medição de distância
        }
        else if ((VAL1 == 0) && (VAL2 == 0) && (VAL3 == 0) && (VAL4 == 1) && !(VAL6)) {
            detectarMovimento();
        } else if ((VAL1 == 0) && (VAL2 == 1) && (VAL3 == 1) && (VAL4 == 0) && !(VAL6)) {
            temperatura();
        } else if ((VAL1 == 1) && (VAL2 == 0) && (VAL3 == 0) && (VAL4 == 1) && !(VAL6)) {
            acenderLedVermelho();
        } else if ((VAL1 == 0) && (VAL2 == 1) && (VAL3 == 0) && (VAL4 == 1) && !(VAL6)) {
            acenderLedVerde();
        } else if ((VAL1 == 0) && (VAL2 == 1) && (VAL3 == 1) && (VAL4 == 1) && !(VAL6)) {
            digitalWrite(LEDC, HIGH);
        } else if ((VAL1 == 1) && (VAL2 == 1) && (VAL3 == 1) && (VAL4 == 1) && !(VAL6)) {
            digitalWrite(LEDC, LOW);
        }

    }
}

// Configuração inicial do Arduino
void setup() {
    pinMode(BIN1, INPUT);
    pinMode(BIN2, INPUT);
    pinMode(BIN3, INPUT);
    pinMode(BIN4, INPUT);
    pinMode(LEDA, OUTPUT);
    pinMode(BUZZ, OUTPUT);
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
    pinMode(pinoR, OUTPUT);
    pinMode(pinoG, OUTPUT);
    pinMode(CHAVE, INPUT);
    pinMode(ENTER, INPUT_PULLUP);
    pinMode(sensorPresenca, INPUT);
    dht.begin();

    
  
    Serial.begin(9600);
    Serial.println("Digite os comandos no formato: 'comando'");
    Serial.println("Comandos disponíveis:");
    Serial.println(" - ativar: INICIO_PROG");
    Serial.println(" - desativar: FIM_PROG");
}

void loop() {
    // Lê os valores das entradas binárias
    VAL1 = digitalRead(BIN1);
    VAL2 = digitalRead(BIN2);
    VAL3 = digitalRead(BIN3);
    VAL4 = digitalRead(BIN4);
    VAL5 = digitalRead(CHAVE);
    VAL6 = digitalRead(ENTER);

    delay(500);

    // Verifica se o programa está no estado inicial e aguarda comandos para ativar
    if (modoAtual == 0) {
        if (Serial.available() > 0) {
            String comando = Serial.readStringUntil('\n');
            comando.trim();
            if (comando == "INICIO_PROG") {
                Serial.println("Programação ativada via comando serial.");
                modoAtual = 1;
            }
        }
        if(VAL6 == 0) {
          Serial.println("Binário Available");
          if (VAL1 == 0 && VAL2 == 0 && VAL3 == 1 && VAL4 == 1) {
              Serial.println("Programação ativada via binário");
              isProgramActive = true;
              modoAtual = 2;
          }
        }
    }

    // Verifica o modo atual e executa a função correspondente
    if (modoAtual == 1) {
        serialTeste(); 
    } else if (modoAtual == 2) {
      //funcao if
        binarioTeste(); // Checa e executa com base nos valores binários
    }

}
