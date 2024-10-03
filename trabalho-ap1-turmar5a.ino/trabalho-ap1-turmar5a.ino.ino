/*
Nomes: 
Guilherme Dias - 202402972091
Guilherme Resende - 202402075365
Guilherme Maranhão - 202301165784
Pedro Caravellos - 202302174264
Vitor Ribeiro - 202301215145
André Casemiro - 202303100418
*/

void setup() {
  Serial.begin(9600);       
  pinMode(9, OUTPUT);  // LED A
  pinMode(10, OUTPUT); // LED B
  pinMode(11, OUTPUT); // LED C
  pinMode(8, OUTPUT);  // Buzzer
  pinMode(A0, INPUT);  // Sensor de Temperatura
  pinMode(5, OUTPUT);  // Trigger do Sensor de Distância
  pinMode(6, INPUT);   // Echo do Sensor de Distância
  pinMode(7, INPUT);   // Sensor de Luz
  
  // Configurar pinos para as chaves binárias e chave Enter
  pinMode(12, INPUT);  // Chave de Seleção de Modo
  pinMode(13, INPUT);  // Chave "Enter"
  pinMode(A2, INPUT);  // Chave Binária 4
  pinMode(A3, INPUT);  // Chave Binária 3
  pinMode(A4, INPUT);  // Chave Binária 2
  pinMode(A5, INPUT);  // Chave Binária 1
  
}

void loop() {
  // Verificar se estamos no modo de programação serial ou binário
  if ((digitalRead(12) == HIGH) && (digitalRead(13) == HIGH)) {
    inicioModoProgramacaoBinario();
  } else {
    if (Serial.available() > 0) {
      String comando = Serial.readStringUntil('\n');
      processarComando(comando);
    }
  }
}

// Função para iniciar o modo de programação via serial
void inicioModoProgramacaoSerial() {
  Serial.println("Modo de Programação Serial Ativado");
}

// Função para iniciar o modo de programação via binário
void inicioModoProgramacaoBinario() {
  Serial.println("Modo de Programação Binário Ativado");
  int comandoBinario = lerChavesBinarias();
  processarComandoBinario(comandoBinario);
}

// Função para encerrar o modo de programação
void fimModoProgramacao() {
  Serial.println("Modo de Programação Encerrado");
}

// Função para ligar um LED (A, B ou C)
void ligarLed(char led) {
  if (led == 'A') {
    digitalWrite(9, HIGH);  // Ativar LED A
    Serial.println("LED A Ligado");
  } else if (led == 'B') {
    digitalWrite(10, HIGH); // Ativar LED B
    Serial.println("LED B Ligado");
  } else if (led == 'C') {
    digitalWrite(11, HIGH); // Ativar LED C
    Serial.println("LED C Ligado");
  }
}

// Função para desligar um LED (A, B ou C)
void desligarLed(char led) {
  if (led == 'A') {
    digitalWrite(9, LOW); // Desativar LED A
    Serial.println("LED A Desligado");
  } else if (led == 'B') {
    digitalWrite(10, LOW); // Desativar LED B
    Serial.println("LED B Desligado");
  } else if (led == 'C') {
    digitalWrite(11, LOW); // Desativar LED C
    Serial.println("LED C Desligado");
  }
}

// Função para ligar o buzzer
void ligarBuzzer() {
  digitalWrite(8, HIGH);
  Serial.println("Buzzer Ligado");
}

// Função para desligar o buzzer
void desligarBuzzer() {
  digitalWrite(8, LOW);
  Serial.println("Buzzer Desligado");
}

// Função para ler o valor do sensor de temperatura (exemplo com LM35)
void lerTemperatura() {
  int leitura = analogRead(A0);
  float temperatura = leitura * (5.0 / 1023.0) * 100; // Conversão para graus Celsius
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" C");
}

// Função para ler a distância com o sensor HC-SR04
void lerDistancia() {
  digitalWrite(5, LOW);
  delayMicroseconds(2);
  digitalWrite(5, HIGH);
  delayMicroseconds(10);
  digitalWrite(5, LOW);
  
  long duracao = pulseIn(6, HIGH);
  int distancia = duracao * 0.034 / 2; // Conversão para centímetros
  
  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");
}

// Função para ler o valor do sensor de presença (PIR)
void lerLuz() {
  int luz = digitalRead(7);
  if (luz < 1) {
    Serial.println("Luz Detectada");
  } else {
    Serial.println("Nenhuma Luz");
  }
}

// Função para definir a cor do LED RGB
void definirCorRGB(int r, int g, int b) {
  analogWrite(3, r); // Pino do LED Vermelho
  analogWrite(4, g); // Pino do LED Verde
  analogWrite(2, b); // Pino do LED Azul
  
  Serial.print("Cor RGB Definida: ");
  Serial.print("R=");
  Serial.print(r);
  Serial.print(" G=");
  Serial.print(g);
  Serial.print(" B=");
  Serial.println(b);
}

// Função para ler o estado das chaves binárias e convertê-las para um valor binário
int lerChavesBinarias() {
  int valor = 0;
  valor |= digitalRead(A5) << 3; // Chave 1
  valor |= digitalRead(A4) << 2; // Chave 2
  valor |= digitalRead(A3) << 1;  // Chave 3
  valor |= digitalRead(A2);       // Chave 4
  
  return valor;
}

// Função para processar e executar um comando enviado via serial
void processarComando(String comando) {
  
  if (comando == "LED_ONA") {
    ligarLed('A');
  } else if (comando == "LED_OFFA") {
    desligarLed('A');
  } else if (comando == "LED_ONB") {
    ligarLed('B');
  } else if (comando == "LED_OFFB") {
    desligarLed('B');
  } else if (comando == "LED_ONC") {
    ligarLed('C');
  } else if (comando == "LED_OFFC") {
    desligarLed('C');
  } else if (comando == "BUZZ_ON") {
    ligarBuzzer();
  } else if (comando == "BUZZ_OFF") {
    desligarBuzzer();
  } else if (comando == "TEMP_READA") {
    lerTemperatura();
  } else if (comando == "DIST_CHECKA") {
    lerDistancia();
  } else if (comando == "LUZ_READA") {
    lerLuz();
  } else if (comando == "RGB_SET_COLORARED") {
    definirCorRGB(LOW, HIGH, HIGH); // RGB Vermelho
  } else if (comando == "RGB_SET_COLORAGREEN") {
    definirCorRGB(HIGH, LOW, HIGH); // RGB Verde
  } else if (comando == "RGB_SET_COLORABLUE") {
    definirCorRGB(HIGH, HIGH, LOW); // RGB Azul
  } else if (comando == "INICIO_PROG") {
    inicioModoProgramacaoSerial();
  } else if (comando == "FIM_PROG") {
    fimModoProgramacao();
  }
}

// Função para processar e executar um comando baseado na entrada binária
void processarComandoBinario(int comando) {
  switch (comando) {
    case 0b0000:
      ligarLed('A');
      break;
    case 0b0001:
      desligarLed('A');
      break;
    case 0b0010:
      ligarLed('B');
      break;
    case 0b0011:
      desligarLed('B');
      break;
    case 0b0100:
      ligarBuzzer();
      break;
    case 0b0101:
      desligarBuzzer();
      break;
    case 0b0110:
      lerTemperatura();
      break;
    case 0b0111:
      lerDistancia();
      break;
    case 0b1000:
      lerLuz();
      break;
    case 0b1001:
      definirCorRGB(LOW, HIGH, HIGH); // RGB Vermelho
      break;
    case 0b1010:
      definirCorRGB(HIGH, LOW, HIGH); // RGB Verde
      break;
    case 0b1011:
      definirCorRGB(HIGH, HIGH, LOW); // RGB Azul
      break;
    case 0b1100:
      inicioModoProgramacaoSerial();
      break;
    case 0b1101:
      fimModoProgramacao();
      break;
    case 0b1110:
      ligarLed('C');
      break;
    case 0b1111:
      desligarLed('C');
      break;
    default:
      Serial.println("Comando binário desconhecido");
      break;
  }
}