// C++ code
// LM35
// Diego Gode Bonani 202401000711 - TA
// Tiago Oliveira Macedo 202401583537 - TA
// Caio Cavalcanti Salomão de Souza 202401285731 - TA
// Marcelle Lohane 202402726056 - TA
// Caio Domingues Azevedo 202403191156 - TA
// Bruno Bittencourt Cheuqer - 202402627309 - TA
// Eduardo Jacob Pontes 202402824058 - TA

#define VEL_SOM 340
const int LED_Y = 9;
const int LED_R = 10;
const int LED_B = 11;
const int BUZZER = 12;
const int CHAVE_1 = 22;
const int CHAVE_2 = 23;
const int CHAVE_3 = 24;
const int CHAVE_4 = 25;
const int RGB_RED = 31;
const int RGB_5V = 30;
const int RGB_BLUE = 32;
const int RGB_GREEN = 33;
const int BUTTON = 2;
const int ECHO = 6;
const int TRIG = 7;
const int SELETOR = 27;
const int LM35 = A0; //entrada analog
const float BASE_CELSIUS = 0.4887585532746823069403714565;

int converteBinarioDecimal(int a, int b, int c, int d)  //chave 1 = a, chave 2 = b, chave 3 = c, chave 4 = d 
{
  return a * 1 + b * 2 + c * 4 + d * 8;
}

void ligarLed(char led)
{
  if(led == 'A')
  {
    digitalWrite(LED_Y, HIGH);
  }
  else if(led == 'B')
  {
    digitalWrite(LED_R, HIGH);
  }
  else if(led == 'C')
  {
    digitalWrite(LED_B, HIGH);
  }
  return;
}

void desligarLed(char led)
{
  if(led == 'A')
  {
    digitalWrite(LED_Y, LOW);
  }
  else if(led == 'B')
  {
    digitalWrite(LED_R, LOW);
  }
  else if(led == 'C')
  {
    digitalWrite(LED_B, LOW);
  }
  return;
}

void ligarBuzzer()
{
  analogWrite(BUZZER, 10);
  return;
}

void desligarBuzzer()
{
  digitalWrite(BUZZER, LOW);
  return;
}

void lerTemperatura()
{
  float temperatura;
  temperatura = analogRead(LM35) * BASE_CELSIUS;
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");
  return;
}

float disparaTrig(){  // A função diapara um sinal de trigger no pino pinTrig com largura de 10 microssegundos
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  return pulseIn(ECHO, HIGH);  // determina o tempo em microssegundos
}

void lerDistancia()
{
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duracao = pulseIn(ECHO, HIGH);
  long distancia = duracao * 0.034 / 2; // converte para cm
  Serial.print(F("Distância: "));
  Serial.print(distancia);
  Serial.println(F(" cm"));

  return distancia;
}

void definirCorRGB(int r, int g, int b)
{
  digitalWrite(RGB_5V, HIGH);
  analogWrite(RGB_RED, r);
  analogWrite(RGB_GREEN, g);
  analogWrite(RGB_BLUE, b);
  return;
}

void inicioModoProgramacaoSerial()
{
  String input;

  while(Serial.available() == 0)
  {
    if(digitalRead(SELETOR) == LOW)
    {
      return;
    }
  }

  input = Serial.readString();
  input.trim();

  if(input == "INICIO_PROG")
  {
    Serial.println("Modo Programação ativado.");
    while(true)
    {
      while(Serial.available() == 0){

      }

      input = Serial.readString();
      input.trim();

      if(input == "LED_ON A")
      {
        ligarLed('A');
      }
      else if(input == "LED_OFF A")
      {
        desligarLed('A');
      }
      else if(input == "LED_ON B")
      {
        ligarLed('B');
      }
      else if(input == "LED_OFF B")
      {
        desligarLed('B');
      }
      else if(input == "LED_ON C")
      {
        ligarLed('C');
      }
      else if(input == "LED_OFF C")
      {
        desligarLed('C');
      }
      else if(input == "BUZZ_ON")
      {
        ligarBuzzer();
      }
      else if(input == "BUZZ_OFF")
      {
        desligarBuzzer();
      }
      else if(input == "TEMP_READ")
      {
        lerTemperatura();
      }
      else if(input == "DIST_CHECK")
      {
        lerDistancia();
      }
      else if(input == "RGB_SET_COLOR A RED")
      {
        definirCorRGB(0, 255, 255);
      }
      else if(input == "RGB_SET_COLOR A GREEN")
      {
        definirCorRGB(255, 0, 255);
      }
      else if(input == "RGB_SET_COLOR A BLUE")
      {
        definirCorRGB(255, 255, 0);
      }
      else if(input == "FIM_PROG")
      {
        return;
      }
      else
      {
        Serial.println("Comando não reconhecido. Tente novamente!");
      }
    }
  }
  else
  {
    Serial.println("Modo Programação desativado. Tente o comando INICIO_PROG!");
  }

  
}

void inicioModoProgramacaoBinario()
{
  int a, b, c, d, opcode;
  a = digitalRead(CHAVE_1);
  b = digitalRead(CHAVE_2);
  c = digitalRead(CHAVE_3);
  d = digitalRead(CHAVE_4);
  opcode = converteBinarioDecimal(a, b, c, d);

  while(digitalRead(BUTTON))
  {
    if(digitalRead(SELETOR) == LOW){
      return;
    }
  }

  switch(opcode)
  {
    case 0:
      ligarLed('A');
      break;
    case 1:
      desligarLed('A');
      break;
    case 2:
      ligarLed('B');
      break;
    case 3:
      desligarLed('B');
      break;
    case 4:
      ligarLed('C');
      break;
    case 5:
      desligarLed('C');
      break;
    case 6:
      ligarBuzzer();
      break;
    case 7:
      desligarBuzzer();
      break;
    case 8:
      lerTemperatura();
      break;
    case 9:
      lerDistancia();
      break;
    case 10:
      definirCorRGB(255, 0, 255); //cor verde
      break;
    case 11:
      definirCorRGB(255, 0, 0); //cor ciano
      break;
    case 12:
      definirCorRGB(0, 255, 0); //cor magenta
      break;
    case 13:
      definirCorRGB(0, 0, 0); //cor branca
      break;
    case 14:
      definirCorRGB(random(10,255), random(10,255), random(10,255)); //cor aleátoria
      break;
    case 15:
      definirCorRGB(255, 255, 255); //desligar o led
      break;
  }
  return;
}

void setup()
{
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(CHAVE_1, INPUT);
  pinMode(CHAVE_2, INPUT);
  pinMode(CHAVE_3, INPUT);
  pinMode(CHAVE_4, INPUT);
  pinMode(BUTTON, INPUT);
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_5V, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(SELETOR, INPUT);
  pinMode(ECHO, INPUT);
  pinMode(TRIG,OUTPUT);
  digitalWrite(TRIG,LOW);
  Serial.begin(9600);
}

void loop()
{
  if(digitalRead(SELETOR)){
    inicioModoProgramacaoBinario();
  }
  else{
    inicioModoProgramacaoSerial();
  }

}