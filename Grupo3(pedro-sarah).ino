// GRUPO 3
// 202307476331 Sarah Ferrari (TA)
// 202402841904 Pedro Henrique Silvestre (TA)

// Inclusão de bibliotecas do sensor de temperatura e umidade DHT11
#include <DHT.h>  
#include <DHT_U.h>

// Definições (#define)
#define DHTPIN A0       // pino do Arduíno ligado ao sensor DHT11
#define DHTTYPE DHT11   // DHT 11
#define VEL_SOM 340     // definição da velocidade do som, em metros por seg
// Definindo as chaves para modo de programação binária
#define ch1 2          
#define ch2 3
#define ch3 4
#define ch4 A8
#define chEnter A9      // Chave Enter - executa os comandos determinados pelas chaves binárias
#define chModo A10      // Chave Modo - inicia / encerra o modo de programação binária

// Declaração de variáveis globais
int progSerial = 0;     // Modo de programação serial começa desligado
int pinEcho = 6;        // pino do Arduíno ligado ao pino Echo do sensor
int pinTrig = 5;        // pino do Arduíno ligado ao pino Trig do sensor
char led;
int r, g, b;
int ldr = A1;          // Atribui A1 à variável ldr
int valorldr = 0;      // Declara a variável valorldr como inteiro
DHT dht(DHTPIN, DHTTYPE); // Inicializa o sensor DHT com o pino e tipo definidos
String comando; 
String modo;
int val1 = 0, val2 = 0, val3 = 0, val4 = 0, valEnter = 0, valModo = 0; // Chaves começam com valor 0

// Funções relacionadas a sensores (distância, temperatura, luminosidade)
float disparaTrig() {  // A função dispara um sinal de trigger no pino pinTrig com largura de 10 microssegundos
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
  return pulseIn(pinEcho, HIGH);  // Determina o tempo em microssegundos
}

float lerDistancia() { // Lê o valor do sensor de distância e exibe no monitor serial.
  float tempo, distancia;
  tempo = disparaTrig();  // Dispara sinal de Trig e retorna a duração do pulso (em microssegundos)
  distancia = tempo * 0.000001 * VEL_SOM * 100 / 2;  // Determina a distância em cm
  Serial.println("Distância em centímetros: ");  // Apresenta o resultado
  Serial.println(distancia, DEC);
}

float lerTemp() { // Lê o valor do sensor de temperatura e exibe no monitor serial.
  float t = dht.readTemperature();
  // Testa se retorno é válido, caso contrário algo está errado.
  if (isnan(t)) {
    Serial.println("Failed to read from DHT");
  } else { 
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C");
  }
}

float lerLuminosidade() { // Sensor LDR - tipo de resistor cuja resistência varia de acordo com a intensidade da luz que incide sobre ele. 
  // Quando há pouca luz, a resistência do LDR é alta.
  // Quando há muita luz, a resistência do LDR é baixa.
  valorldr = analogRead(ldr);  // Lê o valor do sensor ldr e armazena na variável valorldr
  Serial.print("Valor lido pelo LDR = "); //Imprime na serial a mensagem Valor lido pelo LDR
  Serial.println(valorldr); //Imprime na serial os dados de valorldr
  if (valorldr < 100) {  // Indica que a resistência do LDR está baixa, ou seja, é um ambiente bem iluminado.
    digitalWrite(7, HIGH);  // Ligar buzzer
    digitalWrite(9, LOW);   // Desligar led B (azul)
  } 
  if (valorldr > 600) {  // Indica que a resistência do LDR está alta, ou seja, ambiente mais escuro, com pouca luz .
    digitalWrite(9, HIGH);  // Ligar led B 
    digitalWrite(7, LOW);   // Desligar buzzer
  } 
  if (valorldr < 600 and valorldr > 100) {   // Não há uma intensidade muito forte nem muito fraca. Meio termo. 
    digitalWrite(7, LOW);   // Desligar buzzer
    digitalWrite(9, LOW);   // Desligar led B 
  }
  delay(100);   // Aguarda 100 milisegundos
}

// Funções relacionadas a LEDs e buzzer
float ligarLed(char led) { // Liga o LED especificado (A, B ou C).
  if (led == 'A') {
    digitalWrite(8, HIGH); // Liga o led verde
  }
  if (led == 'B') {
    digitalWrite(9, HIGH); // Liga o led azul
  }
  if (led == 'C') {
    digitalWrite(10, HIGH); // Liga o led vermelho
  }
}

float desligarLed(char led) { // Desiga o LED especificado (A, B ou C).
  if (led == 'A') {
    digitalWrite(8, LOW); // Desliga o led verde
  }
  if (led == 'B') {
    digitalWrite(9, LOW); // Desliga o led azul
  }
  if (led == 'C') {
    digitalWrite(10, LOW); // Desliga o led vermelho
  }
}

float ligarBuzzer() {
  digitalWrite(7, HIGH); // O Buzzer está conectado ao pino 7 do arduíno
}

float desligarBuzzer() {
  digitalWrite(7, LOW);
}

int definirCorRGB(int r, int g, int b) { // Define a cor do LED RGB.
  if (r == 1) {
    digitalWrite(11, HIGH); // Liga o canal vermelho do LED RGB
  }
  if (g == 1) {
    digitalWrite(12, HIGH); // Liga o canal verde do LED RGB
  }
  if (b == 1) {
    digitalWrite(13, HIGH); // Liga o canal azul do LED RGB
  }
}

int desligarRGB() { // Desliga todos os canais do LED RGB 
  r = 0;
  g = 0;
  b = 0;
  digitalWrite(11, LOW); // Desliga o canal vermelho do LED RGB
  digitalWrite(12, LOW); // Desliga o canal verde do LED RGB
  digitalWrite(13, LOW); // Desliga o canal azul do LED RGB
}

// Funções relacionadas aos modos de programação (serial, binária)
int modoProgSerial() { // O usuário enviará mnemônicos (representações simbólicas) para controlar dispositivos.
  while (progSerial) { // Enquanto progSerial == 1, o sistema permanece no modo de prog. Serial
    if (Serial.available() > 0) {  // Verifica se há sinal disponível na serial
      // leitura do byte de entrada:
      comando = Serial.readString(); // Lê o comando recebido via Serial
      // Ao enviar um comando, o sistema executará a função referente à ele
      if (comando == "LED_ON A") {
        ligarLed('A');
      }
      if (comando == "LED_ON B") {
        ligarLed('B');
      }
      if (comando == "LED_ON C") {
        ligarLed('C');
      }
      if (comando == "LED_OFF A") {
        desligarLed('A');
      }
      if (comando == "LED_OFF B") {
        desligarLed('B');
      }
      if (comando == "LED_OFF C") {
        desligarLed('C');
      }
      if (comando == "BUZZ_ON") {
        ligarBuzzer();
      }
      if (comando == "BUZZ_OFF") {
        desligarBuzzer();
      }
      if (comando == "RGB_SET_COLOR A RED") {
        r = 1;
        definirCorRGB(r, g, b);
      }
      if (comando == "RGB_SET_COLOR A GREEN") {
        g = 1;
        definirCorRGB(r, g, b);
      }
      if (comando == "RGB_SET_COLOR A BLUE") {
        b = 1;
        definirCorRGB(r, g, b);
      }
      if (comando == "RGB_OFF") {
        desligarRGB();
      }
      if (comando == "DIST_CHECK A") {
        lerDistancia();
      }
      if (comando == "TEMP_READ A") {
        lerTemp();
      }
      if (comando == "LIGHT_READ A") {
        lerLuminosidade();
      }
      if (comando == "FIM_SERIAL") { // Sai do modo de programação serial
        progSerial = 0;
      }
    }
  }
}

int modoProgBinaria() { // Comandos são inseridos via chaves binárias e executados ao pressionar uma chave "Enter".
  while (valModo) { // Se a chave Modo == 1, o sistema entra no modo de prog. binária
    // Lendo os valores das chaves
    val1 = digitalRead(ch1);
    val2 = digitalRead(ch2);
    val3 = digitalRead(ch3);
    val4 = digitalRead(ch4);
    valEnter = digitalRead(chEnter);
    valModo = digitalRead(chModo);

    // Verifica as combinações de chaves e executa ações
    if (!val1 && !val2 && !val3 && !val4 && valEnter) {  // 0000 liga o led verde
      ligarLed('A');
    }
    if (!val1 && !val2 && !val3 && val4 && valEnter) {  // 0001 desliga o led verde
      desligarLed('A');
    }
    if (!val1 && !val2 && val3 && !val4 && valEnter) {  // 0010 liga o led azul
      ligarLed('B');
    }
    if (!val1 && !val2 && val3 && val4 && valEnter) {  // 0011 desliga o led azul
      desligarLed('B');
    }
    if (!val1 && val2 && !val3 && !val4 && valEnter) {  // 0100 liga o led vermelho
      ligarLed('C');
    }
    if (!val1 && val2 && !val3 && val4 && valEnter) {  // 0101 desliga o led vermelho
      desligarLed('C');
    }
    if (!val1 && val2 && val3 && !val4 && valEnter) {  // 0110 liga o buzzer
      ligarBuzzer();
    }
    if (!val1 && val2 && val3 && val4 && valEnter) {  // 0111 desliga o buzzer
      desligarBuzzer(); 
    } 
    if (val1 && !val2 && !val3 && !val4 && valEnter){ //1000 liga o led RGB vermelho
      r = 1;
      definirCorRGB(r, g, b);
    }
    if (val1 && !val2 && !val3 && val4 && valEnter){ //1001 liga o led RGB verde
      g = 1;
      definirCorRGB(r, g, b);
    }
    if (val1 && !val2 && val3 && !val4 && valEnter){ //1010 liga o led RGB azul
      b = 1;
      definirCorRGB(r, g, b);
    }
    if (val1 && !val2 && val3 && val4 && valEnter){ //1011 desliga o led RGB
      desligarRGB();
    }
    if (val1 && val2 && !val3 && !val4 && valEnter){ //1100 liga o sensor de distância
      lerDistancia();
    }
    if (val1 && val2 && !val3 && val4 && valEnter){ //1101 liga o sensor de temperatura
      lerTemp();
    }
    if (val1 && val2 && val3 && !val4 && valEnter){ //1110 liga o sensor de luminosidade
      lerLuminosidade();
    }
    if (val1 && val2 && val3 && val4 && valEnter){ //1111 entra no modo de programação serial
      progSerial = 1;
      modoProgSerial();
    }
    delay(5000); // Pausa entre os comandos
  }
}

// Função setup()
void setup() {
  // Inicializa a comunicação serial
  Serial.begin(9600);

  // Definir modos dos pinos
  pinMode(pinTrig, OUTPUT); // Sensor de distância
  pinMode(pinEcho, INPUT);  // Sensor de distância
  pinMode(7, OUTPUT);   // Buzzer
  pinMode(8, OUTPUT);   // LED A (verde)
  pinMode(9, OUTPUT);   // LED B (azul)
  pinMode(10, OUTPUT);  // LED C (vermelho)
  pinMode(11, OUTPUT);  // LED RGB (vermelho)
  pinMode(12, OUTPUT);  // LED RGB (verde)
  pinMode(13, OUTPUT);  // LED RGB (azul)
  // Chaves
  pinMode(ch1, INPUT);
  pinMode(ch2, INPUT);
  pinMode(ch3, INPUT);
  pinMode(ch4, INPUT);
  pinMode(chEnter, INPUT);
  pinMode(chModo, INPUT);
  dht.begin();  // Inicia o sensor de temperatura
}

// Função loop()
void loop() {
  modo = Serial.readString();  // Lê o comando recebido via Serial 
  if (modo == "MODO_SERIAL"){
    progSerial = 1; // Ativa o loop while da função modoProgSerial
    modoProgSerial(); // O sistema agora está no modo de programação binária
  } 
  valModo = digitalRead(chModo); // Verifica o valor da chave de modo
  if (valModo){ 
    modoProgBinaria(); // Inicia o modo de programação binária
  }
}
